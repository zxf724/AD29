/**
 * **********************************************************************
 *             Copyright (c) 2016 temp. All Rights Reserved.
 * @file Process.h
 * @author � ?? * @version V1.0
 * @date 2016.8.31
 * @brief � ?�逻�?� ? ?�� �头? �?
 *
 * **********************************************************************
 * @note
 *
 * **********************************************************************
 */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _PROCESS_H
#define _PROCESS_H


/* Includes ------------------------------------------------------------------*/
#include "prjlib.h"
#include "cjson.h"

/* Exported define -----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/
extern char subscribeTopic[];
extern char publishTopic[];

/* Exported macro ------------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

void Process_Init(void);
BOOL CMD_Updata(char* cmd, cJSON* desired);
void GeneralResponse(char* ack_cmd, char* ret, char* msgid);
void ArrivePath(uint8_t* dat, uint16_t len);


#endif
