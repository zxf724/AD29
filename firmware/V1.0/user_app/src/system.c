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
 * @Date: 2019-02-27 10:27:06 
 * @Last Modified by: liangjia
 * @Last Modified time: 2019-02-27 10:33:59
 */


/* Includes ------------------------------------------------------------------*/
#include "user_comm.h"

/* Private typedef -----------------------------------------------------------*/
typedef struct {
    char* addr;
    uint16_t port;
} socketParam_t;

/* Private define ------------------------------------------------------------*/

/* Private macros ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static socketParam_t volatile socketPar;

/* Private function prototypes -----------------------------------------------*/

static void ResetFlag_init(void);

static void system_Console(int argc, char* argv[]);

/* Exported functions --------------------------------------------------------*/

/**
 * @brief  System初始化
 * 
 */
void System_Init(void) {
    uint8_t i = 0;
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_RCC_BKP_CLK_ENABLE();
    HAL_PWR_EnableBkUpAccess();
    /*初始化重启计数*/
    ResetFlag_init();
    /*FLASH 读写保护与硬件看门狗使能*/
#if FLASH_WRP_EN == 1
    if (FLASH_WRP_STATUS() == 1) {
        FLASH_WRP_ENABLE();
        DBG_LOG("System will reset to enable flash write read protect.");
        i++;
    }
#else
    DBG_LOG("Firmwave not enable flash write read protect.");
#endif
#if IWDG_HW_EN == 1
    if (IWDG_HW_STATUS() == 1) {
        IWDW_HW_ENABLE();
        DBG_LOG("System will reset to enable hardware IWDG.");
        i++;
    }
#else
    DBG_LOG("Firmwave not enable hardware IWDG.");
#endif
    if (i > 0) {
        NVIC_SystemReset();
    }  
    CMD_ENT_DEF(system, system_Console);
    Cmd_AddEntrance(CMD_ENT(system));
    DBG_LOG("System Init.");
}





/**
 * @brief 读出复位计数值
 * 
 * @param count 读出的结构体
 */
void Read_ResetCount(ResetCount_t* count) {
    count->iwdg = IWDG_BKP;
    count->nrst = NRST_BKP;
    count->por = PORRST_BKP;
    count->soft = SWRST_BKP;
}

/**
 * @brief 清除复位计数器
 * 
 */
void Clear_ResetCount(void) {
    NRST_BKP = 0;
    IWDG_BKP = 0;
    SWRST_BKP = 0;
    PORRST_BKP = 0;
}

/**
 * @brief 获取芯片的唯一ID
 * 
 * @return uint32_t 返回唯一ID的值
 */
uint32_t Read_MCU_ID(void) {
    uint32_t* p = NULL;
#ifdef STM32F1
    p = (uint32_t*)(0x1ffff7e8);
#endif
#ifdef STM32F4
    p = (uint32_t*)(0x1ffff7a10);
#endif
    return (*p >> 1) + (*(p + 1) >> 2) + (*(p + 2) >> 3);
}

/**
 * @brief 开机时记录启动日志
 * 
 */
void StartLog_Recoder(void) {
#if START_LOG_EN > 0
    char timebuf[24];
    uint8_t buf[64];
    LogBlock_t  block;
    uint32_t addr, ts = 0;
    addr = WorkParam.StartLogAddr;
    if (addr == 0) {
        addr = LOG_ADDR();
    } else {
        addr = SaveLog_NextAddr(addr);
    }
    
    /*查询复位前是否有任务溢出*/
    SFlash_Read(LOG_TASK_ADDR(), (uint8_t*)buf, 64);
    ts = *(uint32_t*)buf;
    if (ts > 0 && ts < BIT32_MAX) {
        block.tsFault = ts;
        strcpy(block.FaultTask, (char*)&buf[4]);
        SFlash_EraseSectors(LOG_TASK_ADDR(), 1);
    } else {
        block.tsFault = 0;
        block.FaultTask[0] = 0;
    }
    block.tsPwrOn = RTC_ReadTick();
    Read_ResetCount(&(block.ResetCount));
    block.crc = CRC_16(0, (uint8_t*) & (block.ResetCount), sizeof(LogBlock_t) - 2);
    RTC_TickToStr(block.tsPwrOn, timebuf);
    DBG_LOG("System start: %s, IWDG:%d, NRST:%d, PORD:%d, SOFT:%d", timebuf,
            block.ResetCount.iwdg, block.ResetCount.nrst, block.ResetCount.por, block.ResetCount.soft);
    if (block.tsFault > 0) {
        RTC_TickToStr(block.tsFault, timebuf);
        DBG_LOG("Last Task:%s Fault, TS:%u", block.FaultTask, timebuf);
    }
    MCPU_ENTER_CRITICAL();
    WorkParam.StartLogAddr = addr;
    MCPU_EXIT_CRITICAL();
    WorkParam_Save();
    if (addr % SFLASH_SECTOR_SIZE == 0) {
        SFlash_EraseSectors(addr, 1);
    }
    SFlash_Write(addr, (uint8_t*)&block, sizeof(block));
#endif
}



