#include "timer.h"
#include "led.h"
#include "usart.h"	
#include "rtc.h" 	
#include "prjlib.h"
#include "moto.h"
#include "ananlysis_data.h"

#define HERAD	0xFF

extern _calendar_obj calendar;
extern void timer0_backcall_func(void);
extern Moto motoDef;

timer_t  timerlist[TIMER_LIST_MAX];
uint32_t timer3_tick =0;

void TIM3_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���

	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx					 
}

//��ʱ��3�жϷ������
void TIM3_IRQHandler(void)   //TIM3�ж�
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
	{
			TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //���TIMx�����жϱ�־ 
		  timer3_tick++;
      timer_task();
			// DBG_LOG("hello,world!");
			//heart beat data
			static uint8_t report_data[8] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08};
			Report_State(HERAD,report_data,sizeof(report_data));
		}
	} 

void timer_task()
{
	uint8_t i;
	for(i=0;i<TIMER_LIST_MAX ;i++)
	{
			if(timerlist[i].state==0)
				continue;
			else{
							timerlist[i].count++;
			}
			if(timerlist[i].count==timerlist[i].expire){
							timerlist[i].count = 0;
							timerlist[i].callback();
			}
	}
        
}


void timer_enable(uint8_t num,uint16_t  val,void (* timer_backcall_func) (void))
{
		timerlist[num].state  = 1;
		timerlist[num].expire =  val+timer3_tick;
		timerlist[num].callback = timer_backcall_func;
}


void timer_disable(uint8_t num)
{
    timerlist[num].state = 0;
	  timerlist[num].expire = 0;
}
