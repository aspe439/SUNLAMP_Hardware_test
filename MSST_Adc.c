/*
 * MSST_ADC.c
 *
 *  Created on: Dec 14, 2016
 *      Author: Yang Lei
 */

#include "MSST_PWM.h"
#include "F28x_Project.h"
#include "MSST_GlobalFunctions.h"
#include "Syncopation_SCI.h"
#include "Syncopation_Data.h"
//#define Protection
//#define DAB
#define L_con
#define TS 2e-5
#define omega 377;

Uint16 V_DC;
Uint16 I_DC_1;
Uint16 I_DC_2;
Uint16 I_AC_1;
Uint16 I_AC_2;
Uint16 LIGHT;
Uint16 V_AC;

Uint16 TEMP_1;
Uint16 TEMP_2;
Uint16 TEMP_3;
Uint16 TEMP_4;

void ControlLoop(void);
void Adc_A_Init();
void Adc_B_Init();
//extern float SinglePhasePLL(float Vac, float *Freq, float *Vac_amp);
//extern float ChbControl(float Vac, float Vdc_sec, float Iac);
extern float current_loop(float Iac_ref, float Iac, float Freq);
extern int LcControl(float L_Angle,float Iac_ref_m,float Iac);
void AdcInit()
{
    Adc_A_Init();
    Adc_B_Init();

    PieCtrlRegs.PIEIER1.bit.INTx2 = 1;  // ADC-B interrupt 1
    EALLOW;
    PieVectTable.ADCB1_INT = &ControlLoop;
    EDIS;
}

void Adc_A_Init()
{
    EALLOW;
    AdcaRegs.ADCCTL1.bit.ADCPWDNZ = 1;  // ADC-A power up
    AdcaRegs.ADCCTL1.bit.INTPULSEPOS = 1; // Interrupt position at the end of conversion
    AdcaRegs.ADCCTL2.bit.PRESCALE = 2; // ADC Clock = SYSCLK / 2;
    AdcaRegs.ADCCTL2.bit.RESOLUTION = 0; // 12-bit resolution
    AdcaRegs.ADCCTL2.bit.SIGNALMODE = 0; // Single-ended signal mode
    AdcaRegs.ADCBURSTCTL.bit.BURSTEN = 0; // Disable burst mode
    AdcaRegs.ADCINTFLGCLR.all = 15; // Clear all the interrupt flags
    AdcaRegs.ADCINTOVFCLR.all = 15; // Clear all the interrupt overflow flags
    AdcaRegs.ADCINTSEL1N2.all = 0; //  Enable ADCINT1 and it is triggered by EOC3, disable ADCINT2
    AdcaRegs.ADCINTSEL3N4.all = 0; //  Disable ADCINT3 and ADCINT4
    AdcaRegs.ADCSOCPRICTL.all = 0; // Round robin control. Conversion starts from SOC0.
    AdcaRegs.ADCINTSOCSEL1.all = 0; // ADC interrupt doesn't trigger any SOC
    AdcaRegs.ADCINTSOCSEL2.all = 0; // ADC interrupt doesn't trigger any SOC

    AdcaRegs.ADCSOC0CTL.bit.TRIGSEL = 5; // SOC-0 software start
    AdcaRegs.ADCSOC0CTL.bit.CHSEL = 2; // SOC-0 convert channel 2, which is ADC_A2 pin
    AdcaRegs.ADCSOC0CTL.bit.ACQPS = 19; // Sample window is 49+1 clock cycles

    AdcaRegs.ADCSOC1CTL.bit.TRIGSEL = 5; // SOC-1 software start
    AdcaRegs.ADCSOC1CTL.bit.CHSEL = 3; // SOC-1 convert channel 3, which is ADC_A3 pin
    AdcaRegs.ADCSOC1CTL.bit.ACQPS = 19; // Sample window is 49+1 clock cycles

    AdcaRegs.ADCSOC2CTL.bit.TRIGSEL = 5; // SOC-2 software start
    AdcaRegs.ADCSOC2CTL.bit.CHSEL = 4; // SOC-2 convert channel 4, which is ADC_A4 pin
    AdcaRegs.ADCSOC2CTL.bit.ACQPS = 19; // Sample window is 49+1 clock cycles

    AdcaRegs.ADCSOC3CTL.bit.TRIGSEL = 5; // SOC-3 software start
    AdcaRegs.ADCSOC3CTL.bit.CHSEL = 5; // SOC-3 convert channel 5, which is ADC_A5 pin
    AdcaRegs.ADCSOC3CTL.bit.ACQPS = 19; // Sample window is 49+1 clock cycles
    EDIS;
}

