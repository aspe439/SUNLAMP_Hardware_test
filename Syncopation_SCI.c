/*
 * YSerialPort.c
 *
 *  Created on: May 5, 2018
 *      Author: Yang Lei
 */

#include "F2837xS_Device.h"
#include "MSST_GlobalFunctions.h"
#include "MSST_Pwm.h"

///////////////////////////////////////////////////
// Choose channel here

// Channel C (Fiber optic)
//#define RX_ISR  PieVectTable.SCIC_RX_INT
//#define RX_ACK  PieCtrlRegs.PIEACK.bit.ACK8
//#define RX_IEN  PieCtrlRegs.PIEIER8.bit.INTx5
//volatile struct SCI_REGS *sci_ch = &ScicRegs;

// Channel B (USB)
#define RX_ISR  PieVectTable.SCIB_RX_INT
#define RX_ACK  PieCtrlRegs.PIEACK.bit.ACK9
#define RX_IEN  PieCtrlRegs.PIEIER9.bit.INTx3
volatile struct SCI_REGS *sci_ch = &ScibRegs;
///////////////////////////////////////////////////

#define PACKET_INT16_NUM 8
#define PACKET_FLOAT_NUM 4
#define PACKET_BYTE_SIZE (PACKET_INT16_NUM + 2*PACKET_FLOAT_NUM)

//extern Uint16 DataLog_state;

void SCI_SerialPortReceiveISR(void);
Uint16 SCI_SerialPort_TxBusy();

struct DataPacket {
    int16_t data_int16[8];
    float data_float[4];
};

union PacketAssembly
{
    struct DataPacket payload;
    uint16_t word[PACKET_BYTE_SIZE];
} Packet;

struct CmdPacket {
    uint16_t header;
    uint16_t cmd_word;
    float arg_2;
    float arg_3;
    int16_t arg_1;
    uint16_t checksum;
};
union CmdAssembly
{
    struct CmdPacket packet;
    uint16_t word[8];
    char bytes[16];
} CmdPacket;


void SCI_Config(void)
{
    EALLOW;
    RX_ISR = &SCI_SerialPortReceiveISR;
    RX_IEN = 1;
    EDIS;

    sci_ch->SCICCR.all = 0x0007;
    sci_ch->SCICTL1.all = 0x0003;
    sci_ch->SCIHBAUD.all = 0x0000;
    sci_ch->SCILBAUD.all = 24;

    sci_ch->SCIFFTX.all = 0x4040;
    sci_ch->SCIFFRX.all = 0x4070;
    sci_ch->SCIFFTX.all = 0xE040;
    sci_ch->SCIFFRX.all = 0x6070;
    sci_ch->SCICTL1.all = 0x0023;
}

#pragma CODE_SECTION(SendByte, ".TI.ramfunc");
void SendByte(char byte)
{
    while(!(sci_ch->SCIFFTX.bit.TXFFST < 16));
    sci_ch->SCITXBUF.all = byte;
}

#pragma CODE_SECTION(SCI_SendPacket, ".TI.ramfunc");
void SCI_SendPacket(void)
{
    uint16_t writeCount = 0;
    SendByte(85); // Header_1
    SendByte(170); // Header_2
    while(writeCount < PACKET_BYTE_SIZE){
        SendByte(Packet.word[writeCount]);
        SendByte((Packet.word[writeCount])>>8);
        writeCount++;
    }
    SendByte(0xf0); // Tail
    SendByte(0xf0); // Tail
}

#pragma CODE_SECTION(SCI_UpdatePacketInt16, ".TI.ramfunc");
void SCI_UpdatePacketInt16(uint16_t index, int16_t data)
{
    if(index < PACKET_INT16_NUM)
        Packet.payload.data_int16[index] = data;
}

#pragma CODE_SECTION(SCI_UpdatePacketFloat, ".TI.ramfunc");
void SCI_UpdatePacketFloat(uint16_t index, float data)
{
    if(index < PACKET_FLOAT_NUM)
        Packet.payload.data_float[index] = data;
}

//////////////////////////////////////////////////////////////////////////
// Receive message macro definitions.
//////////////////////////////////////////////////////////////////////////
#define PWM_DIS         0x10
#define PWM_EN          0x11
#define REC_DIS         0x12
#define REC_EN          0x13
#define DAB_PRI_DIS     0x14
#define DAB_PRI_EN      0x15

