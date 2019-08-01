#include "analysis_protocol.h"
#include "ananlysis_data.h"
#include "delay.h"
#include "fifo.h"
#include "key.h"
#include "led.h"
#include "moto.h"
#include "prjlib.h"
#include "rtc.h"
#include "schedule.h"
#include "sys.h"
#include "timer.h"
#include "usart.h"
#include "wdg.h"

#define HEAR_BEAT_TIME (500 * 90)

static void funControl(int argc, char *argv[]);
extern Moto motoDef;
extern mPin Pin_Array[PINMAX];
int time = 0;
extern _calendar_obj calendar;  //时钟结构体
extern uint8_t g_array_ML[32];
extern uint8_t flag_finish;

int main(void) {
  CMD_ENT_DEF(MOTO, funControl);
  Cmd_AddEntrance(CMD_ENT(MOTO));
  delay_init();
  NVIC_PriorityGroupConfig(
      NVIC_PriorityGroup_2);  //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
  uart1_init(115200);
  uart2_init(115200);
  Moto_Init();
  TIM4_Int_Init(500, 7199);  // 10Khz的计数频率，计数到5000为500ms
  TIM2_Int_Init(20, 7199);   // 10Khz的计数频率，计数到5000为500ms
  sound_control();
  CLOSE_ELECTRIC_LOCK;
  // DBG_LOG("init_moto");
  init_moto();
  // TIM3_Int_Init(HEAR_BEAT_TIME, 7199);  // 10Khz的计数频率，计数到5000为500ms

  delay_ms_whx(1000);
  //
  // DBG_LOG("system start");
  if (RTC_Init())
    // DBG_LOG("RTC Init fail");
    IWDG_Init(6, 1024);  //与分频数为64,重载值为625,溢出时间为1s

  while (1) {
    IWDG_Feed();
    Gun_CommandReceive_Poll();
    Screen_CommandReceive_Poll();
    Start_Schedule();
    open_all_door();
    led_light();
    wait_fun();
    test_fun();
  }
}

static void funControl(int argc, char *argv[]) {
  uint8_t i = 0;
  argv++;
  argc--;
  if (ARGV_EQUAL("ALL_OPEN")) {
    for (i = 1; i < 57; i++) {
      motoDef.open_moto(i);
    }
  } else if (ARGV_EQUAL("ALL_CLOSE")) {
    for (i = 0; i < 57; i++) {
      motoDef.close_moto(i);
    }
  } else if (ARGV_EQUAL("RUN")) {
    motoDef.open_moto(uatoi(argv[1]));
  } else if (ARGV_EQUAL("CLOSE")) {
    motoDef.close_moto(uatoi(argv[1]));
  } else if (ARGV_EQUAL("motor_turn_off")) {
    PUSH_MOTOR(LEFT);
  }
}

void led_light(void) {
  RTC_Get();
  if (calendar.hour <= 6) {
    // led turns off
    GPIO_ResetBits(GPIOB, GPIO_Pin_8);
    GPIO_ResetBits(GPIOB, GPIO_Pin_9);
  } else {
    // led turns on
    // DBG_LOG("calendar.hour = %d",calendar.hour);
    GPIO_SetBits(GPIOB, GPIO_Pin_8);
    GPIO_SetBits(GPIOB, GPIO_Pin_9);
  }
}

void sound_control(void) {
  // sound control
  GPIO_SetBits(GPIOE, GPIO_Pin_1);
}

void wait_fun(void) {
  for (uint8_t i = 0; i <= 31; i++) {
    if ((g_array_ML[i] > 0) && (flag_finish == 1)) {
      motoDef.num = g_array_ML[i];
      // DBG_LOG("g_array_ML[%d] = %d, motoDef.num = %d", i, g_array_ML[i],
      // motoDef.num);
      g_array_ML[i] = 0;
      flag_finish = 0;
    }
  }
}

void test_fun() {
#if 1
  static uint8_t flag = 0;
  GPIO_SetBits(GPIOC, GPIO_Pin_10);  // EN1
  GPIO_SetBits(GPIOC, GPIO_Pin_12);  // EN2
  //
  GPIO_ResetBits(
      GPIOD,
      GPIO_Pin_0);  // DIR   GPIO_SetBits() -> out  GPIO_ResetBits() -> in
  GPIO_ResetBits(
      GPIOC,
      GPIO_Pin_11);  // DIR   GPIO_SetBits() -> out  GPIO_ResetBits() -> in
  if (flag == 0) {
    flag = 1;
    MotorSetpperMove(160000);  // 160000
    // MicroStep_Motro(400);
  }

#endif
  // if (TOUR_SWITCH == 0) {  // NEW_SENSOR  TOUR_SWITCH
  //   DBG_LOG("hello,world!");
  // }
  // if (TOUR_SWITCH == 1) {
  //   DBG_LOG("777777777")
  // }
}
