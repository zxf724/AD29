#ifndef _ANALYSIS_PROTOCOL_H
#define _ANALYSIS_PROTOCOL_H

#include "stm32f10x.h"

#define COMMAND_MAX   512

/*帧头尾*/
#define FHEADER       0x7E
#define FEND          0x7E

/*下发协议命令*/
#define CMD_TIME      0x00
#define CMD_MOTO      0x01
#define CMD_LOCK      0x02
#define CMD_GUN       0x03
#define CMD_CARGO     0x84
#define CMD_BACK      0x85
#define CMD_BACK_SUCCESS  0x86
/*上传协议命令*/
#define CMD_RETIME    0x80
#define CMD_REMOTO    0x81
#define CMD_RELOCK    0x82
#define CMD_REGUN     0x83
#define CMD_RECARGO   0x04
#define CMD_REBACK    0x05
#define CMD_REBACK_SUCCESS  0x06


void Screen_CommandReceive_Poll(void);
void Gun_CommandReceive_Poll(void);

void Uart_Protocol_Cmd_Analy(uint8_t* CmdRecBuf,uint8_t length);
#endif

