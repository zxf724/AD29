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

/* Includes ------------------------------------------------------------------*/
#include "Control.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
uint8_t g_unix_buf[10];
uint32_t g_unix_temp;
/* External variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void Motor_Console(int argc, char *argv[]);
static void BarcodeScan_Console(int argc, char *argv[]);
static void Return_Lock_Console(int argc, char *argv[]);

/* Exported functions --------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/****************************  Lamp_Band_Control ****************************/

/**
 * @brief 灯带翻转控制
 * 
 */
void Lamp_Band_Control() {
  Lamp_PWM(1);
}


/**
 * @brief  PWM输出翻转
 * 
 * @param num  灯带号
 */
void Lamp_PWM(uint8_t num) {
  if (num == 1) {
    //      HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_0);
    osDelay(1000);
  } else if (num == 2) {
    //      HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_0);
    osDelay(1000);
  }
}

/******************** Motor_Control ************************/

/**
 * @brief 货道电机控制初始化
 * 
 */
void Motor_Cmd_Init() {
  CMD_ENT_DEF(MOTOR, Motor_Console);
  Cmd_AddEntrance(CMD_ENT(MOTOR));
}

/**
 * @brief  货道电机控制台
 * 
 * @param argc 参数项
 * @param argv 参数列表
 */
static void Motor_Console(int argc, char *argv[]) {
  while (argc > 0 && *argv != NULL) {
    Motor_Control((*argv[1]));

    argv++;
    argc--;
  }
}

/**
 * @brief 电机控制函数
 * 
 * @param num 货道电机号
 */
