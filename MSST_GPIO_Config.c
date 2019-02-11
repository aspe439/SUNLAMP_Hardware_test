/*
 * MSST_GPIO_Config.c
 *
 *  Created on: Jul 18, 2017
 *      Author: Yang Lei
 */

#include "F2837xS_Device.h"

void MSSTGpioConfig(void)
{
    EALLOW;
    GpioCtrlRegs.GPAGMUX1.bit.GPIO2 = 0;    // PWM2A -- PWM1A
    GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 1;
    GpioCtrlRegs.GPAGMUX1.bit.GPIO3 = 0;    // PWM2B -- PWM1B
    GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 1;
    GpioCtrlRegs.GPAGMUX1.bit.GPIO10 = 0;   // PWM6A -- PWM2A
    GpioCtrlRegs.GPAMUX1.bit.GPIO10 = 1;
    GpioCtrlRegs.GPAGMUX1.bit.GPIO11 = 0;   // PWM6B -- PWM2B
    GpioCtrlRegs.GPAMUX1.bit.GPIO11 = 1;
    GpioCtrlRegs.GPAGMUX1.bit.GPIO12 = 0;   // PWM7A -- PWM3A
    GpioCtrlRegs.GPAMUX1.bit.GPIO12 = 1;
    GpioCtrlRegs.GPAGMUX1.bit.GPIO13 = 0;   // PWM7B -- PWM3B
    GpioCtrlRegs.GPAMUX1.bit.GPIO13 = 1;
    GpioCtrlRegs.GPAGMUX1.bit.GPIO14 = 0;   // PWM8A -- PWM4A
    GpioCtrlRegs.GPAMUX1.bit.GPIO14 = 1;
    GpioCtrlRegs.GPAGMUX1.bit.GPIO15 = 0;   // PWM8B -- PWM4B
    GpioCtrlRegs.GPAMUX1.bit.GPIO15 = 1;

//    //DAB Current Calibration output configuration
//    GpioCtrlRegs.GPADIR.bit.GPIO12 = 1;
//    GpioCtrlRegs.GPADIR.bit.GPIO13 = 1;
//    GpioCtrlRegs.GPADIR.bit.GPIO14 = 1;
//    GpioCtrlRegs.GPADIR.bit.GPIO15 = 1;
//    GpioDataRegs.GPASET.bit.GPIO12 = 1;
//    GpioDataRegs.GPASET.bit.GPIO13 = 1;
//    GpioDataRegs.GPASET.bit.GPIO14 = 1;
//    GpioDataRegs.GPASET.bit.GPIO15 = 1;


    GpioCtrlRegs.GPAGMUX2.bit.GPIO16 = 1;   // PWM9A -- Sync_output
    GpioCtrlRegs.GPAMUX2.bit.GPIO16 = 1;

    GpioCtrlRegs.GPADIR.bit.GPIO18 = 1;  // Fault feedback signal

//
//    GpioCtrlRegs.GPAGMUX2.bit.GPIO18 = 0;   // SCITXDB
//    GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 2;
    GpioCtrlRegs.GPAGMUX2.bit.GPIO19 = 0;   // SCIRXDB
    GpioCtrlRegs.GPAMUX2.bit.GPIO19 = 2;


    GpioCtrlRegs.GPAPUD.bit.GPIO20 = 1;     // Unused pin
    GpioCtrlRegs.GPAPUD.bit.GPIO21 = 1;     // Unused pin
    GpioCtrlRegs.GPAPUD.bit.GPIO22 = 1;     // Unused pin
    GpioCtrlRegs.GPAPUD.bit.GPIO23 = 1;     // Unused pin
    GpioCtrlRegs.GPAPUD.bit.GPIO24 = 1;     // Unused pin

    ////// SPI-A
    GpioCtrlRegs.GPBGMUX2.bit.GPIO58 = 3;   // SPI_SIMO_A
    GpioCtrlRegs.GPBMUX2.bit.GPIO58 = 3;
    GpioCtrlRegs.GPBGMUX2.bit.GPIO59 = 3;   // SPI_SOMI_A
    GpioCtrlRegs.GPBMUX2.bit.GPIO59 = 3;
    GpioCtrlRegs.GPBGMUX2.bit.GPIO60 = 3;   // SPI_CLK_A
    GpioCtrlRegs.GPBMUX2.bit.GPIO60 = 3;
    GpioCtrlRegs.GPBGMUX2.bit.GPIO61 = 3;   // SPI_STEn_A
    GpioCtrlRegs.GPBMUX2.bit.GPIO61 = 3;

    GpioCtrlRegs.GPBPUD.bit.GPIO58 = 0;
    GpioCtrlRegs.GPBPUD.bit.GPIO59 = 0;     // SPI_SOMI_A (Pull-up enabled)
    GpioCtrlRegs.GPBPUD.bit.GPIO60 = 0;
    GpioCtrlRegs.GPBPUD.bit.GPIO61 = 0;
    GpioCtrlRegs.GPBPUD.bit.GPIO62 = 1;     // SPI_INT (Pull-up enabled)
    GpioCtrlRegs.GPBDIR.bit.GPIO63 = 1;     // RF_CE (Output)
    //////

    GpioCtrlRegs.GPCPUD.bit.GPIO64 = 1;     // Unused pin
    GpioCtrlRegs.GPCPUD.bit.GPIO65 = 1;     // Unused pin
    GpioCtrlRegs.GPCPUD.bit.GPIO66 = 1;     // Unused pin
    GpioCtrlRegs.GPCPUD.bit.GPIO69 = 1;     // Unused pin

    GpioCtrlRegs.GPCDIR.bit.GPIO70 = 1;     // CPU_LED
//    GpioCtrlRegs.GPBDIR.bit.GPIO41 = 1;     // CPU_LED

    GpioCtrlRegs.GPCDIR.bit.GPIO72 = 1;     // CPLD_RST || Fault_out

    GpioCtrlRegs.GPCPUD.bit.GPIO73 = 1;     // Unused pin
    GpioCtrlRegs.GPCPUD.bit.GPIO78 = 1;     // Unused pin
    GpioCtrlRegs.GPCPUD.bit.GPIO84 = 1;     // Unused pin
    GpioCtrlRegs.GPCPUD.bit.GPIO86 = 1;     // Unused pin
    GpioCtrlRegs.GPCPUD.bit.GPIO87 = 1;     // Unused pin
    GpioCtrlRegs.GPCPUD.bit.GPIO89 = 1;     // Unused pin
    GpioCtrlRegs.GPCPUD.bit.GPIO90 = 1;     // Unused pin

    GpioCtrlRegs.GPCGMUX2.bit.GPIO91 = 1;   // SDAA
    GpioCtrlRegs.GPCMUX2.bit.GPIO91 = 2;
    GpioCtrlRegs.GPCGMUX2.bit.GPIO92 = 1;   // SCLA
    GpioCtrlRegs.GPCMUX2.bit.GPIO92 = 2;

    GpioCtrlRegs.GPDPUD.bit.GPIO99 = 1;     // Unused pin
    EDIS;
}
