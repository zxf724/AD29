#ifndef __LED_H
#define __LED_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
// ALIENTEKս��STM32������
// LED��������
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/2
//�汾��V1.0
//��Ȩ���У�����ؾ���
// Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
// All rights reserved
//////////////////////////////////////////////////////////////////////////////////
#define LED0 PBout(5)  // PB5
#define LED1 PEout(5)  // PE5

#define LED_ON      (GPIO_ResetBits(GPIOF, GPIO_Pin_13))
#define LED_OFF     (GPIO_SetBits(GPIOF, GPIO_Pin_13))
#define LED_TOGGLE  (GPIO_WriteBit(GPIOF,GPIO_Pin_13,(BitAction)((1-GPIO_ReadOutputDataBit(GPIOF,GPIO_Pin_13)))))

void led_light_times(uint8_t num);

void LED_Init(void);  //��ʼ��
void led_light(void);

#endif