void Adc_B_Init()
{
    EALLOW;
    AdcbRegs.ADCCTL1.bit.ADCPWDNZ = 1;  // ADC-A power up
    AdcbRegs.ADCCTL1.bit.INTPULSEPOS = 1; // Interrupt position at the end of conversion
    AdcbRegs.ADCCTL2.bit.PRESCALE = 2; // ADC Clock = SYSCLK / 2;
    AdcbRegs.ADCCTL2.bit.RESOLUTION = 0; // 12-bit resolution
    AdcbRegs.ADCCTL2.bit.SIGNALMODE = 0; // Single-ended signal mode
    AdcbRegs.ADCBURSTCTL.bit.BURSTEN = 0; // Disable burst mode
    AdcbRegs.ADCINTFLGCLR.all = 15; // Clear all the interrupt flags
    AdcbRegs.ADCINTOVFCLR.all = 15; // Clear all the interrupt overflow flags
    AdcbRegs.ADCINTSEL1N2.all = 0x0025; //  Enable ADCINT1 and it is triggered by EOC5, disable ADCINT2
    AdcbRegs.ADCINTSEL3N4.all = 0; //  Disable ADCINT3 and ADCINT4
    AdcbRegs.ADCSOCPRICTL.all = 0; // Round robin control. Conversion starts from SOC0.
    AdcbRegs.ADCINTSOCSEL1.all = 0; // ADC interrupt doesn't trigger any SOC
    AdcbRegs.ADCINTSOCSEL2.all = 0; // ADC interrupt doesn't trigger any SOC

    AdcbRegs.ADCSOC0CTL.bit.TRIGSEL = 5; // SOC-0 EPWM7 SOCA
    AdcbRegs.ADCSOC0CTL.bit.CHSEL = 2; // SOC-0 convert channel 2, which is ADC_B2 pin
    AdcbRegs.ADCSOC0CTL.bit.ACQPS = 19; // Sample window is 49+1 clock cycles

    AdcbRegs.ADCSOC1CTL.bit.TRIGSEL = 5; // SOC-1 EPWM7 SOCA
    AdcbRegs.ADCSOC1CTL.bit.CHSEL = 3; // SOC-1 convert channel 3, which is ADC_B3 pin
    AdcbRegs.ADCSOC1CTL.bit.ACQPS = 19; // Sample window is 49+1 clock cycles

    AdcbRegs.ADCSOC2CTL.bit.TRIGSEL = 5; // SOC-2 EPWM7 SOCA
    AdcbRegs.ADCSOC2CTL.bit.CHSEL = 4; // SOC-2 convert channel 4, which is ADC_B4 pin
    AdcbRegs.ADCSOC2CTL.bit.ACQPS = 19; // Sample window is 49+1 clock cycles

    AdcbRegs.ADCSOC3CTL.bit.TRIGSEL = 5; // SOC-3 EPWM7 SOCA
    AdcbRegs.ADCSOC3CTL.bit.CHSEL = 5; // SOC-3 convert channel 5, which is ADC_B5 pin
    AdcbRegs.ADCSOC3CTL.bit.ACQPS = 19; // Sample window is 49+1 clock cycles

    AdcbRegs.ADCSOC4CTL.bit.TRIGSEL = 5; // SOC-4 EPWM7 SOCA
    AdcbRegs.ADCSOC4CTL.bit.CHSEL = 14; // SOC-3 convert channel 14, which is ADC_14 pin
    AdcbRegs.ADCSOC4CTL.bit.ACQPS = 19; // Sample window is 49+1 clock cycles

    AdcbRegs.ADCSOC5CTL.bit.TRIGSEL = 5; // SOC-5 EPWM7 SOCA
    AdcbRegs.ADCSOC5CTL.bit.CHSEL = 15; // SOC-3 convert channel 15, which is ADC_15 pin
    AdcbRegs.ADCSOC5CTL.bit.ACQPS = 19; // Sample window is 49+1 clock cycles

    AdcbRegs.ADCCTL1.bit.INTPULSEPOS = 1;    // ADCINT1 trips after AdcResults latch
    AdcbRegs.ADCINTSEL1N2.bit.INT1E     = 1;    // Enabled ADCINT1
    AdcbRegs.ADCINTSEL1N2.bit.INT1CONT  = 0;    // Disable ADCINT1 Continuous mode
    AdcbRegs.ADCINTSEL1N2.bit.INT1SEL   = 5;    // setup EOC3 to trigger ADCINT1 to fire

    EDIS;
}

// Feedback signals
Uint16 Prd = 0;
Uint16 Duty = 0;
float Iac = 0;
float I_ac_offset = 50.9928255516;
float Vdc = 0;
float Idc = 0;
float Vac = 0;

float Iac_ref_m = 1;
float L_Angle = 0;

int Modu_1 = 0;
int Modu_2 = 0;

// Controller status
Uint16 Status = 0;
Uint16 Sec_En = 0;

// Controller outputs
float RectMi = 0;

// Controller state
Uint16 State = 0;

#define STATE_INITIAL     0
#define STATE_STAND_BY    1
#define STATE_DAB_SS      2
#define STATE_DAB_NORMAL  3
#define STATE_RECT_NORMAL 4
#define STATE_FAULT       5

void pri_inital();
void standby();
void dab_ss();
void dab_normal();
void rect_normal();
void protection();

Uint16 led_count = 0;

#define FAULT_RELEASE   GpioDataRegs.GPASET.bit.GPIO18 = 1
#define FAULT_TRIP      GpioDataRegs.GPACLEAR.bit.GPIO18 = 1

