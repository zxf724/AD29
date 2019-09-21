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
#include "core_cm3.h"

#define HEAR_BEAT_TIME (500 * 90)

static void funControl(int argc, char *argv[]);
extern Moto motoDef;
extern mPin Pin_Array[PINMAX];
int time = 0;
extern _calendar_obj calendar;  //时钟结构体
extern uint8_t g_array_ML[32];
extern uint8_t flag_finish;
extern uint8_t flag_calc_times;

int main(void) {
  CMD_ENT_DEF(MOTO, funControl);
  Cmd_AddEntrance(CMD_ENT(MOTO));
  delay_init();
  NVIC_PriorityGroupConfig(
      NVIC_PriorityGroup_2);  //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
  uart1_init(115200);
  uart2_init(115200);
  Moto_Init();
  TIM4_Int_Init(5, 7199);     // 10Khz的计数频率，计数到5000为500ms
  TIM2_Int_Init(1000, 7199);  // 10Khz的计数频率，计数到5000为500ms
  TIM3_Int_Init(HEAR_BEAT_TIME, 7199);  // 10Khz的计数频率，计数到5000为500ms

  sound_control();
  CLOSE_ELECTRIC_LOCK;
  init_moto();
  RTC_Init();
  DBG_LOG("system start!")

  IWDG_Init(6, 1024);  //与分频数为64,重载值为625,溢出时间为1s

  while (1) {
    IWDG_Feed();
    Screen_CommandReceive_Poll();
    Start_Schedule();
    open_all_door();
    wait_fun();
    send_hart();
  }
}

void led_light(void) {
  RTC_Get();
  if (calendar.hour <= 6) {
    // led turns off
    GPIO_ResetBits(GPIOB, GPIO_Pin_8);
    GPIO_ResetBits(GPIOB, GPIO_Pin_9);	
    GPIO_ResetBits(GPIOB, GPIO_Pin_6);	
    GPIO_ResetBits(GPIOG, GPIO_Pin_15);
  } else {
    // led turns on
    // DBG_LOG("calendar.hour = %d",calendar.hour);
    GPIO_SetBits(GPIOB, GPIO_Pin_8);
     GPIO_SetBits(GPIOB, GPIO_Pin_9);
     GPIO_SetBits(GPIOB, GPIO_Pin_6);
     GPIO_SetBits(GPIOG, GPIO_Pin_15);
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



static void funControl(int argc, char *argv[]) {
  uint8_t i = 0;
  argv++;
  argc--;
  if (ARGV_EQUAL("CHECK_ALL_MOTO")) {   // check all moto 
    for (i = 0; i <= PINMAX; i++) {
      IWDG_Feed();
      DBG_LOG("open moto num %d",i);
      motoDef.open_moto(i);
      delay_ms_whx(100);
      motoDef.close_moto(i);
    }
  } else if (ARGV_EQUAL("OPEN_LOCK")) {   // open clock 
      DBG_LOG("open lock num %d",uatoi(argv[1]));
      open_lock(uatoi(argv[1]));
  } else if (ARGV_EQUAL("CHECK_ALL_LOCK")) {  // check all lock 
      for(uint8_t i=0;i<=LOCK_MAX;i++) {
        IWDG_Feed();
        DBG_LOG("open lock num %d",i);
        open_lock(i);
        delay_ms_whx(100);
        close_lock(i);
      }
  } else if (ARGV_EQUAL("CLOCK_LOCK")) {    // close lock
      DBG_LOG("close lock num %d",uatoi(argv[1]));
      close_lock(uatoi(argv[1]));
  } else if (ARGV_EQUAL("RUN")) {   // run moto 
    motoDef.open_moto(uatoi(argv[1]));
  } else if (ARGV_EQUAL("CLOSE")) { // close moto 
    motoDef.close_moto(uatoi(argv[1]));
  } else if (ARGV_EQUAL("motor_turn_off")) {  // moto turn left
    PUSH_MOTOR(LEFT);
  } else if (ARGV_EQUAL("hello_world")) {   // hello,world
    DBG_LOG("hello,world!");
  } else if (ARGV_EQUAL("TEST_NUM")) {  // test num 
    DBG_LOG("test num %d",uatoi(argv[1]));
    motoDef.num = uatoi(argv[1]);
  } else if (ARGV_EQUAL("FEETBACK")) {  // feetback signal
    DBG_LOG("FEETBACK");
    if (motoDef.get_moto_feetback(uatoi(argv[1])) == 0) {
      DBG_LOG("get the signal");
    } else if (motoDef.get_moto_feetback(uatoi(argv[1])) == 1) {
      DBG_LOG("have no signal");
    }
  } else if (ARGV_EQUAL("CHECK_SENSOR_A")) {  // check notch sensor a
     DBG_LOG("sensor A test!");
     for (uint16_t i=0;i<=500;i++) {
       IWDG_Feed();
       if((GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_14)) == 0) {   // signal A
         DBG_LOG("sensor A equal 0");
       } else if ((GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_14)) == 1) {
         DBG_LOG("sensor A equal 1");
       }
     } 
  } else if (ARGV_EQUAL("CHECK_SENSOR_B")) {    // check notch sensor b
     DBG_LOG("sensor B test!");
     for (uint16_t i=0;i<=500;i++) {
       IWDG_Feed();
       if((GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_5)) == 0) {   // signal  B
         DBG_LOG("sensor B equal 0");
       } else if ((GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_5)) == 1) {
         DBG_LOG("sensor B equal 1");
       }
     }
  } else if (ARGV_EQUAL("CHECK_SENSOR_C")) {      // other notch
     DBG_LOG("sensor C test!");
  } else if (ARGV_EQUAL("CHECK_MOTO_SIGNAL")) {
    motoDef.open_moto(uatoi(argv[1]));
    while (1) {
      IWDG_Feed();
      if (motoDef.get_moto_feetback(uatoi(argv[1])) == 0) {
        motoDef.close_moto(uatoi(argv[1]));
        DBG_LOG("get the signal");
      } else if (motoDef.get_moto_feetback(uatoi(argv[1])) == 1) {
        DBG_LOG("no signal");
      }
    }
  } else if (ARGV_EQUAL("RUN_LEFT")) {      // moto steper left  out 
     DBG_LOG("run left %d",uatoi(argv[1]));
     steper_moto_in();
     MotorSetpperMove(uatoi(argv[1]));
  } else if (ARGV_EQUAL("RUN_RIGHT")) {      // moto steper right in
     DBG_LOG("run right %d",uatoi(argv[1]));
     steper_moto_out();
     MotorSetpperMove(uatoi(argv[1]));
  } else if (ARGV_EQUAL("RUN_BACK")) {      // moto steper run back 
      init_moto();
  } else if (ARGV_EQUAL("CHECK_LOCK_OPEN")) {      // door lock open 
      OPEN_ELECTRIC_LOCK;
      delay_ms_whx(200);
      CLOSE_ELECTRIC_LOCK;
  } else if (ARGV_EQUAL("GET_TIME")) {
      DBG_LOG("calendar.hour = %d\ncalendar.min = %d",\
      calendar.hour,calendar.min);
  } else if (ARGV_EQUAL("RESTART")) {
      DBG_LOG("restart");
      NVIC_SystemReset();
  }
}