void Motor_Control(uint8_t num) {
  DBG_LOG("Control");

  switch (num) {
    case 0x01:
      Motor_H(MOTOR1);
      //掉落检测
      Detection_CargoRoad();
      Motor_L(MOTOR1);
      break;
    case 0x02:
      Motor_H(MOTOR2);
       //掉落检测
      Detection_CargoRoad();
      Motor_L(MOTOR2);
      break;
    case 0x03:
      Motor_H(MOTOR3);
       //掉落检测
      Detection_CargoRoad();
      Motor_L(MOTOR3);
      break;
    case 0x04:
      Motor_H(MOTOR4);
       //掉落检测
      Detection_CargoRoad();
      Motor_L(MOTOR4);
      break;
    case 0x05:
      Motor_H(MOTOR5);
       //掉落检测
      Detection_CargoRoad();
      Motor_L(MOTOR5);
      break;
    case 0x06:
      Motor_H(MOTOR6);
       //掉落检测
      Detection_CargoRoad();
      Motor_L(MOTOR6);
      break;
    case 0x07:
      Motor_H(MOTOR7);
       //掉落检测
      Detection_CargoRoad();
      Motor_L(MOTOR7);
      break;
    case 0x08:
      Motor_H(MOTOR8);
       //掉落检测
      Detection_CargoRoad();
      Motor_L(MOTOR8);
      break;
    case 0x09:
      Motor_H(MOTOR9);
       //掉落检测
      Detection_CargoRoad();
      Motor_L(MOTOR9);
      break;
    case 0x10:
      Motor_H(MOTOR10);
       //掉落检测
      Detection_CargoRoad();
      Motor_L(MOTOR10);
      break;
    case 0x11:
      Motor_H(MOTOR11);
       //掉落检测
      Detection_CargoRoad();
      Motor_L(MOTOR11);
      break;
    case 0x12:
      Motor_H(MOTOR12);
       //掉落检测
      Detection_CargoRoad();
      Motor_L(MOTOR12);
      break;
    case 0x13:
      Motor_H(MOTOR13);
       //掉落检测
      Detection_CargoRoad();
      Motor_L(MOTOR13);
      break;
    case 0x14:
      Motor_H(MOTOR14);
       //掉落检测
      Detection_CargoRoad();
      Motor_L(MOTOR14);
      break;
    case 0x15:
      Motor_H(MOTOR15);
       //掉落检测
      Detection_CargoRoad();
      Motor_L(MOTOR15);
      break;
    case 0x16:
      Motor_H(MOTOR16);
       //掉落检测
      Detection_CargoRoad();
      Motor_L(MOTOR16);
      break;
    case 0x17:
      Motor_H(MOTOR17);
      //掉落检测
      Detection_CargoRoad();
      Motor_L(MOTOR17);
      break;
    case 0x18:
      Motor_H(MOTOR18);
       //掉落检测
      Detection_CargoRoad();
      Motor_L(MOTOR18);
      break;
    case 0x19:
      Motor_H(MOTOR19);
       //掉落检测
      Detection_CargoRoad();
      Motor_L(MOTOR19);
      break;
    case 0x20:
      Motor_H(MOTOR20);
       //掉落检测
      Detection_CargoRoad();
      Motor_L(MOTOR20);
      break;
    case 0x21:
      Motor_H(MOTOR21);
       //掉落检测
      Detection_CargoRoad();
      Motor_L(MOTOR21);
      break;
    case 0x22:
      Motor_H(MOTOR22);
       //掉落检测
      Detection_CargoRoad();
      Motor_L(MOTOR22);
      break;
    case 0x23:
      Motor_H(MOTOR23);
       //掉落检测
      Detection_CargoRoad();
      Motor_L(MOTOR23);
      break;
    case 0x24:
      Motor_H(MOTOR24);
       //掉落检测
      Detection_CargoRoad();
      Motor_L(MOTOR24);
      break;
    case 0x25:
      Motor_H(MOTOR25);
       //掉落检测
      Detection_CargoRoad();
      Motor_L(MOTOR25);
      break;
    case 0x26:
      Motor_H(MOTOR26);
       //掉落检测
      Detection_CargoRoad();
      Motor_L(MOTOR26);
      break;
    case 0x27:
      Motor_H(MOTOR27);
       //掉落检测
      Detection_CargoRoad();
      Motor_L(MOTOR27);
      break;
    case 0x28:
      Motor_H(MOTOR28);
       //掉落检测
      Detection_CargoRoad();
      Motor_L(MOTOR28);
      break;
    case 0x29:
      Motor_H(MOTOR29);
       //掉落检测
      Detection_CargoRoad();
      Motor_L(MOTOR29);
      break;
    case 0x30:
      Motor_H(MOTOR30);
       //掉落检测
      Detection_CargoRoad();
      Motor_L(MOTOR30);
      break;
    case 0x31:
      Motor_H(MOTOR31);
       //掉落检测
      Detection_CargoRoad();
      Motor_L(MOTOR31);
      break;
    case 0x32:
      Motor_H(MOTOR32);
       //掉落检测
      Detection_CargoRoad();
      Motor_L(MOTOR32);
      break;
    default:
      DBG_LOG("error :num \r\n");
      break;
  }

  //出货电机控制
  ShipMent_Motor_Control();

  //出货检测
  ShipMent_Door_Check();
}

/**
 * @brief 掉落检测
 * 
 */
void Detection_CargoRoad() {
  uint8_t flag = 1;

  while (flag) {
    if (HAL_GPIO_ReadPin(MOTORLOG)) {
      flag = 0;
    }
  }
}

/**
 * @brief 出货电机控制
 * 
 */
void ShipMent_Motor_Control() {
  uint8_t flag = 1;

  DBG_LOG("ShipMent_Motor_Control\n");
  Motor_H(SHIPMENTMOTOR);

  //出货检测
  while (flag) {
    if (HAL_GPIO_ReadPin(SMOTORCHECK)) {
      flag = 0;
    }
    DBG_LOG("GPIO:%d", HAL_GPIO_ReadPin(SMOTORCHECK));
  }

  Motor_L(SHIPMENTMOTOR);
}

/**
 * @brief 出货门检测
 * 
 */
void ShipMent_Door_Check() {
  DBG_LOG("ShipMent_Lock_Control\n");
  LOCK_OPEN(SLOCK);
  osDelay(100);
  LOCK_CLOSE(SLOCK);
  LOCK_CHECK(SLOCKCHECK);
  DBG_LOG("success !!!\n");
}

