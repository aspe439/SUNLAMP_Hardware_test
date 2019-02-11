/*
 * Launch_GlobalFunctions.h
 *
 *  Created on: Apr 25, 2017
 *      Author: y3437
 */

#ifndef MSST_GLOBALFUNCTIONS_H_
#define MSST_GLOBALFUNCTIONS_H_

#include "F2837xS_Device.h"

#define CPU_LED_BIT GpioDataRegs.GPCDAT.bit.GPIO70
#define CPU_LED_TOGGLE GpioDataRegs.GPCTOGGLE.bit.GPIO70
//
//#define CPU_LED_BIT GpioDataRegs.GPBDAT.bit.GPIO41
//#define CPU_LED_TOGGLE GpioDataRegs.GPBTOGGLE.bit.GPIO41

extern void MSSTGpioConfig(void);

extern void InterruptInit(void);

extern void AdcInit(void);

extern Uint16 V_DC;
extern Uint16 I_DC_1;
extern Uint16 I_DC_2;
extern Uint16 I_AC_1;
extern Uint16 I_AC_2;
extern Uint16 LIGHT;

extern void V_dc_ref_set(float arg_2);
extern void V_dc_ref_inc();
extern void V_dc_ref_dec();
extern void Q_ref_set(float arg_2);

extern void I_loop_PR(float arg_2,float arg_3);
extern void I_offset(float arg_2);

extern void Rectifier_Start();
extern void Rectifier_Stop();


extern void Logging_length(int16 arg_1);
extern void current_loop_pr(float arg_2, float arg_3);
extern void v_sogi_k_p(float arg_2, float arg_3);
extern void v_loop_pi(float arg_2, float arg_3);

extern void DabPI(float arg_2, float arg_3);
extern void DabIref(float arg_2);
extern void DabCtrlEn();
extern void DabCtrlDis();


#endif /* MSST_GLOBALFUNCTIONS_H_ */
