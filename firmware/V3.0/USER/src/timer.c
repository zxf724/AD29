#include "timer.h"
#include "ananlysis_data.h"
#include "led.h"
#include "moto.h"
#include "prjlib.h"
#include "rtc.h"
#include "usart.h"

#define HERAD 0xFF

extern _calendar_obj calendar;
extern void timer0_backcall_func(void);
extern Moto motoDef;

timer_t timerlist[TIMER_LIST_MAX];
uint32_t timer3_tick = 0, timer4_tick = 0, timer2_tick = 0;

void TIM3_Int_Init(u16 arr, u16 psc) {
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);  //时钟使能

  //定时器TIM3初始化
  TIM_TimeBaseStructure.TIM_Period =
      arr;  //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
  TIM_TimeBaseStructure.TIM_Prescaler =
      psc;  //设置用来作为TIMx时钟频率除数的预分频值
  TIM_TimeBaseStructure.TIM_ClockDivision =
      TIM_CKD_DIV1;  //设置时钟分割:TDTS = Tck_tim
  TIM_TimeBaseStructure.TIM_CounterMode =
      TIM_CounterMode_Up;  // TIM向上计数模式
  TIM_TimeBaseInit(
      TIM3, &TIM_TimeBaseStructure);  //根据指定的参数初始化TIMx的时间基数单位

  TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);  //使能指定的TIM3中断,允许更新中断

  //中断优先级NVIC设置
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;            // TIM3中断
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;         //从优先级3级
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  // IRQ通道被使能
  NVIC_Init(&NVIC_InitStructure);                  //初始化NVIC寄存器

  TIM_Cmd(TIM3, ENABLE);  //使能TIMx
}

//定时器3中断服务程序
void TIM3_IRQHandler(void)  // TIM3中断
{
  if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
  {
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  //清除TIMx更新中断标志
    timer3_tick++;
    timer_task();
  }
}

void timer_task() {
  uint8_t i;
  for (i = 0; i < TIMER_LIST_MAX; i++) {
    if (timerlist[i].state == 0)
      continue;
    else {
      timerlist[i].count++;
    }
    if (timerlist[i].count == timerlist[i].expire) {
      timerlist[i].count = 0;
      timerlist[i].callback();
    }
  }
}

//  TIM4
void TIM4_Int_Init(u16 arr, u16 psc) {
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);  //时钟使能

  //定时器TIM4初始化
  TIM_TimeBaseStructure.TIM_Period =
      arr;  //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
  TIM_TimeBaseStructure.TIM_Prescaler =
      psc;  //设置用来作为TIMx时钟频率除数的预分频值
  TIM_TimeBaseStructure.TIM_ClockDivision =
      TIM_CKD_DIV1;  //设置时钟分割:TDTS = Tck_tim
  TIM_TimeBaseStructure.TIM_CounterMode =
      TIM_CounterMode_Up;  // TIM向上计数模式
  TIM_TimeBaseInit(
      TIM4, &TIM_TimeBaseStructure);  //根据指定的参数初始化TIMx的时间基数单位

  TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);  //使能指定的TIM4中断,允许更新中断

  //中断优先级NVIC设置
  NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;            // TIM4中断
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;         //从优先级3级
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  // IRQ通道被使能
  NVIC_Init(&NVIC_InitStructure);                  //初始化NVIC寄存器

  TIM_Cmd(TIM4, ENABLE);  //使能TIMx
}

//定时器4中断服务程序
void TIM4_IRQHandler(void)  // TIM4中断
{
  if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)  //检查TIM4更新中断发生与否
  {
    TIM_ClearITPendingBit(TIM4, TIM_IT_Update);  //清除TIMx更新中断标志
    timer4_tick++;
    timer_task();
    // heart beat data
    static uint8_t report_data[8] = {0x01, 0x02, 0x03, 0x04,
                                     0x05, 0x06, 0x07, 0x08};
    Report_State(HERAD, report_data, sizeof(report_data));
  }
}

// TIM2
void TIM2_Int_Init(u16 arr, u16 psc) {
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);  //时钟使能

  //定时器TIM2初始化
  TIM_TimeBaseStructure.TIM_Period =
      arr;  //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
  TIM_TimeBaseStructure.TIM_Prescaler =
      psc;  //设置用来作为TIMx时钟频率除数的预分频值
  TIM_TimeBaseStructure.TIM_ClockDivision =
      TIM_CKD_DIV1;  //设置时钟分割:TDTS = Tck_tim
  TIM_TimeBaseStructure.TIM_CounterMode =
      TIM_CounterMode_Up;  // TIM向上计数模式
  TIM_TimeBaseInit(
      TIM2, &TIM_TimeBaseStructure);  //根据指定的参数初始化TIMx的时间基数单位

  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);  //使能指定的TIM2中断,允许更新中断

  //中断优先级NVIC设置
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;            // TIM2中断
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;         //从优先级3级
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  // IRQ通道被使能
  NVIC_Init(&NVIC_InitStructure);                  //初始化NVIC寄存器

  TIM_Cmd(TIM2, ENABLE);  //使能TIMx
}

//定时器2中断服务程序
void TIM2_IRQHandler(void)  // TIM2中断
{
  if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //检查TIM2更新中断发生与否
  {
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  //清除TIMx更新中断标志
    timer2_tick++;
    timer_task();
  }
}