/***************************Return_Lock_Control********************************/

/**
 * @brief 归还舱门初始化
 * 
 */
void Return_Lock_Cmd_Init() {
  CMD_ENT_DEF(RLOCK, Return_Lock_Console);
  Cmd_AddEntrance(CMD_ENT(RLOCK));
}

/**
 * @brief 归还仓控制台
 * 
 * @param argc 参数项
 * @param argv 参数列表
 */
static void Return_Lock_Console(int argc, char *argv[]) {
  uint8_t Temp;
  while (argc > 0 && *argv != NULL) {
    Temp = argv[1][0];

    Lock_Open(Temp);

    argv++;
    argc--;
  }
}

/**
 * @brief 归还舱门开门
 * 
 * @param name 舱门号
 */
void Lock_Open(uint8_t name) {
  switch (name) {
    case 0x01:
      LOCK_OPEN(LOCK1);
      osDelay(100);
      LOCK_CLOSE(LOCK1);
      LOCK_CHECK(LOCKCHECK1);
      DBG_LOG(" open sucess\n");
      break;
    case 0x02:
      LOCK_OPEN(LOCK2);
      osDelay(100);
      LOCK_CLOSE(LOCK2);
      LOCK_CHECK(LOCKCHECK2);
      DBG_LOG(" open sucess\n");
      break;
    case 0x03:
      LOCK_OPEN(LOCK3);
      osDelay(100);
      LOCK_CLOSE(LOCK3);
      LOCK_CHECK(LOCKCHECK3);
      DBG_LOG(" open sucess\n");
      break;
    case 0x04:
      LOCK_OPEN(LOCK4);
      osDelay(100);
      LOCK_CLOSE(LOCK4);
      LOCK_CHECK(LOCKCHECK4);
      DBG_LOG(" open sucess\n");
      break;
    case 0x05:
      LOCK_OPEN(LOCK5);
      osDelay(100);
      LOCK_CLOSE(LOCK5);
      LOCK_CHECK(LOCKCHECK5);
      DBG_LOG(" open sucess\n");
      break;
    case 0x06:
      LOCK_OPEN(LOCK6);
      osDelay(100);
      LOCK_CLOSE(LOCK6);
      LOCK_CHECK(LOCKCHECK6);
      DBG_LOG(" open sucess\n");
      break;
    case 0x07:
      LOCK_OPEN(LOCK7);
      osDelay(100);
      LOCK_CLOSE(LOCK7);
      LOCK_CHECK(LOCKCHECK7);
      DBG_LOG(" open sucess\n");
      break;
    case 0x08:
      LOCK_OPEN(LOCK8);
      osDelay(100);
      LOCK_CLOSE(LOCK8);
      LOCK_CHECK(LOCKCHECK8);
      DBG_LOG(" open sucess\n");
      break;
    case 0x09:
      LOCK_OPEN(LOCK9);
      osDelay(100);
      LOCK_CLOSE(LOCK9);
      LOCK_CHECK(LOCKCHECK9);
      DBG_LOG(" open sucess\n");
      break;
    case 0x10:
      LOCK_OPEN(LOCK10);
      osDelay(100);
      LOCK_CLOSE(LOCK10);
      LOCK_CHECK(LOCKCHECK10);
      DBG_LOG(" open sucess\n");
      break;
    case 0x11:
      LOCK_OPEN(LOCK11);
      osDelay(100);
      LOCK_CLOSE(LOCK11);
      LOCK_CHECK(LOCKCHECK11);
      DBG_LOG(" open sucess\n");
      break;
    case 0x12:
      LOCK_OPEN(LOCK12);
      osDelay(100);
      LOCK_CLOSE(LOCK12);
      LOCK_CHECK(LOCKCHECK12);
      DBG_LOG(" open sucess\n");
      break;
    case 0x13:
      LOCK_OPEN(LOCK13);
      osDelay(100);
      LOCK_CLOSE(LOCK13);
      LOCK_CHECK(LOCKCHECK13);
      DBG_LOG(" open sucess\n");
      break;
    case 0x14:
      LOCK_OPEN(LOCK14);
      osDelay(100);
      LOCK_CLOSE(LOCK14);
      LOCK_CHECK(LOCKCHECK14);
      DBG_LOG(" open sucess\n");
      break;
    case 0x15:
      LOCK_OPEN(LOCK15);
      osDelay(100);
      LOCK_CLOSE(LOCK15);
      LOCK_CHECK(LOCKCHECK15);
      DBG_LOG(" open sucess\n");
      break;
    case 0x16:
      LOCK_OPEN(LOCK16);
      osDelay(100);
      LOCK_CLOSE(LOCK16);
      LOCK_CHECK(LOCKCHECK16);
      DBG_LOG(" open sucess\n");
      break;
    case 0x17:
      LOCK_OPEN(LOCK17);
      osDelay(100);
      LOCK_CLOSE(LOCK17);
      LOCK_CHECK(LOCKCHECK17);
     DBG_LOG(" open sucess\n");
      break;
    case 0x18:
      LOCK_OPEN(LOCK18);
      osDelay(100);
      LOCK_CLOSE(LOCK18);
      LOCK_CHECK(LOCKCHECK18);
      DBG_LOG(" open sucess\n");
      break;
    case 0x19:
      LOCK_OPEN(LOCK19);
      osDelay(100);
      LOCK_CLOSE(LOCK19);
      LOCK_CHECK(LOCKCHECK19);
      DBG_LOG(" open sucess\n");
      break;
    case 0x20:
      LOCK_OPEN(LOCK20);
      osDelay(100);
      LOCK_CLOSE(LOCK20);
      LOCK_CHECK(LOCKCHECK20);
      DBG_LOG(" open sucess\n");
      break;
    case 0x21:
      LOCK_OPEN(LOCK21);
      osDelay(100);
      LOCK_CLOSE(LOCK21);
      LOCK_CHECK(LOCKCHECK21);
      DBG_LOG(" open sucess\n");
      break;
    case 0x22:
      LOCK_OPEN(LOCK22);
      osDelay(100);
      LOCK_CLOSE(LOCK22);
      LOCK_CHECK(LOCKCHECK22);
      DBG_LOG(" open sucess\n");
      break;
    default:
      DBG_LOG("error :num \r\n");
      break;
  }
}

