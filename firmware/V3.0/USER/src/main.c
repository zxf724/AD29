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
  // TIM3_Int_Init(500, 7199);
  TIM4_Int_Init(HEAR_BEAT_TIME, 7199);  // 10Khz的计数频率，计数到5000为500ms
  // TIM2_Int_Init(10000, 7199);
  sound_control();
  CLOSE_ELECTRIC_LOCK;
  init_moto();
  delay_ms_whx(1000);

  // steper moto
  Initial_PWM_Motor1();  // PUL1- PB3 TIM2_CH2
  Initial_PWM_Motor2();  // PUL2- PB4 TIM3_CH1
  MotorRunParaInitial();  //初始化电机运行参数，主要是根据S型曲线参数生成表格
  Initial_Motor(
      1, M1DIV,
      73600);  //将电机的驱动细分，通过控制连接THB6128的GPIO引脚的电平实现
  Initial_Motor(
      2, M2DIV,
      73600);  // 此外还有指定各个电机使用的定时器，GPIO，电机顺时针方向数值等参数

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
#if TEST
  // test borrow motor
  motoDef.open_moto(2);
  if (motoDef.read_moto(CHECK_TRACK)) {
    DBG_LOG("signal feedback55");
    motoDef.close_moto(2);
  }

  // // test push motor
  // delay_ms(2000);
  // motoDef.open_moto();
  delay_ms(2000);

  // test infrared
  if (!(motoDef.read_moto(INFRARED_SENSOR_TEST))) {
    // DBG_LOG("infrared!");
  }
  // test push motor
  GPIO_ResetBits(GPIOC, GPIO_Pin_11);
  GPIO_SetBits(GPIOC, GPIO_Pin_10);
  PUSH_MOTOR(RIGHT);  // out
  PUSH_MOTOR(LEFT);   // in

  // test electric lock
  IWDG_Feed();
  OPEN_ELECTRIC_LOCK;
  delay_ms_whx(5000);
  IWDG_Feed();
  CLOSE_ELECTRIC_LOCK;
  delay_ms_whx(5000);
  // crc16 test!
  uint16_t crc_test;
  uint8_t crc[10] = {0x01, 0x02, 0x03, 0x04, 0x05,
                     0x06, 0x07, 0x08, 0x09, 0x0A};
  crc_test = CRC_16(0xffff, crc + 2, 5);
  // DBG_LOG("crc_test = 0x%04x",crc_test);

  // led bug
  motoDef.open_moto(18);
  if (motoDef.read_moto(CHECK_TRACK)) {
    // DBG_LOG("test!");
    // DBG_LOG("hello,world!");
  }

  // test unix time seting
  uint8_t CmdRecBuf[18] = {0x7E, 0x00, 0x5C, 0xE4, 0xA6, 0x42,
                           0X07, 0x53, 0x43, 0x41, 0x4E, 0x20,
                           0x4F, 0x4E, 0x00, 0x63, 0xBF, 0x7E};
  Get_Time(CmdRecBuf);
  delay_ms_whx(1000);
  // DBG_LOG("calendar.sec is %d",calendar.hour);

  uint16_t data = 0xe267;
  uint8_t data15 = 0, data16 = 0;
  data15 = data;
  data16 = data >> 8;

  // // test input function
  delay_ms_whx(1000);
  static uint8_t report_data[8] = {0x01, 0x02, 0x03, 0x04,
                                   0x05, 0x06, 0x07, 0x08};
  Report_State(0x80, (char *)report_data, sizeof(report_data));

  // // test new push motor
  PUSH_MOTOR(LEFT);
  PUSH_MOTOR_RIGHT;

  // // screen
  static uint8_t start_screen[6] = {0x04, 0xE4, 0x04, 0x00, 0xFF, 0x14};
  delay_ms(100);
  Uart_Send_Data(GUN, start_screen, (sizeof(start_screen) - 1));

  success !!static uint8_t start_screen[6] = {0x04, 0xE4, 0x04,
                                              0x00, 0xFF, 0x14};
  delay_ms_whx(1000);
  Uart_Send_Data(GUN, start_screen, sizeof(start_screen) - 1);

  // MicroStep Motro
  static uint8_t flag = 0;

  GPIO_SetBits(GPIOC, GPIO_Pin_10);  // EN
  GPIO_SetBits(GPIOC, GPIO_Pin_11);  // DIR   GPIO_SetBits() -> out
  GPIO_ResetBits()->in

      GPIO_SetBits(GPIOC, GPIO_Pin_12);  // EN
  GPIO_SetBits(GPIOD, GPIO_Pin_0);       // DIR   GPIO_SetBits() -> out
  GPIO_ResetBits()->in if (flag == 0) {
    flag = 1;
    MicroStep_Motro(740);
  }

  // test tour switch
  if (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_5) == 0) {
    DBG_LOG("hello,world!");
  }

  // sensor
  if (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_4) == 1) {
    DBG_LOG("hello,");
  } else if (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_4) == 0) {
    DBG_LOG("world!");
  }

  // test road
  static uint8_t flag = 0;
  if (flag == 0) {
    flag = 1;
    motoDef.num = 22;
  }

  motoDef.open_moto(1);

  // get timestamp
  uint32_t time = RTC_GetCounter();
  uint8_t timestamp[4] = {0};
  DBG_LOG("time = 0x%08x", time);
  delay_ms_whx(1000);
  for (uint8_t i = 0; i <= 3; i++) {
    timestamp[i] = time >> (i * 8);
    DBG_LOG("timestamp[%d] = 0x%02x", i, timestamp[i]);
  }

  GPIO_SetBits(GPIOC, GPIO_Pin_10);  // EN1
  GPIO_SetBits(
      GPIOC,
      GPIO_Pin_11);  // DIR1   GPIO_SetBits() -> out  GPIO_ResetBits() -> in
  GPIO_SetBits(GPIOC, GPIO_Pin_12);  // EN2
  GPIO_SetBits(
      GPIOD,
      GPIO_Pin_0);  // DIR2   GPIO_SetBits() -> out  GPIO_ResetBits() -> in
  MicroStep_Motro(1);

  if (TOUR_SWITCH == 0) {
    DBG_LOG("hello,world!");
  } else if (TOUR_SWITCH == 1) {
    DBG_LOG("test!");
  }
#endif
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
    MotorSetpperMove(75000);  // 160000
  }

  //
}
