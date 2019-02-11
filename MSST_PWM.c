/*
 * MSST_Pwm.c
 *
 *  Created on: Oct 18, 2016
 *      Author: Yang Lei
 *      EPwm2, EPwm6 and EPwm7,EPwm8 have been swapped.
 */

#include "F28x_Project.h"

#define PWM_PRD     2000
#define PWM_DB_INIT 1980
#define PWM_DB      40

#define DAB_PRD     2000
#define DAB_OFFSET  29

Uint16 dab_prd = PWM_PRD;
int16 dab_phs = 0;


void Epwm1Init()
{
    EPwm1Regs.TBPRD = DAB_PRD;

    EPwm1Regs.TBCTL.bit.SYNCOSEL = 1; // Sync when TBCTR = 0
    EPwm1Regs.TBCTL.bit.PHSEN = 1; // Enable phase shift
    EPwm1Regs.TBCTL.bit.HSPCLKDIV = 0; // TBCLK = EPWMCLK / (HSPCLKDIV x CLKDIV)

//    EPwm1Regs.TBCTL.bit.CTRMODE = 2;

    EPwm1Regs.ETSEL.bit.SOCASEL = 1; // SOCA at counter equals to 0
    EPwm1Regs.ETSEL.bit.SOCAEN = 1;  // Enable SOCA
    EPwm1Regs.ETPS.bit.SOCAPRD = 1;  // Pulse at every event
}

void Epwm2Init()
{
    EPwm7Regs.TBPRD = 3999;

    EPwm7Regs.TBCTL.bit.SYNCOSEL = 0; // Sync when TBCTR = 0
    EPwm7Regs.TBCTL.bit.PHSEN = 1; // Enable phase shift
    EPwm7Regs.TBCTL.bit.HSPCLKDIV = 0; // TBCLK = EPWMCLK / (HSPCLKDIV x CLKDIV)

    EPwm7Regs.AQCTLA.bit.CAU = 1; // Clear output A at TBCTR = CMPA
    EPwm7Regs.AQCTLA.bit.ZRO = 2; // Set output A at TBCTR = 0
    EPwm7Regs.DBRED.bit.DBRED = PWM_DB-1;
    EPwm7Regs.DBFED.bit.DBFED = PWM_DB-1;
    EPwm7Regs.DBCTL.bit.IN_MODE = 0;
    EPwm7Regs.DBCTL.bit.POLSEL = 2;

    EPwm7Regs.CMPA.bit.CMPA = 2000;

//    EPwm7Regs.TBCTL.bit.CTRMODE = 0; // Count-up mode, start the counter

    EALLOW;
    EPwm7Regs.TZCTL.bit.TZA = TZ_FORCE_LO;
    EPwm7Regs.TZCTL.bit.TZB = TZ_FORCE_LO;
    EPwm7Regs.TZFRC.bit.OST = 1;
    EDIS;
    EPwm7Regs.DBCTL.bit.OUT_MODE = 3;
}

void Epwm6Init()
{
    EPwm8Regs.TBPRD = 3999;

    EPwm8Regs.TBCTL.bit.SYNCOSEL = 0; // Sync when TBCTR = 0
    EPwm8Regs.TBCTL.bit.PHSEN = 1; // Enable phase shift
    EPwm8Regs.TBCTL.bit.HSPCLKDIV = 0; // TBCLK = EPWMCLK / (HSPCLKDIV x CLKDIV)

    EPwm8Regs.AQCTLA.bit.CAU = 1; // Clear output A at TBCTR = CMPA
    EPwm8Regs.AQCTLA.bit.ZRO = 2; // Set output A at TBCTR = 0
    EPwm8Regs.DBRED.bit.DBRED = PWM_DB-1;
    EPwm8Regs.DBFED.bit.DBFED = PWM_DB-1;
    EPwm8Regs.DBCTL.bit.IN_MODE = 0;
    EPwm8Regs.DBCTL.bit.POLSEL = 2;
    EPwm8Regs.DBCTL.bit.OUTSWAP = 3;

    EPwm8Regs.CMPA.bit.CMPA = 2000;

//    EPwm8Regs.TBCTL.bit.CTRMODE = 0; // Count-up mode, start the counter


    EALLOW;
    EPwm8Regs.TZCTL.bit.TZA = TZ_FORCE_LO;
    EPwm8Regs.TZCTL.bit.TZB = TZ_FORCE_LO;
    EPwm8Regs.TZFRC.bit.OST = 1;
    EDIS;
    EPwm8Regs.DBCTL.bit.OUT_MODE = 3;
}