/******************** BarcodeScan_Control **********************/

/**
 * @brief  扫码模块初始化
 * 
 */

void BarcodeScan_Cmd_Init() {
  CMD_ENT_DEF(SCAN, BarcodeScan_Console);
  Cmd_AddEntrance(CMD_ENT(SCAN));
}

/**
 * @brief  扫码控制台
 * 
 * @param argc 
 * @param argv 
 */
static void BarcodeScan_Console(int argc, char *argv[]) {
  uint8_t *buf = NULL;
  uint8_t len = 0;

  while (argc > 0 && *argv != NULL) {
    if (strncmp(argv[1], "ON", 2) == 0) {
  
      osDelay(5000);
      len = UART_DataSize(2);

      DBG_LOG("len:%d", len);

      if (len > 0) {
        buf = MMEMORY_ALLOC(len + 1);
        if (buf != NULL) {
          UART_ReadData(2, buf, len);
          DBG_LOG("buf:%s\n", buf);
          buf[len] = 0;
          MMEMORY_FREE(buf);
        }
      }
    }
    argv++;
    argc--;
  }
}

/**
 * @brief 数据包解释与提取
 * 
 * @param name 
 * @param cbuf 
 */
void Pack_Explain(uint8_t *name, uint8_t *cbuf) {
  uint8_t *p;
  p = NULL;

  uint16_t crc, crc_check = 0;
  uint8_t i = 0, len = 0, CmdLen = 0;

  if (name != NULL) {
    p = name;
  } else {
    DBG_LOG("error data\n");
  }

  if (*p == 0x7E) {
    p++;
    len++;

    if ((*p == 0x00) || (*p == 0x01) || (*p == 0x02) || (*p == 0x03)) {
      //TODO：时间戳
      p = p + 5;

      len = len + 5;

      DBG_LOG("Pack len:%d \n", (*p));
      p++;
      len++;

      //提取数据包
      for (i = 0; i < 7; i++) {
        *cbuf = *p;
        cbuf++;
        p++;
        len++;
      }

      //CRC检测
      DBG_LOG("crclen:%d", len);
      crc = CRC_16(0x0000, name, len - 1);
      DBG_LOG("crc=%d \n", crc);

      crc_check = (*p);
      p++;
      crc_check = (crc_check << 8) | (*p);

      DBG_LOG("crc_check:%d \n ", crc_check);

      if (crc_check == crc) {
        DBG_LOG("CRC success");
      }

      p++;
      //֡数据尾
      if (*p == 0x7E) {
        DBG_LOG("Explain success\n");
        p++;
      }
    }
  }
}

