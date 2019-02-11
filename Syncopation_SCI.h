/*
 * YSerialPort.h
 *
 *  Created on: May 6, 2018
 *      Author: y3437
 */

#ifndef SYNCOPATION_SCI_H_
#define SYNCOPATION_SCI_H_

extern void SCI_Config(void);
extern void SendByte(char byte);
extern void SCI_SendPacket(void);
extern void SCI_UpdatePacketFloat(uint16_t index, float data);
extern void SCI_UpdatePacketInt16(uint16_t index, int16_t data);
extern Uint16 SCI_SerialPort_TxBusy();

#endif /* SYNCOPATION_SCI_H_ */