void Epwm7Init()
{
    EPwm2Regs.TBPRD = PWM_PRD;

    EPwm2Regs.TBCTL.bit.SYNCOSEL = 0; // Sync when TBCTR = 0
    EPwm2Regs.TBCTL.bit.PHSEN = 1; // Enable phase shift
    EPwm2Regs.TBCTL.bit.HSPCLKDIV = 0; // TBCLK = EPWMCLK / (HSPCLKDIV x CLKDIV)

    EPwm2Regs.AQCTLA.bit.CAU = 2; // Clear output A at TBCTR = CMPA
    EPwm2Regs.AQCTLA.bit.CAD = 1; // Set output A at TBCTR = 0

    EPwm2Regs.DBRED.bit.DBRED = PWM_DB_INIT-1;
    EPwm2Regs.DBFED.bit.DBFED = PWM_DB_INIT-1;
    EPwm2Regs.DBCTL.bit.IN_MODE = 0;
    EPwm2Regs.DBCTL.bit.POLSEL = 2;

    EPwm2Regs.CMPA.bit.CMPA = 1000;

//    EPwm2Regs.TBCTL.bit.CTRMODE = 2; // Count-up mode, start the counter

    EALLOW;
    EPwm2Regs.TZCTL.bit.TZA = TZ_FORCE_LO;
    EPwm2Regs.TZCTL.bit.TZB = TZ_FORCE_LO;
    EPwm2Regs.TZFRC.bit.OST = 1;
    EDIS;
    EPwm2Regs.DBCTL.bit.OUT_MODE = 3;
}

void Epwm8Init()
{
    EPwm6Regs.TBPRD = PWM_PRD;

    EPwm6Regs.TBCTL.bit.SYNCOSEL = 0; // Sync when TBCTR = 0
    EPwm6Regs.TBCTL.bit.PHSEN = 1; // Enable phase shift
    EPwm6Regs.TBCTL.bit.HSPCLKDIV = 0; // TBCLK = EPwMCLK / (HSPCLKDIV x CLKDIV)

    EPwm6Regs.AQCTLA.bit.CAU = 2; // Clear output A at TBCTR = CMPA
    EPwm6Regs.AQCTLA.bit.CAD = 1; // Set output A at TBCTR = 0

    EPwm6Regs.DBRED.bit.DBRED = PWM_DB_INIT-1;
    EPwm6Regs.DBFED.bit.DBFED = PWM_DB_INIT-1;
    EPwm6Regs.DBCTL.bit.IN_MODE = 0;
    EPwm6Regs.DBCTL.bit.POLSEL = 2;
    EPwm6Regs.DBCTL.bit.OUTSWAP = 3;

    EPwm6Regs.CMPA.bit.CMPA = 1000;

//    EPwm6Regs.TBCTL.bit.CTRMODE = 2; // Up-down count mode, start the counter

    EALLOW;
    EPwm6Regs.TZCTL.bit.TZA = TZ_FORCE_LO;
    EPwm6Regs.TZCTL.bit.TZB = TZ_FORCE_LO;
    EPwm6Regs.TZFRC.bit.OST = 1;
    EDIS;
    EPwm6Regs.DBCTL.bit.OUT_MODE = 3;
}

void Epwm9Init()
{
    EPwm9Regs.TBPRD = 3999;

    EPwm9Regs.TBCTL.bit.SYNCOSEL = 0; // Sync when TBCTR = 0
    EPwm9Regs.TBCTL.bit.PHSEN = 1; // Enable phase shift
    EPwm9Regs.TBCTL.bit.HSPCLKDIV = 0; // TBCLK = EPWMCLK / (HSPCLKDIV x CLKDIV)

    EPwm9Regs.AQCTLA.bit.CAU = 1; // Clear output A at TBCTR = CMPA
    EPwm9Regs.AQCTLA.bit.ZRO = 2; // Set output A at TBCTR = 0

    EPwm9Regs.DBRED.bit.DBRED = 0;
    EPwm9Regs.DBFED.bit.DBFED = 0;
    EPwm9Regs.DBCTL.bit.IN_MODE = 0;
    EPwm9Regs.DBCTL.bit.POLSEL = 2;

    EPwm9Regs.CMPA.bit.CMPA = 30;
    EPwm9Regs.TBPHS.bit.TBPHS = DAB_OFFSET;

//    EPwm9Regs.TBCTL.bit.CTRMODE = 0; // Up-down count mode, start the counter

    EALLOW;
    EPwm9Regs.TZCTL.bit.TZA = TZ_FORCE_LO;
    EPwm9Regs.TZCTL.bit.TZB = TZ_FORCE_LO;
    EDIS;
    EPwm9Regs.DBCTL.bit.OUT_MODE = 3;
}