#pragma CODE_SECTION(ControlLoop, ".TI.ramfunc");
__interrupt void ControlLoop(void)
{
    V_DC = AdcbResultRegs.ADCRESULT1;
//    I_DC_1 = AdcbResultRegs.ADCRESULT2;
    I_DC_2 = AdcbResultRegs.ADCRESULT0;
    I_AC_1 = AdcbResultRegs.ADCRESULT4;
    I_AC_2 = AdcbResultRegs.ADCRESULT5;
    LIGHT = AdcbResultRegs.ADCRESULT3;
    V_AC = AdcbResultRegs.ADCRESULT2; //New ADC results by Hall.

    TEMP_1 = AdcaResultRegs.ADCRESULT0;
    TEMP_2 = AdcaResultRegs.ADCRESULT1;
    TEMP_3 = AdcaResultRegs.ADCRESULT2;
    TEMP_4 = AdcaResultRegs.ADCRESULT3;

    Prd = (ECap1Regs.CAP2 + ECap1Regs.CAP4) >> 1;
    Duty = (ECap1Regs.CAP1 + ECap1Regs.CAP3) >> 1;
    Iac = -0.0247676798 * I_AC_1 + I_ac_offset;
    Vdc = 0.2965626611 * V_DC + 0.0219840284;
    Idc = -0.0076936010 * I_DC_2 + 15.3691515548;

#ifdef L_con
    L_Angle += TS * omega;
    if(L_Angle>=6.2832)
        L_Angle-=6.2832;
    Duty = LcControl(L_Angle,Iac_ref_m,Iac);

//    Modu_1 =(int) 2000 + 2000*0.9*__sinpuf32(L_Angle);
//    Modu_2 =(int) 2000 - 2000*0.9*__sinpuf32(L_Angle);

//    EPwm7Regs.CMPA.bit.CMPA = Modu_1;
//    EPwm8Regs.CMPA.bit.CMPA = Modu_2;
    rect_normal();
    //Chb_Vac_ref = ChbControl(Vac, Vdc, Iac);
#endif

#ifdef protection
    if((State > 0) && ((Iac < -15)||(Iac > 15)))
        Status |= (1 << 1);

    if((State > 0) && (Vdc > 900))
        Status |= (1 << 2);

    if((State > 0) && ((Idc < -20)||(Idc > 20)))
        Status |= (1 << 3);

    if((State > 0) && (!((Duty > 50) && (Duty < 3950))))
        Status |= (1 << 4);

    if(Status)
    {
        Status |= 1;
        State = STATE_FAULT;
    }

    switch(State)
    {
    case STATE_INITIAL: pri_inital();       break;
    case STATE_STAND_BY: standby();         break;
    case STATE_DAB_SS: dab_ss();            break;
    case STATE_DAB_NORMAL: dab_normal();    break;
    case STATE_RECT_NORMAL: rect_normal();  break;
    case STATE_FAULT: protection();         break;
    default: protection();                  break;
    }

    if(Sec_En)
        EPwm9Regs.CMPA.bit.CMPA = V_DC + 50;
    else
        EPwm9Regs.CMPA.bit.CMPA = 40;
#endif

    led_count++;
    if(led_count>=20000)
    {
        led_count = 0;
        CPU_LED_TOGGLE = 1;
    }

    AdcbRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;	//Clear ADCINT1 flag reinitialize for next SOC
	PieCtrlRegs.PIEACK.bit.ACK1 = 1;

}

#ifdef DAB
Uint16 initial_count = 10;

void pri_inital()
{
    Pwm_DIS();
    Sec_En = 0;
    if(initial_count > 0)
        initial_count--;
    else
        State = STATE_STAND_BY;
}

float dab_ss_db = 1980;

void standby()
{
    Pwm_DIS();
    Sec_En = 0;

    if(Duty < 50)
    {
        State = STATE_STAND_BY;
    }
    else
    {
        dab_ss_db = 1980;
        DabPri_EN();
        State = STATE_DAB_SS;
    }
}

void dab_ss()
{
    Uint16 dab_db = (Uint16)dab_ss_db - 1;
    EPwm7Regs.DBRED.bit.DBRED = dab_db;
    EPwm7Regs.DBFED.bit.DBFED = dab_db;
    EPwm8Regs.DBRED.bit.DBRED = dab_db;
    EPwm8Regs.DBFED.bit.DBFED = dab_db;
    dab_ss_db -= 1;

    if(dab_ss_db <= 40)
    {
        Sec_En = 1;
        FAULT_RELEASE;
        State = STATE_DAB_NORMAL;
    }
}
void dab_normal()
{
    if(Duty > 100)
    {
        RectCmp_SET(Duty - 100);
        Rectifier_EN();
        State = STATE_RECT_NORMAL;
    }
}
void protection()
{
    Pwm_DIS();
    Sec_En = 0;
    FAULT_TRIP;
}

#endif
void rect_normal()
{
    RectCmp_SET(Duty - 100);
}
