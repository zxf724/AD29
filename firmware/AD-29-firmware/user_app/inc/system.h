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
 * @Date: 2019-02-27 10:34:16
 * @Last Modified by: liangjia
 * @Last Modified time: 2019-02-27 10:34:38
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _SYSTEM_H
#define _SYSTEM_H

/* Includes ------------------------------------------------------------------*/
#include "prjlib.h"

/* Exported define -----------------------------------------------------------*/

#define LOG_SAVE_BLOCK_SIZE 64

/* Exported types ------------------------------------------------------------*/
/*�������������ṹ*/
typedef struct {
  uint16_t nrst;
  uint16_t iwdg;
  uint16_t soft;
  uint16_t por;
} ResetCount_t;

/*��־�洢�鶨��*/
typedef struct {
  uint16_t crc;
  ResetCount_t ResetCount;
  uint32_t tsPwrOn;
  uint32_t tsFault;
  char FaultTask[36];
} LogBlock_t;

/* Exported constants --------------------------------------------------------*/
/*������־�洢�ĵ�ַ*/
#define LOG_ADDR() (SECTOR_ADDR(LOG_SAVE_SECTOR))
#define LOG_ADDR_MAX() (SECTOR_ADDR(LOG_SAVE_SECTOR + SECTOR_LOG_SAVE_SIZE - 1))
#define LOG_TASK_ADDR() (SECTOR_ADDR(LOG_SAVE_SECTOR + SECTOR_LOG_SAVE_SIZE))

/* Exported macro ------------------------------------------------------------*/
/*IO����*/
#define IO_READ(x) ((x##_GPIO_Port->IDR & x##_Pin) ? 1 : 0)
#define IO_H(x) (x##_GPIO_Port->BSRR = x##_Pin)
#define IO_L(x) (x##_GPIO_Port->BSRR = (uint32_t)x##_Pin << 16)
#define IO_TOGGLE(x) (x##_GPIO_Port->ODR ^= x##_Pin)
#define IO_WRITE(x, state) \
  (HAL_GPIO_WritePin(x##_GPIO_Port, x##_Pin, (GPIO_PinState)state))
#define IO_IS_OUT(x) (x##_GPIO_Port->ODR & x##_Pin)

#define GPIO_H(port, pin) (port->BSRR = pin)
#define GPIO_L(port, pin) (port->BSRR = (uint32_t)pin << 16)

#define LED_ON(x) IO_H(LED_##x)
#define LED_OFF(x) IO_L(LED_##x)
#define LED_TOGGLE(x) IO_TOGGLE(LED_##x)
#define LED_WRITE(x, state) IO_WRITE(LED_##x, state)
#define LED_IS_ON(x) IO_IS_OUT(LED_##x)

#define LED_ALL_ON() \
  do {               \
    LED_ON(ERR);     \
    LED_ON(RUN);     \
    LED_ON(NET);     \
  } while (0)
#define LED_ALL_OFF() \
  do {                \
    LED_OFF(ERR);     \
    LED_OFF(RUN);     \
    LED_OFF(NET);     \
  } while (0)

/*TS*/
#define TS_INIT(ts)     \
  do {                  \
    ts = HAL_GetTick(); \
  } while (0)
#define TS_IS_OVER(ts, over) (HAL_GetTick() - ts >= over)
#define TS_COUNT(ts) (HAL_GetTick() - ts)

/*�ڴ����*/
#define MMEMORY_ALLOC pvPortMalloc
#define MMEMORY_FREE vPortFree

/*���ж�*/
#define MCPU_ENTER_CRITICAL() portENTER_CRITICAL()
#define MCPU_EXIT_CRITICAL() portEXIT_CRITICAL()
#define MCPU_IS_ISR() (__get_IPSR() != 0)

/* Exported variables --------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
void System_Init(void);
void Read_ResetCount(ResetCount_t *count);
void Clear_ResetCount(void);
uint32_t Read_MCU_ID(void);

void StartLog_Recoder(void);
void TaskFault_Save(uint32_t tsFault, char *faultTask);
BOOL SaveLog_GetBlock(uint32_t addr, LogBlock_t *block);

#endif
