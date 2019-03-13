/*
 * This file is a part of AFUIOT, Which is a free software project, Developed by
 * ShenZhen AFU Smart Internet Technology Co.,Ltd(AFU company).
 * AFUIOT, The new start of IoT, get news on www.afuiot.com.
 *
 * Copyright ? 2019 [copyright AFU company].
 * @license (MIT license)
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
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
 * Version Logs:
 * Date         Author          Notes
 * 2019.2.8     Yang Song       first version.
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _CONTROL_H
#define _CONTROL_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "prjlib.h"
#include "user_comm.h"
/* Exported define -----------------------------------------------------------*/

// extern RTC_HandleTypeDef hrtc;

#define MOTORLOG GPIOA, GPIO_PIN_15

/******************** MOTOR ***************/
#define MOTOR1 GPIOE, GPIO_PIN_2

#define MOTOR2 GPIOE, GPIO_PIN_3

#define MOTOR3 GPIOE, GPIO_PIN_4

#define MOTOR4 GPIOE, GPIO_PIN_5

#define MOTOR5 GPIOE, GPIO_PIN_6

#define MOTOR6 GPIOC, GPIO_PIN_13

#define MOTOR7 GPIOF, GPIO_PIN_0

#define MOTOR8 GPIOF, GPIO_PIN_1

#define MOTOR9 GPIOF, GPIO_PIN_2

#define MOTOR10 GPIOF, GPIO_PIN_3

#define MOTOR11 GPIOF, GPIO_PIN_4

#define MOTOR12 GPIOF, GPIO_PIN_5

#define MOTOR13 GPIOF, GPIO_PIN_6

#define MOTOR14 GPIOF, GPIO_PIN_7

#define MOTOR15 GPIOF, GPIO_PIN_8

#define MOTOR16 GPIOF, GPIO_PIN_9

#define MOTOR17 GPIOF, GPIO_PIN_10

#define MOTOR18 GPIOC, GPIO_PIN_0

#define MOTOR19 GPIOC, GPIO_PIN_1

#define MOTOR20 GPIOC, GPIO_PIN_2

#define MOTOR21 GPIOC, GPIO_PIN_3

#define MOTOR22 GPIOA, GPIO_PIN_0

#define MOTOR23 GPIOA, GPIO_PIN_1

#define MOTOR24 GPIOA, GPIO_PIN_4

#define MOTOR25 GPIOA, GPIO_PIN_5

#define MOTOR26 GPIOA, GPIO_PIN_6

#define MOTOR27 GPIOA, GPIO_PIN_7

#define MOTOR28 GPIOC, GPIO_PIN_4

#define MOTOR29 GPIOC, GPIO_PIN_5

#define MOTOR30 GPIOB, GPIO_PIN_0

#define MOTOR31 GPIOB, GPIO_PIN_1

#define MOTOR32 GPIOB, GPIO_PIN_2

//电机控制
#define Motor_H(x) (HAL_GPIO_WritePin(x, (GPIO_PinState)SET))
#define Motor_L(x) (HAL_GPIO_WritePin(x, (GPIO_PinState)RESET))

/*********************Lock*******************/

#define LOCK1 GPIOF, GPIO_PIN_11
#define LOCKCHECK1 GPIOF, GPIO_PIN_13

#define LOCK2 GPIOF, GPIO_PIN_12
#define LOCKCHECK2 GPIOF, GPIO_PIN_14

#define LOCK3 GPIOF, GPIO_PIN_15
#define LOCKCHECK3 GPIOG, GPIO_PIN_1

#define LOCK4 GPIOG, GPIO_PIN_0
#define LOCKCHECK4 GPIOE, GPIO_PIN_7

#define LOCK5 GPIOE, GPIO_PIN_8
#define LOCKCHECK5 GPIOE, GPIO_PIN_10

#define LOCK6 GPIOE, GPIO_PIN_9
#define LOCKCHECK6 GPIOE, GPIO_PIN_11

