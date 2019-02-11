/*
 * ChbControl.c
 *
 *  Created on: Jan 27, 2019
 *      Author: y3437
 */

#include "F28x_Project.h"
#include "Syncopation_Data.h"

#define TS 2e-5

float Grid_Freq = 0;
float Grid_Angle = 0;
float Grid_Amplitude = 0;



extern float SinglePhasePLL(float Vac, float *Freq, float *Vac_amp);


float Vdc_filter = 0;
float vdc_h1_x1 = 0;
float vdc_h1_x2 = 0;
float vdc_h2_x1 = 0;
float vdc_h2_x2 = 0;
float vdc_h0_x = 160;
float vdc_sogi_k = 1.414;
float vdc_h0_k = 100;

//void v_sogi_k_p(float arg_2, float arg_3)
//{
//    vdc_sogi_k = arg_2;
//    vdc_h0_k = arg_3;
//}

#pragma CODE_SECTION(voltage_sogi, ".TI.ramfunc");
float voltage_sogi(float Vdc, float Freq)
{
    float v_omega_T = Freq * TS;
    float v_2omega_T = 2 * Freq * TS;

    float vdc_h1_x1_n;
    float vdc_h1_x2_n;
    float vdc_h2_x1_n;
    float vdc_h2_x2_n;
    float vdc_h0_x_n;

    float v_sogi_error = vdc_sogi_k * (Vdc - vdc_h1_x1 - vdc_h2_x1 - vdc_h0_x);

    vdc_h0_x_n = vdc_h0_x + vdc_h0_k * v_sogi_error * TS;

    vdc_h1_x1_n = __cospuf32(v_omega_T) * vdc_h1_x1 - __sinpuf32(v_omega_T) * vdc_h1_x2 + TS * 377 * v_sogi_error;
    vdc_h1_x2_n = __sinpuf32(v_omega_T) * vdc_h1_x1 + __cospuf32(v_omega_T) * vdc_h1_x2;

    vdc_h2_x1_n = __cospuf32(v_2omega_T) * vdc_h2_x1 - __sinpuf32(v_2omega_T) * vdc_h2_x2 + TS * 377 * 2 * v_sogi_error;
    vdc_h2_x2_n = __sinpuf32(v_2omega_T) * vdc_h2_x1 + __cospuf32(v_2omega_T) * vdc_h2_x2;


    vdc_h1_x1 = vdc_h1_x1_n;
    vdc_h1_x2 = vdc_h1_x2_n;
    vdc_h2_x1 = vdc_h2_x1_n;
    vdc_h2_x2 = vdc_h2_x2_n;
    vdc_h0_x = vdc_h0_x_n;

    return vdc_h0_x;
}

float v_loop_inte = 0;
float v_loop_error = 0;

float v_loop_kp = 0.04;
float v_loop_ki = 0.4;

//float v_loop_kp = 0.05;
//float v_loop_ki = 0.5;
//
//void v_loop_pi(float arg_2, float arg_3)
//{
//    v_loop_kp = arg_2;
//    v_loop_ki = arg_3;
//}

#pragma CODE_SECTION(voltage_loop, ".TI.ramfunc");
float voltage_loop(float Vdc_ref, float Vdc_filtered)
{
    v_loop_error = Vdc_ref - Vdc_filtered;

    v_loop_inte += TS * v_loop_error;

    float I_ac_ref = v_loop_kp * v_loop_error + v_loop_ki * v_loop_inte;

    return I_ac_ref;
}

// I_loop_variables
float i_kp = 8;
float i_kr = 20;
//float i_kp = 40;
//float i_kr = 100;
float i_sogi_x1 = 0;
float i_sogi_x2 = 0;
float i_dc_offset_inte = 0;
float i_sogi_error;
float i_omega_h1_T;
// End of I_loop_variables

//void current_loop_pr(float arg_2, float arg_3)
//{
//    i_kp = arg_2;
//    i_kr = arg_3;
//}

#pragma CODE_SECTION(current_loop, ".TI.ramfunc");
float current_loop(float Iac_ref, float Iac, float Freq)
{
    i_omega_h1_T = Freq * TS;

    float i_sogi_h1_x1_n;
    float i_sogi_h1_x2_n;

    i_sogi_error = -Iac_ref + Iac;

    i_sogi_h1_x1_n = __cospuf32(i_omega_h1_T) * i_sogi_x1 - __sinpuf32(i_omega_h1_T) * i_sogi_x2 + TS * 377 * i_sogi_error;
    i_sogi_h1_x2_n = __sinpuf32(i_omega_h1_T) * i_sogi_x1 + __cospuf32(i_omega_h1_T) * i_sogi_x2;

    i_sogi_x1 = i_sogi_h1_x1_n;
    i_sogi_x2 = i_sogi_h1_x2_n;

    float Vac_ref = i_kp * i_sogi_error + i_kr * i_sogi_x1;

    return Vac_ref;
}

