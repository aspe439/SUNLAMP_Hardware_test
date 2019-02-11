/*
 * Syncopation_Data.c
 *
 *  Created on: Dec 13, 2018
 *      Author: y3437
 */

#include "F28x_Project.h"
#include "Syncopation_SCI.h"

#define DATA_LOG_SIZE 8000
// Data log size <= 8000

float Data_Ch1[DATA_LOG_SIZE];
float Data_Ch2[DATA_LOG_SIZE];
float Data_Ch3[DATA_LOG_SIZE];
float Data_Ch4[DATA_LOG_SIZE];

#pragma DATA_SECTION(Data_Ch1, "DataLogFile");
#pragma DATA_SECTION(Data_Ch2, "DataLogFile");
#pragma DATA_SECTION(Data_Ch3, "DataLogFile");
#pragma DATA_SECTION(Data_Ch4, "DataLogFile");

Uint16 DataLog_index = 0;
Uint16 DataLog_state = 0;

Uint16 DataLog_send_size = 0;

void DataLog_Logging(Uint16 index, float ch1, float ch2, float ch3, float ch4)
{
    if(index < DATA_LOG_SIZE)
    {
        Data_Ch1[index] = ch1;
        Data_Ch2[index] = ch2;
        Data_Ch3[index] = ch3;
        Data_Ch4[index] = ch4;
    }
}

extern int16 pwm_counter;
extern Uint16 rect_state;
extern Uint16 dab_state;

void DataLog_SendSample(Uint16 index)
{
    if(DataLog_index < DATA_LOG_SIZE)
    {
        SCI_UpdatePacketFloat(0, Data_Ch1[index]);
        SCI_UpdatePacketFloat(1, Data_Ch2[index]);
        SCI_UpdatePacketFloat(2, Data_Ch3[index]);
        SCI_UpdatePacketFloat(3, Data_Ch4[index]);

        SCI_UpdatePacketInt16(0, 1);
        int16 converter_state = (rect_state & 0xfffc) | ((dab_state & 0xfffc) << 2);
        SCI_UpdatePacketInt16(1, converter_state);
        SCI_SendPacket();
    }
}

void DataLog_StartToSend(Uint16 length)
{
    DataLog_send_size = length;
    DataLog_state = 1;
}

void DataLog_ISR()
{
//    if(DataLog_state == 1)
//    {
//        DataLog_SendSample();
//    }
}

Uint16 getDataLogSize()
{
    return DATA_LOG_SIZE;
}

Uint16 getDataLogState()
{
    return DataLog_state;
}

void DataLogReset()
{
    DataLog_state = 0;
}