void ECapInit()
{
    EALLOW;
    InputXbarRegs.INPUT7SELECT = 17; // GPIO-17 to ECAP1
    ECap1Regs.ECCTL1.bit.CAPLDEN = 1;
    ECap1Regs.ECCTL1.bit.CTRRST4 = 1;
    ECap1Regs.ECCTL1.bit.CAP3POL = 1;
    ECap1Regs.ECCTL1.bit.CTRRST2 = 1;
    ECap1Regs.ECCTL1.bit.CAP1POL = 1;
    ECap1Regs.ECCTL2.bit.TSCTRSTOP = 1;
    EDIS;
}

void PwmInit()
{
    EALLOW;
    InputXbarRegs.INPUT5SELECT = 17;
    EDIS;
    ECapInit();
    Epwm1Init();
    Epwm2Init();
    Epwm6Init();
    Epwm7Init();
    Epwm8Init();
    Epwm9Init();

    EPwm1Regs.TBCTL.bit.CTRMODE = 2;
    EPwm7Regs.TBCTL.bit.CTRMODE = 0;
    EPwm8Regs.TBCTL.bit.CTRMODE = 0;
    EPwm2Regs.TBCTL.bit.CTRMODE = 2;
    EPwm6Regs.TBCTL.bit.CTRMODE = 2;
    EPwm9Regs.TBCTL.bit.CTRMODE = 0;

}

void Rectifier_EN()
{
    EALLOW;
    EPwm7Regs.TZCLR.bit.OST = 1;
    EPwm8Regs.TZCLR.bit.OST = 1;
    EDIS;
}

void Rectifier_DIS()
{
    EALLOW;
    EPwm7Regs.TZFRC.bit.OST = 1;
    EPwm8Regs.TZFRC.bit.OST = 1;
    EDIS;
}

void DabPri_EN()
{
    EALLOW;
    EPwm7Regs.TZCLR.bit.OST = 1;
    EPwm8Regs.TZCLR.bit.OST = 1;
    EDIS;
}

void DabPri_DIS()
{
    EALLOW;
    EPwm7Regs.TZFRC.bit.OST = 1;
    EPwm8Regs.TZFRC.bit.OST = 1;
    EDIS;
}

void Pwm_EN()
{
    EALLOW;
    EPwm2Regs.TZCLR.bit.OST = 1;
    EPwm6Regs.TZCLR.bit.OST = 1;
    EPwm7Regs.TZCLR.bit.OST = 1;
    EPwm8Regs.TZCLR.bit.OST = 1;
    EDIS;
}

void Pwm_DIS()
{
    EALLOW;
    EPwm2Regs.TZFRC.bit.OST = 1;
    EPwm6Regs.TZFRC.bit.OST = 1;
    EPwm7Regs.TZFRC.bit.OST = 1;
    EPwm8Regs.TZFRC.bit.OST = 1;
    EDIS;
}

void RectDuty_SET(float duty)
{
    if(duty > 0.98)
        duty = 0.98;
    if(duty < -0.98)
        duty = -0.98;
    Uint16 cmp_value = (Uint16)(2000 * (1 + duty));
    EPwm7Regs.CMPA.bit.CMPA = cmp_value;
    EPwm8Regs.CMPA.bit.CMPA = cmp_value;
}

void RectCmp_SET(Uint16 cmp)
{
    if(cmp > 3500)
        cmp = 3500;
    if(cmp < 500)
        cmp = 500;
    EPwm7Regs.CMPA.bit.CMPA = cmp;
    EPwm8Regs.CMPA.bit.CMPA = cmp;
}

