/*
 * Launch_Interrupt.c
 *
 *  Created on: Apr 25, 2017
 *      Author: Yang Lei
 */

#include "F28x_Project.h"

#include "MSST_GlobalFunctions.h"

void XinterruptInit(void);

void InterruptInit(void)
{
    DINT;
    PieCtrlRegs.PIECTRL.bit.ENPIE = 0;

    PieCtrlRegs.PIEIER1.all = 0;
    PieCtrlRegs.PIEIER2.all = 0;
    PieCtrlRegs.PIEIER3.all = 0;
    PieCtrlRegs.PIEIER4.all = 0;
    PieCtrlRegs.PIEIER5.all = 0;
    PieCtrlRegs.PIEIER6.all = 0;
    PieCtrlRegs.PIEIER7.all = 0;
    PieCtrlRegs.PIEIER8.all = 0;
    PieCtrlRegs.PIEIER9.all = 0;
    PieCtrlRegs.PIEIER10.all = 0;
    PieCtrlRegs.PIEIER11.all = 0;
    PieCtrlRegs.PIEIER12.all = 0;

    PieCtrlRegs.PIEIFR1.all = 0;
    PieCtrlRegs.PIEIFR2.all = 0;
    PieCtrlRegs.PIEIFR3.all = 0;
    PieCtrlRegs.PIEIFR4.all = 0;
    PieCtrlRegs.PIEIFR5.all = 0;
    PieCtrlRegs.PIEIFR6.all = 0;
    PieCtrlRegs.PIEIFR7.all = 0;
    PieCtrlRegs.PIEIFR8.all = 0;
    PieCtrlRegs.PIEIFR9.all = 0;
    PieCtrlRegs.PIEIFR10.all = 0;
    PieCtrlRegs.PIEIFR11.all = 0;
    PieCtrlRegs.PIEIFR12.all = 0;

    PieCtrlRegs.PIEACK.all = 0xFFFF;
}

void XinterruptInit(void)
{
    XintRegs.XINT1CR.bit.POLARITY = 0;  // Negative edge trig
    XintRegs.XINT1CR.bit.ENABLE = 1;    // Enable Xint-1

    EALLOW;
    InputXbarRegs.INPUT14SELECT = 60;
    EDIS;
}
