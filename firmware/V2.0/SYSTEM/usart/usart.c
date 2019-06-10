/*
 * @Author: 黄施泓
 * @Description: 
 * @Version: V1.0
 * @Date: 2019-06-04 09:03:50
 * @LastEditors: OBKoro1
 * @LastEditTime: 2019-06-10 10:38:19
 * @locale: zh-CN
 */
#include "sys.h"
#include "usart.h"	 
#include <stdarg.h>
#include "fifo.h"
#include "usart.h"	 

extern app_fifo_t  rx_fifo_Gun_Def; 
extern app_fifo_t  rx_fifo_Screen_Def;

uint8_t gs_screen_rx_buff[1024] = {'\0'};
static uint8_t gs_gun_rx_buff[1024] = {'\0'};

void uart1_init(u32 bound){
  //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
  
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.9
   
  //USART1_RX	  GPIOA.10初始化
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.10

  //Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
   //USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

  USART_Init(USART1, &USART_InitStructure); //初始化串口1
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启串口接受中断
  USART_Cmd(USART1, ENABLE);                    //使能串口1 
  app_fifo_init(&rx_fifo_Screen_Def,gs_screen_rx_buff,sizeof(gs_screen_rx_buff));
  app_fifo_init(&rx_fifo_Gun_Def,gs_gun_rx_buff,sizeof(gs_gun_rx_buff));
}

void uart2_init(u32 bound)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA.2
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.9
   
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//PA3
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.10  

  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

  USART_Init(USART2, &USART_InitStructure); 
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
  USART_Cmd(USART2, ENABLE);


}


void USART1_IRQHandler(void)                	//串口1中断服务程序
{
	u8 Res;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		Res = USART_ReceiveData(USART1);	//读取接收到的数据
		app_uart_put(Res,SCREEN); 		 
  } 
} 


void USART2_IRQHandler(void)                	//串口2中断服务程序
{
	u8 Res;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		Res =USART_ReceiveData(USART2);	//读取接收到的数据
	  app_uart_put(Res,GUN); 
  }
} 
	
void Gun_printf (char *fmt, ...)
{
		char buffer[200] = {'\0'}; 
		u8 i = 0;
		
		va_list arg_ptr;
		va_start(arg_ptr, fmt);  
		vsnprintf(buffer, 200, fmt, arg_ptr);
		while ((i < 200) && buffer)
		{
				USART_SendData(USART2, (u8) buffer[i++]);
				while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET); 
		}
		va_end(arg_ptr);
}

int fputc(int ch, FILE *f)
{
	USART_SendData(USART1, (uint8_t) ch);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {}	
  return ch;
}

void Screen_printf (char *fmt, ...)
{
		char buffer[200] = {'\0'};  
		u8 i = 0;
		
		va_list arg_ptr;
		va_start(arg_ptr, fmt);  
		vsnprintf(buffer, 200, fmt, arg_ptr);
		while ((i < 200) && buffer)
		{
				USART_SendData(USART1, (u8) buffer[i++]);
				while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET); 
		}
		va_end(arg_ptr);
}

void Uart_Send_Data(uint8_t usart,uint8_t* data,int len)
{
	  int i = 0;
	  USART_TypeDef* USARTx = NULL;
	  if(usart == GUN)
		{
			USARTx = 	USART2;
		}else if(usart == SCREEN)
		{
			USARTx = 	USART1;
		}
		while ((i <= len))
		{
				USART_SendData(USARTx, (u8) data[i++]);
				while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET); 
		}
}