void timer_enable(uint8_t num, uint16_t val,
                  void (*timer_backcall_func)(void)) {
  timerlist[num].state = 1;
  timerlist[num].expire = val + timer3_tick;
  timerlist[num].callback = timer_backcall_func;
}

void timer_disable(uint8_t num) {
  timerlist[num].state = 0;
  timerlist[num].expire = 0;
}

/*电机1的PWM输出初始化，使用的是定时器4*/  //  PUL1- PB3 TIM2_CH2
void Initial_PWM_Motor1(void) {
  TIM_TimeBaseInitTypeDef TIM_BaseInitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  TIM_OCInitTypeDef TIM_OCInitStructure;

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);  //时钟使能

  TIM_DeInit(TIM2);
  //中断NVIC设置：允许中断，设置优先级
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =
      PWM1_PreemptionPriority;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = PWM1_SubPriority;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  TIM_BaseInitStructure.TIM_Period = 1000;
#ifdef OUTPUT_DATA
  TIM_BaseInitStructure.TIM_Prescaler = 710000;  // 71000
#else
  TIM_BaseInitStructure.TIM_Prescaler = 5;
#endif
  TIM_BaseInitStructure.TIM_ClockDivision = 0;
  TIM_BaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_BaseInitStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(TIM2, &TIM_BaseInitStructure);

  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;  // PWM2模式
  TIM_OCInitStructure.TIM_OutputState =
      TIM_OutputState_Enable;  //信号输出到对应的输出引脚
  TIM_OCInitStructure.TIM_OutputNState =
      TIM_OutputNState_Enable;  //互补信号输出到对应的输出引脚
  TIM_OCInitStructure.TIM_Pulse = 50;                       //脉冲宽度
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;  //互补输出高电平有效
  TIM_OCInitStructure.TIM_OCNPolarity =
      TIM_OCNPolarity_High;  //互补输出高电平有效
  TIM_OCInitStructure.TIM_OCIdleState =
      TIM_OCIdleState_Reset;  //输出空闲状态为1
  TIM_OCInitStructure.TIM_OCNIdleState =
      TIM_OCIdleState_Reset;                //互补输出空闲状态为0
  TIM_OC2Init(TIM2, &TIM_OCInitStructure);  // OC1通道初始化

  TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
  TIM_ARRPreloadConfig(TIM2, ENABLE);

  //清中断，以免一启用中断后立即产生中断
  TIM_ClearFlag(TIM2, TIM_FLAG_Update);
  //使能TIM1中断源
  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
  TIM_Cmd(TIM2, ENABLE);
  TIM_CtrlPWMOutputs(TIM2, ENABLE);  //使能PWM输出
}

// PUL2- PB4 TIM3_CH1
void Initial_PWM_Motor2(void) {
  TIM_TimeBaseInitTypeDef TIM_BaseInitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  TIM_OCInitTypeDef TIM_OCInitStructure;

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);  //时钟使能
  TIM_DeInit(TIM3);
  //中断NVIC设置：允许中断，设置优先级
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //更新事件 	TIM2_IRQHandler
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =
      PWM2_PreemptionPriority;  //抢占优先级3
  NVIC_InitStructure.NVIC_IRQChannelSubPriority =
      PWM2_SubPriority;                            //响应优先级1
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  //允许中断
  NVIC_Init(&NVIC_InitStructure);
  TIM_BaseInitStructure.TIM_Period = 1000;
#ifdef OUTPUT_DATA
  TIM_BaseInitStructure.TIM_Prescaler = 710000;  // 71000
#else
  TIM_BaseInitStructure.TIM_Prescaler = 5;
#endif
  TIM_BaseInitStructure.TIM_ClockDivision = 0;
  TIM_BaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_BaseInitStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(TIM3, &TIM_BaseInitStructure);

  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;  // PWM2模式
  TIM_OCInitStructure.TIM_OutputState =
      TIM_OutputState_Enable;  //信号输出到对应的输出引脚
  TIM_OCInitStructure.TIM_OutputNState =
      TIM_OutputNState_Enable;  //互补信号输出到对应的输出引脚
  TIM_OCInitStructure.TIM_Pulse = 50;                       //脉冲宽度
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;  //互补输出高电平有效
  TIM_OCInitStructure.TIM_OCNPolarity =
      TIM_OCNPolarity_High;  //互补输出高电平有效
  TIM_OCInitStructure.TIM_OCIdleState =
      TIM_OCIdleState_Reset;  //输出空闲状态为1
  TIM_OCInitStructure.TIM_OCNIdleState =
      TIM_OCIdleState_Reset;                //互补输出空闲状态为0
  TIM_OC1Init(TIM3, &TIM_OCInitStructure);  // OC1通道初始化

  TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
  TIM_ARRPreloadConfig(TIM3, ENABLE);
  //清中断，以免一启用中断后立即产生中断
  TIM_ClearFlag(TIM3, TIM_FLAG_Update);
  //使能TIM1中断源
  TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
  TIM_Cmd(TIM3, DISABLE);
  // TIM_CtrlPWMOutputs(TIM3,ENABLE); //使能PWM输出
}
