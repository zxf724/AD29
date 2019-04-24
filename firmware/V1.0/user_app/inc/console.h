/*
 * This file is a part of AFUIOT, Which is a free software project, Developed by
 * ShenZhen AFU Smart Internet Technology Co.,Ltd(AFU company).
 * AFUIOT, The new start of IoT, get news on www.afuiot.com.
 *
 * Copyright © 2019 [copyright AFU company].
 * @license (MIT license)
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction,including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:The above copyright
 * notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * @Author: liangjia
 * @Date: 2019-02-27 10:22:15
 * @Last Modified by: liangjia
 * @Last Modified time: 2019-02-27 10:24:13
 */

#ifndef _CONSOLE_H
#define _CONSOLE_H

/* Includes ------------------------------------------------------------------*/
#include "prjlib.h"
#include "user_comm.h"

/* Exported define -----------------------------------------------------------*/
#define CMD_PIPE_MAX 8

#define CMD_UART_EN 1

#define CMD_UART_REFRESH_TICKS 1000

#define CMD_ECHO_DEF 1

#define CMD_PRINTF_BUFF_MAX 256
#define CMD_FIFO_BUF_SIZE 256

#define CMD_TASK_STK_SIZE 128
#define CMD_TASK_PRIO osPriorityNormal

/* Exported types ------------------------------------------------------------*/
typedef BOOL (*CMD_SendFun)(uint8_t *dat, uint16_t len);

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

void CMD_Task(void const *argument);
void CMD_Init(void);
void CMD_UART_Read_Poll(void);
void CMD_NewData(uint8_t pipe, uint8_t *dat, uint16_t len);
uint8_t CMD_Pipe_Register(CMD_SendFun fun);

void CMD_SendData(uint8_t *dat, uint16_t len);
uint16_t CMD_ReadData(uint8_t *dat, uint16_t len);
uint16_t CMD_DataSize(void);

void CMD_SetEchoEnable(BOOL en);
void CMD_SetDebugLevel(uint8_t level);

void CMD_Printf(char *format, ...);
void CMD_HEX_Print(uint8_t *dat, uint16_t len);

void CMD_Printf_Level(uint8_t level, char *format, ...);
void CMD_HEX_Print_Level(uint8_t level, uint8_t *dat, uint16_t len);

void CMD_Virtual(char *cmd);

#endif