#define LOCK7 GPIOE, GPIO_PIN_12
#define LOCKCHECK7 GPIOE, GPIO_PIN_14

#define LOCK8 GPIOE, GPIO_PIN_13
#define LOCKCHECK8 GPIOE, GPIO_PIN_15

#define LOCK9 GPIOB, GPIO_PIN_10
#define LOCKCHECK9 GPIOB, GPIO_PIN_12

#define LOCK10 GPIOB, GPIO_PIN_11
#define LOCKCHECK10 GPIOB, GPIO_PIN_13

#define LOCK11 GPIOB, GPIO_PIN_14
#define LOCKCHECK11 GPIOD, GPIO_PIN_8

#define LOCK12 GPIOB, GPIO_PIN_15
#define LOCKCHECK12 GPIOD, GPIO_PIN_9

#define LOCK13 GPIOD, GPIO_PIN_10
#define LOCKCHECK13 GPIOD, GPIO_PIN_12

#define LOCK14 GPIOD, GPIO_PIN_11
#define LOCKCHECK14 GPIOD, GPIO_PIN_13

#define LOCK15 GPIOD, GPIO_PIN_14
#define LOCKCHECK15 GPIOG, GPIO_PIN_2

#define LOCK16 GPIOD, GPIO_PIN_15
#define LOCKCHECK16 GPIOG, GPIO_PIN_3

#define LOCK17 GPIOG, GPIO_PIN_4
#define LOCKCHECK17 GPIOG, GPIO_PIN_6

#define LOCK18 GPIOG, GPIO_PIN_5
#define LOCKCHECK18 GPIOG, GPIO_PIN_7

#define LOCK19 GPIOG, GPIO_PIN_8
#define LOCKCHECK19 GPIOC, GPIO_PIN_7

#define LOCK20 GPIOC, GPIO_PIN_6
#define LOCKCHECK20 GPIOC, GPIO_PIN_8

#define LOCK21 GPIOC, GPIO_PIN_9
#define LOCKCHECK21 GPIOA, GPIO_PIN_11

#define LOCK22 GPIOA, GPIO_PIN_8
#define LOCKCHECK22 GPIOA, GPIO_PIN_12

#define LOCK_OPEN(x) (HAL_GPIO_WritePin(x, (GPIO_PinState)SET))
#define LOCK_CLOSE(x) (HAL_GPIO_WritePin(x, (GPIO_PinState)RESET))

#define LOCK_CHECK(x) while ((HAL_GPIO_ReadPin(x)))

/***************************Shipment  Motor*******************************/
#define SHIPMENTMOTOR GPIOD, GPIO_PIN_1
#define SMOTORCHECK GPIOD, GPIO_PIN_3

#define SLOCK GPIOD, GPIO_PIN_0
#define SLOCKCHECK GPIOD, GPIO_PIN_2

#define LED_EN    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, (GPIO_PinState)SET))
#define PWM_LED GPIOD
#define PWM_LED_PIN GPIO_PIN_4

#define LED1_EN    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6, (GPIO_PinState)SET))
#define PWM1_LED GPIOD
#define PWM1_LED_PIN GPIO_PIN_7


#define RSP_SIZE 20



/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* External variables --------------------------------------------------------*/
extern BOOL ScanStationStart, ScanChannelStart, ReaportIR, RFID_Reset;
/* External functions --------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
void Lamp_Band_Control();
void Lamp_Band_Control1();
void Lamp_PWM(uint8_t num);

void BarcodeScan_Cmd_Init();

void Return_Lock_Cmd_Init();
void Lock_Open(uint8_t name);

void Motor_Cmd_Init();
void Motor_Control(uint8_t num);
void Detection_CargoRoad();

void Pack_Explain(uint8_t *name, uint8_t *cbuf);

void ShipMent_Motor_Control();
void ShipMent_Door_Check();

void Unix_To_Time(unsigned int r);
uint8_t Time_To_Unix();

void Device_Respond(uint8_t num);

#endif
