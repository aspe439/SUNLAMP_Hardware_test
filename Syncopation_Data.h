/*
 * Syncopation_Data.h
 *
 *  Created on: Dec 13, 2018
 *      Author: y3437
 */

#ifndef SYNCOPATION_DATA_H_
#define SYNCOPATION_DATA_H_

extern void DataLog_Logging(Uint16 index, float ch1, float ch2, float ch3, float ch4);
extern void DataLog_StartToSend(Uint16 length);
extern void DataLog_SendSample(Uint16 index);
extern void DataLog_ISR();
extern Uint16 getDataLogSize();
extern Uint16 getDataLogState();
extern void DataLogReset();



#endif /* SYNCOPATION_DATA_H_ */