#define DAB_FREQ_INC    0x20
#define DAB_FREQ_DEC    0x21
#define DAB_FREQ_SET    0x22

#define DAB_PHS_INC     0x28
#define DAB_PHS_DEC     0x29
#define DAB_PHS_SET     0x2A
#define DAB_PI          0x2B
#define DAB_IREF        0x2C
#define DAB_CTRL_EN     0x2D
#define DAB_CTRL_DIS    0x2E

#define LOGGING_TRIG    0x40
#define LOGGING_LENGTH  0x41
#define I_LOOP_PR       0x42
#define V_SOGI_K_P      0x43
#define V_LOOP_PI       0x44
//////////////////////////////////////////////////////////////////////////
// End of receive message macro definitions.
//////////////////////////////////////////////////////////////////////////

#pragma CODE_SECTION(SCI_SerialPortReceiveISR, ".TI.ramfunc");
__interrupt void SCI_SerialPortReceiveISR(void)
{
    if(sci_ch->SCIFFRX.bit.RXFFOVF)
    {
        sci_ch->SCIFFRX.bit.RXFFOVRCLR = 1;
        sci_ch->SCIFFRX.bit.RXFIFORESET = 0;
        sci_ch->SCIFFRX.bit.RXFIFORESET = 1;
        sci_ch->SCIFFRX.bit.RXFFINTCLR = 1;
        RX_ACK = 1;
        return;
    }

    int i;
    char read_buffer[16];
    for(i=0;i<16;i++)
        read_buffer[i] = (char)sci_ch->SCIRXBUF.all;
    for(i=0;i<8;i++)
        CmdPacket.word[i] = (read_buffer[2*i+1] << 8) | read_buffer[2*i];

    uint16_t checksum = 0;
    for(i=0;i<7;i++)
        checksum += CmdPacket.word[i];

    if(checksum != CmdPacket.packet.checksum)
    {
        sci_ch->SCIFFRX.bit.RXFFOVRCLR = 1;
        sci_ch->SCIFFRX.bit.RXFIFORESET = 0;
        sci_ch->SCIFFRX.bit.RXFIFORESET = 1;
        sci_ch->SCIFFRX.bit.RXFFINTCLR = 1;
        RX_ACK = 1;
        return;
    }

    uint16_t cmd = CmdPacket.packet.cmd_word;
    int16_t arg_1 = CmdPacket.packet.arg_1;
    float arg_2 = CmdPacket.packet.arg_2;
    float arg_3 = CmdPacket.packet.arg_3;



    switch(cmd) {
//    case 'a':       GpioDataRegs.GPDTOGGLE.bit.GPIO110 = 1;          break;
//    case PWM_EN:        Pwm_EN();           break;
//    case REC_DIS:       Rectifier_Stop();   break;
//    case REC_EN:        Rectifier_Start();  break;
//    case LOGGING_LENGTH:    Logging_length(arg_1);      break;
//    case I_LOOP_PR:     current_loop_pr(arg_2, arg_3);  break;
//    case V_SOGI_K_P:    v_sogi_k_p(arg_2, arg_3);       break;
//    case V_LOOP_PI:     v_loop_pi(arg_2, arg_3);        break;
//    case DAB_PRI_DIS:   DabPri_DIS();       break;
//    case DAB_PRI_EN:    DabPri_EN();        break;
//    case DAB_FREQ_INC:  DabFreq_INC();      break;
//    case DAB_FREQ_DEC:  DabFreq_DEC();      break;
//    case DAB_FREQ_SET:  DabFreq_SET(arg_2); break;
//    case DAB_PHS_INC:   DabPhs_INC();       break;
//    case DAB_PHS_DEC:   DabPhs_DEC();       break;
//    case DAB_PHS_SET:   DabPhs_SET(arg_1);  break;
//    case DAB_PI:        DabPI(arg_2,arg_3); break;
//    case DAB_IREF:      DabIref(arg_2);     break;
//    case DAB_CTRL_EN:   DabCtrlEn();        break;
//    case DAB_CTRL_DIS:  DabCtrlDis();       break;
//    case LOGGING_TRIG:  DataLog_state = 1;  break;
    default: break;
    }



    sci_ch->SCIFFRX.bit.RXFFINTCLR = 1;
    RX_ACK = 1;
}

Uint16 SCI_SerialPort_TxBusy()
{
    return sci_ch->SCIFFTX.bit.TXFFST;
}
