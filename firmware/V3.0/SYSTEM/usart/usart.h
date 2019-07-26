#ifndef __USART_H
#define __USART_H
#include "stdio.h"
#include "sys.h"

#define DBGLOG 1

#if DBGLOG
#define DBG_LOG(format, ...) printf("> " format "\r\n", ##__VA_ARGS__);
#endif

#define F2TIME_PARA 12000000  //��Ƶ��ֵת��Ϊ��ʱ���Ĵ���ֵ��ת������
#define STEP_PARA 10  //����ʱ��ת��������������
#define STEP_AA 31    //�Ӽ��ٽ׶Σ���ɢ������
#define STEP_UA 31    //�ȼ��ٽ׶Σ���ɢ������
#define STEP_RA 31    //�����ٽ׶Σ���ɢ������

#define STEP_SPTA 20          // SPTA����ٶȵȼ�
#define MAXSPEED_SPTA 80000   // SPTA����ٶ�
#define ACCSPEED_SPTA 150000  // SPTA���ٶ�

//����봮���жϽ��գ��벻Ҫע�����º궨��
void uart1_init(u32 bound);
void uart2_init(u32 bound);

void Gun_printf(char *fmt, ...);
void Screen_printf(char *fmt, ...);

void Uart_Send_Data(uint8_t usart, uint8_t *data, int len);
#endif