/**
 * Description:Unixʱ������������ñ���ʱ��
 * Parameter:UnixNum  Unixʱ���
 * Return: NULL
 * Others:
 */
// void Unix_To_Time(uint32_t UnixNum)
//{
//	struct tm *stmU;
//
//        RTC_DateTypeDef sdate;
//        RTC_TimeTypeDef stime;
//
//        time_t Count;
//
//
//        Count = UnixNum;
//
//	stmU = localtime(&Count);
//	sdate.Year = stmU->tm_year - 100;
//	sdate.Month = stmU->tm_mon + 1;
//	sdate.Date = stmU->tm_mday;
//	stime.Hours = stmU->tm_hour + 8;
//	stime.Minutes = stmU->tm_min;
//	stime.Seconds = stmU->tm_sec;
//
//	HAL_RTC_SetDate(&hrtc,&sdate,RTC_FORMAT_BIN);
//	HAL_RTC_SetTime(&hrtc,&stime,RTC_FORMAT_BIN);
//
//       DBG_LOG("%02d/%02d/%02d\r\n",2000 + sdate.Year, sdate.Month,
//       sdate.Date); DBG_LOG("%02d:%02d:%02d\r\n",stime.Hours, stime.Minutes,
//       stime.Seconds); DBG_LOG("\r\n");
//
//}

///**
// * Description:����ʱ������Unixʱ���
// * Parameter:
// * Return:
// * Others:
// */
// uint8_t Time_To_Unix()
//{
//
//
//        struct tm stmT;
//
//
//	RTC_DateTypeDef sdate;
//        RTC_TimeTypeDef stime;
//
//        HAL_RTC_GetTime(&hrtc, &stime, RTC_FORMAT_BIN);
//        HAL_RTC_GetDate(&hrtc, &sdate, RTC_FORMAT_BIN);
//
//
//       stmT.tm_year=sdate.Year+100;
//       stmT.tm_mon=sdate.Month-1;
//       stmT.tm_mday=sdate.Date;
//       stmT.tm_hour=stime.Hours-8;
//       stmT.tm_min=stime.Minutes;
//       stmT.tm_sec=stime.Seconds;
//
//
//       g_unix_temp = mktime(&stmT);
//       DBG_LOG("g_unix_temp:%d",g_unix_temp );
//     // sDBG_LOG(g_unix_buf,"%u",mktime(&stmT));
//
//
//    //  return g_unix_temp;
//}

/**
 * @brief 数据应答
 * 
 * @param num 响应号
 */
void Device_Respond(uint8_t num) {
  uint8_t i = 0;
//  uint8_t Respond[20];
  uint8_t *Respond = NULL;


   Respond = MMEMORY_ALLOC(RSP_SIZE);
   
   if( Respond != NULL){
      *Respond = 0x7E;
       i++;
      if(*(Respond+1) == 0x80){
        //TODO :Unix时间戳信息返回---应答
      }else if(*(Respond+1) == 0x81){
       //TODO:
      }else if(*(Respond+1) == 0x82){
       //TODO： 
      }

   }




  for (i = 0; i < strlen(Respond); i++) {
    DBG_LOG("Respond :%x \n", Respond[i]);
  }
}