float Vdc_ref = 20;
float Iac_ref = 0;
float Iac_mag = 0;
float Vac_ref = 0;


void ResetState()
{
    i_sogi_x1 = 0;
    i_sogi_x2 = 0;
    i_dc_offset_inte = 0;

    v_loop_inte = 0;

    Iac_ref = 0;
    Iac_mag = 0;
    Vac_ref = 0;
}

Uint16 chb_state;

Uint16 trigger_state = 0;
Uint16 trigger_count = 0;
Uint16 trigger_limit = 4000;

void Chb_Trigger()
{
    chb_state = 2;
    trigger_state = 1;
    trigger_count = 0;
}

void Chb_on()
{
    if(chb_state == 1)
        chb_state = 2;
    ResetState();
}

void Chb_off()
{
    if(chb_state == 2)
        chb_state = 1;
    ResetState();
}

float ChbControl(float Vac, float Vdc_sec, float Iac)
{
    Grid_Angle = SinglePhasePLL(Vac, &Grid_Freq, &Grid_Amplitude);
    Vdc_filter = voltage_sogi(Vdc_sec, Grid_Freq);

    Vac_ref = 0;

    if(chb_state == 2)
    {
        Iac_mag = voltage_loop(Vdc_ref, Vdc_filter);

        if(Iac_mag > 15)
            Iac_mag = 15;


        Iac_ref = Iac_mag * __cospuf32(Grid_Angle);

        Vac_ref = current_loop(Iac_ref, Iac, Grid_Freq);
    }

//    if(trigger_state == 1)
//    {
//        DataLog_Logging(trigger_count, Vdc_sec, (Vac - Vac_ref), Iac, (Iac_ref - Iac));
//
//        trigger_count++;
//        if(trigger_count>=trigger_limit)
//        {
//            if(chb_state!=3)
//                chb_state = 1;
//            trigger_state = 0;
//            ResetState();
//            DataLog_StartToSend(trigger_limit);
//        }
//    }
    return Vac_ref;
}

int LcControl(float L_Angle,float Iac_ref_m,float Iac)
{
    float Iac_ref = Iac_ref_m*__cospuf32(L_Angle);
    Vac_ref = current_loop(Iac_ref, Iac, 60);
    return (int) (Vac_ref*2000/Vdc_ref+2000)+100;
}

//
//

//
////float *probe;
////#pragma CODE_SECTION(DataLogWr, ".TI.ramfunc");
////void DataLogWr()
////{
////    if(DataLog_state == 0)
////    {
////        DataLog[DataLog_index] = omega_h1;
////        DataLog_index++;
////        DataLog[DataLog_index] = V_ac_amp;
////        DataLog_index++;
////        DataLog[DataLog_index] = Iac_amp;
////        DataLog_index++;
////        DataLog[DataLog_index] = V_dc_filtered;
////        DataLog_index++;
////        if(DataLog_index >= 20000)
////        {
////            DataLog_index = 0;
////            DataLog_state = 2;
//////            control_state = 0;
////        }
////    }
////}
////
////#pragma CODE_SECTION(ResetStateVariables, ".TI.ramfunc");
////void ResetStateVariables()
////{
////    i_sogi_x1 = 0;
////    i_sogi_x2 = 0;
////    i_dc_offset_inte = 0;
////    v_loop_inte = 0;
////}
////
////void V_dc_ref_set(float arg_2)
////{
////    if((arg_2 > 200) && (arg_2 < 850))
////        V_dc_ref = arg_2;
////}
////
////void V_dc_ref_inc()
////{
////    if(V_dc_ref < 850)
////        V_dc_ref += 1;
////}
////
////void V_dc_ref_dec()
////{
////    if(V_dc_ref > 200)
////        V_dc_ref -= 1;
////}
////
////void Q_ref_set(float arg_2)
////{
////    if((arg_2 > -5) && (arg_2 < 5))
////        I_react = arg_2;
////}
////
////void I_offset(float arg_2)
////{
//////    EPwm9Regs.CMPA.bit.CMPA = (Uint16)arg_2;
////}
////
////void I_loop_PR(float arg_2,float arg_3)
////{
//////    i_kp = arg_2;
//////    i_ki = arg_3;
////}
