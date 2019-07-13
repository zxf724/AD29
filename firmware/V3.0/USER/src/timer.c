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

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能
	
	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器

	TIM_Cmd(TIM3, ENABLE);  //使能TIMx					 
}

//定时器3中断服务程序
void TIM3_IRQHandler(void)   //TIM3中断
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
	{
			TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx更新中断标志 
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
