#ifndef __USART_H
#define __USART_H
#include "stdio.h"
#include "sys.h"

#define DBGLOG 1

#if DBGLOG
#define DBG_LOG(format, ...) printf("> " format "\r\n", ##__VA_ARGS__);
#endif

#define F2TIME_PARA 12000000  //将频率值转换为定时器寄存器值得转换参数
#define STEP_PARA 10  //任意时刻转动步数修正因子
#define STEP_AA 31    //加加速阶段，离散化点数
#define STEP_UA 31    //匀加速阶段，离散化点数
#define STEP_RA 31    //减加速阶段，离散化点数

#define STEP_SPTA 20          // SPTA最大速度等级
#define MAXSPEED_SPTA 80000   // SPTA最大速度
#define ACCSPEED_SPTA 150000  // SPTA加速度

//如果想串口中断接收，请不要注释以下宏定义
void uart1_init(u32 bound);
void uart2_init(u32 bound);

void Gun_printf(char *fmt, ...);
void Screen_printf(char *fmt, ...);

void Uart_Send_Data(uint8_t usart, uint8_t *data, int len);
#endif
