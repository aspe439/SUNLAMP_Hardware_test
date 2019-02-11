/*
 * MSST_PLL.c
 *
 *  Created on: Jan 28, 2018
 *      Author: Yang Lei
*/

#define TS 2e-5
float theta = 0;

float omega_h1 = 60;
float omega_h3 = 180;
float omega_h5 = 300;
float omega_h7 = 420;

float sogi_h0_x  = 0;
float sogi_h1_x1 = 0;
float sogi_h1_x2 = 0;
float sogi_h3_x1 = 0;
float sogi_h3_x2 = 0;
float sogi_h5_x1 = 0;
float sogi_h5_x2 = 0;
float sogi_h7_x1 = 0;
float sogi_h7_x2 = 0;

float sogi_error = 0;
float sogi_k = 1.414;

float fll_inte = 0;
float gamma = 5e-5;

float v_q_inte = 0;
float pll_kp = 10;
float pll_ki = 200;

float delay = 0.022;

float v_q;

#pragma CODE_SECTION(SinglePhasePLL, ".TI.ramfunc");
float SinglePhasePLL(float Vac, float *Freq, float *Vac_amp)
{
    float omega_h1_T = omega_h1 * TS;
    float omega_h3_T = omega_h3 * TS;
    float omega_h5_T = omega_h5 * TS;
    float omega_h7_T = omega_h7 * TS;

    float sogi_h1_x1_n;
    float sogi_h1_x2_n;
    float sogi_h3_x1_n;
    float sogi_h3_x2_n;
    float sogi_h5_x1_n;
    float sogi_h5_x2_n;
    float sogi_h7_x1_n;
    float sogi_h7_x2_n;

    sogi_error = sogi_k * (Vac - sogi_h0_x - sogi_h1_x1 - sogi_h3_x1 - sogi_h5_x1 - sogi_h7_x1);

    sogi_h0_x += TS * 50 * sogi_error;

    sogi_h1_x1_n = __cospuf32(omega_h1_T) * sogi_h1_x1 - __sinpuf32(omega_h1_T) * sogi_h1_x2 + TS * 377 * sogi_error;
    sogi_h1_x2_n = __sinpuf32(omega_h1_T) * sogi_h1_x1 + __cospuf32(omega_h1_T) * sogi_h1_x2;

    sogi_h3_x1_n = __cospuf32(omega_h3_T) * sogi_h3_x1 - __sinpuf32(omega_h3_T) * sogi_h3_x2 + TS * 377 * 3 * sogi_error;
    sogi_h3_x2_n = __sinpuf32(omega_h3_T) * sogi_h3_x1 + __cospuf32(omega_h3_T) * sogi_h3_x2;

    sogi_h5_x1_n = __cospuf32(omega_h5_T) * sogi_h5_x1 - __sinpuf32(omega_h5_T) * sogi_h5_x2 + TS * 377 * 5 * sogi_error;
    sogi_h5_x2_n = __sinpuf32(omega_h5_T) * sogi_h5_x1 + __cospuf32(omega_h5_T) * sogi_h5_x2;

    sogi_h7_x1_n = __cospuf32(omega_h7_T) * sogi_h7_x1 - __sinpuf32(omega_h7_T) * sogi_h7_x2 + TS * 377 * 7 * sogi_error;
    sogi_h7_x2_n = __sinpuf32(omega_h7_T) * sogi_h7_x1 + __cospuf32(omega_h7_T) * sogi_h7_x2;

    sogi_h1_x1 = sogi_h1_x1_n;
    sogi_h1_x2 = sogi_h1_x2_n;
    sogi_h3_x1 = sogi_h3_x1_n;
    sogi_h3_x2 = sogi_h3_x2_n;
    sogi_h5_x1 = sogi_h5_x1_n;
    sogi_h5_x2 = sogi_h5_x2_n;
    sogi_h7_x1 = sogi_h7_x1_n;
    sogi_h7_x2 = sogi_h7_x2_n;

    float fll_inte_n,fll_inte_in;

    fll_inte_in = -TS * sogi_error * sogi_h1_x2 * omega_h1 * gamma;
    fll_inte_n = fll_inte + fll_inte_in;
    fll_inte = fll_inte_n;

    omega_h1 = 60 + fll_inte;
    omega_h3 = omega_h1 * 3;
    omega_h5 = omega_h1 * 5;
    omega_h7 = omega_h1 * 7;

    v_q = __cospuf32(theta) * sogi_h1_x2 - __sinpuf32(theta) * sogi_h1_x1;

    v_q_inte += TS * v_q;

    float omega_pll = pll_kp * v_q + pll_ki * v_q_inte + 60;

    theta += TS * omega_pll;


//    *Freq = omega_h1 * 0.159154943091895;

    theta = theta - (int)(theta);

    *Freq = omega_h1;

    *Vac_amp = __sqrt(sogi_h1_x1 * sogi_h1_x1 + sogi_h1_x2 * sogi_h1_x2);


    return (theta + delay - (int)(theta + delay));
}

