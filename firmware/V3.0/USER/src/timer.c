#include "timer.h"
#include "ananlysis_data.h"
#include "led.h"
#include "moto.h"
#include "prjlib.h"
#include "rtc.h"
#include "usart.h"
#include "analysis_protocol.h"

extern _calendar_obj calendar;
extern void timer0_backcall_func(void);
extern Moto motoDef;
extern uint8_t flag_stop;
extern uint8_t close_800mm_moto;
extern uint8_t close_3min_cargo;
extern uint8_t flag_finish;

timer_t timerlist[TIMER_LIST_MAX];
uint32_t timer3_tick = 0, timer4_tick = 0, timer2_tick = 0;
uint8_t flag_hart = 0;
uint8_t flag_new_sensor = 0;
uint16_t calc_times = 0;
uint8_t flag_calc_times = 0;
uint16_t delay_time = 400;

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
    // heart beat data 
    flag_hart = 1;
    close_800mm_moto++;
    close_3min_cargo++;
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
    if (NORCH_SENSOR_A_MOTO == 0) {
      calc_times++;
    }
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
    if (calc_times >= 50) {
      flag_calc_times = 1;
      calc_times = 0;
    } else {
      calc_times = 0;
    }
    led_light();
    if(delay_time>=300) delay_time-=4;
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
