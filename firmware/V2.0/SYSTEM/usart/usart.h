#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 

#define DBG_LOG(format, ...)     printf("> "format"\r\n", ##__VA_ARGS__);

//如果想串口中断接收，请不要注释以下宏定义
void uart1_init(u32 bound);
void uart2_init(u32 bound);


void Gun_printf (char *fmt, ...);
void Screen_printf (char *fmt, ...);

void Uart_Send_Data(uint8_t usart,uint8_t* data,int len);
#endif