/**
 * @brief 用于在任务看门狗溢出时记录任务的名称
 * 
 * @param tsFault 
 * @param faultTask 溢出的任务名称
 */
void TaskFault_Save(uint32_t tsFault, char* faultTask) {
#if START_LOG_EN > 0
    uint32_t addr = LOG_TASK_ADDR();
    SFlash_EraseSectors(addr, 1);
    SFlash_Write(addr, (uint8_t*)&tsFault, 4);
    SFlash_Write(addr + 4, (uint8_t*)faultTask, strlen(faultTask) + 1);
#endif
}


/**
 * @brief system socket数据接收的回调函数
 * 
 */
static void ResetFlag_init(void) {
    ResetCount_t count;
    Read_ResetCount(&count);
    if (__HAL_RCC_GET_FLAG(RCC_FLAG_IWDGRST)) {
        count.iwdg++;
    }
    if (__HAL_RCC_GET_FLAG(RCC_FLAG_SFTRST)) {
        count.soft++;
    }
    if (__HAL_RCC_GET_FLAG(RCC_FLAG_PORRST)) {
        count.por++;
    }
    if (__HAL_RCC_GET_FLAG(RCC_FLAG_PINRST)) {
        count.nrst++;
    }
    __HAL_RCC_CLEAR_RESET_FLAGS();
    IWDG_BKP = count.iwdg;
    NRST_BKP = count.nrst;
    PORRST_BKP = count.por;
    SWRST_BKP = count.soft;
}

/**
 * @brief system调试命令
 * 
 * @param argc 参数项数量
 * @param argv 参数列表
 */
static void system_Console(int argc, char* argv[]) {
    char* p = NULL;
    uint16_t d = 0;
    uint32_t ts1, ts2;
    ResetCount_t count;
    argv++;
    argc--;
    if (ARGV_EQUAL("reset")) {
        d = uatoi(argv[1]);
        DBG_LOG("System Will Reset %dms latter.", d);
        osDelay(d + 5);
        NVIC_SystemReset();
    } else if (ARGV_EQUAL("version")) {
        DBG_LOG("Project:%s, Firmware version:%s, Hardware version:%s",
                PROJECT,
                VERSION,
                VERSION_HARDWARE);
    } else if (ARGV_EQUAL("alloc")) {
        d = uatoi(argv[1]);
        p = MMEMORY_ALLOC(d);
        if (p != NULL) {
            MMEMORY_FREE(p);
            DBG_LOG("Memory alloc size:%d OK, address:0x%p", d, p);
        } else {
            DBG_LOG("Memory alloc size:%d Failed.", d);
        }
    } else if (ARGV_EQUAL("resetcount")) {
        Read_ResetCount(&count);
        DBG_LOG("System reset count, IWDG:%d, NRST:%d, PORD:%d, SOFT:%d",
                count.iwdg, count.nrst, count.por, count.soft);
    } else if (ARGV_EQUAL("resetcountclear")) {
        Clear_ResetCount();
        DBG_LOG("System reset count clear.");
    } else if (ARGV_EQUAL("heapfreesize")) {
        DBG_LOG("Heap Memory free size:%u", xPortGetFreeHeapSize());
    } else if (ARGV_EQUAL("mcuid")) {
        DBG_LOG("MCU ID:%#x", Read_MCU_ID());
    } else if (ARGV_EQUAL("startlog")) {
        if (argv[1] && argv[2]) {
            ts1 = uatoi(argv[1]);
            ts2 = uatoi(argv[2]);
            DBG_LOG("ts begin:%u, ts end:%u", ts1, ts2);
//            StartLog_UART(ts1, ts2);
        }
    }  
}