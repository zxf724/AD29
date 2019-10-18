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

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);  //ʱ��ʹ��

  //��ʱ��TIM3��ʼ��
  TIM_TimeBaseStructure.TIM_Period =
      arr;  //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
  TIM_TimeBaseStructure.TIM_Prescaler =
      psc;  //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
  TIM_TimeBaseStructure.TIM_ClockDivision =
      TIM_CKD_DIV1;  //����ʱ�ӷָ�:TDTS = Tck_tim
  TIM_TimeBaseStructure.TIM_CounterMode =
      TIM_CounterMode_Up;  // TIM���ϼ���ģʽ
  TIM_TimeBaseInit(
      TIM3, &TIM_TimeBaseStructure);  //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

  TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);  //ʹ��ָ����TIM3�ж�,��������ж�

  //�ж����ȼ�NVIC����
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;            // TIM3�ж�
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;         //�����ȼ�3��
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  // IRQͨ����ʹ��
  NVIC_Init(&NVIC_InitStructure);                  //��ʼ��NVIC�Ĵ���

  TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx
}

//��ʱ��3�жϷ������
void TIM3_IRQHandler(void)  // TIM3�ж�
{
  if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
  {
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  //���TIMx�����жϱ�־
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

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);  //ʱ��ʹ��

  //��ʱ��TIM4��ʼ��
  TIM_TimeBaseStructure.TIM_Period =
      arr;  //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
  TIM_TimeBaseStructure.TIM_Prescaler =
      psc;  //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
  TIM_TimeBaseStructure.TIM_ClockDivision =
      TIM_CKD_DIV1;  //����ʱ�ӷָ�:TDTS = Tck_tim
  TIM_TimeBaseStructure.TIM_CounterMode =
      TIM_CounterMode_Up;  // TIM���ϼ���ģʽ
  TIM_TimeBaseInit(
      TIM4, &TIM_TimeBaseStructure);  //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

  TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);  //ʹ��ָ����TIM4�ж�,��������ж�

  //�ж����ȼ�NVIC����
  NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;            // TIM4�ж�
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;         //�����ȼ�3��
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  // IRQͨ����ʹ��
  NVIC_Init(&NVIC_InitStructure);                  //��ʼ��NVIC�Ĵ���

  TIM_Cmd(TIM4, ENABLE);  //ʹ��TIMx
}

//��ʱ��4�жϷ������
void TIM4_IRQHandler(void)  // TIM4�ж�
{
  if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)  //���TIM4�����жϷ������
  {
    TIM_ClearITPendingBit(TIM4, TIM_IT_Update);  //���TIMx�����жϱ�־
    if (NORCH_SENSOR_A_MOTO == 0) {
      calc_times++;
    }
  }
}

// TIM2
void TIM2_Int_Init(u16 arr, u16 psc) {
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);  //ʱ��ʹ��

  //��ʱ��TIM2��ʼ��
  TIM_TimeBaseStructure.TIM_Period =
      arr;  //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
  TIM_TimeBaseStructure.TIM_Prescaler =
      psc;  //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
  TIM_TimeBaseStructure.TIM_ClockDivision =
      TIM_CKD_DIV1;  //����ʱ�ӷָ�:TDTS = Tck_tim
  TIM_TimeBaseStructure.TIM_CounterMode =
      TIM_CounterMode_Up;  // TIM���ϼ���ģʽ
  TIM_TimeBaseInit(
      TIM2, &TIM_TimeBaseStructure);  //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);  //ʹ��ָ����TIM2�ж�,��������ж�

  //�ж����ȼ�NVIC����
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;            // TIM2�ж�
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;         //�����ȼ�3��
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  // IRQͨ����ʹ��
  NVIC_Init(&NVIC_InitStructure);                  //��ʼ��NVIC�Ĵ���

  TIM_Cmd(TIM2, ENABLE);  //ʹ��TIMx
}

//��ʱ��2�жϷ������
void TIM2_IRQHandler(void)  // TIM2�ж�
{
  if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //���TIM2�����жϷ������
  {
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  //���TIMx�����жϱ�־
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
