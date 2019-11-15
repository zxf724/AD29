#include "schedule.h"
#include "analysis_protocol.h"
#include "ananlysis_data.h"
#include "delay.h"
#include "gun.h"
#include "moto.h"
#include "stm32f10x_tim.h"
#include "string.h"
#include "usart.h"
#include "wdg.h"

extern uint8_t g_start_cmd[7];
extern Moto motoDef;
extern Machine machine;
extern mError errorDef;
extern uint8_t flag_new_sensor;
extern uint8_t flag_calc_times;
extern uint8_t report_data[8];
extern uint16_t  calc_times;
extern uint16_t delay_time;
extern uint8_t flag_door_time;

uint8_t close_800mm_moto = 0;
uint8_t close_3min_cargo = 0;
uint8_t flag_finish = 1;

void Start_Schedule() {
  switch (machine.state) {
    case state_stop:
      if (motoDef.num < 33 && motoDef.num > 0) {  //出货电机
        machine.state = state_borrow;
      }
      if ((motoDef.lock_num >= 33) && (motoDef.lock_num <= 54)) {
        machine.state = state_repay;  //进入借物流程
      }
      break;
    case state_repay:
      Start_Repay();
      break;
    case state_borrow:
      Start_Borrow();
      break;
  }
}

/**
 * start borrow tools
 * @param argc
 * @param argv
 */
void Start_Borrow() {
  IWDG_Feed();
  switch (motoDef.state) {
    case state_stop:
      if (motoDef.num) {
        motoDef.state = state_run_first;
        close_800mm_moto = 0;
      } else {
        machine.state = state_stop;
      }
      break;
    case state_run_first:  // input
      motoDef.open_moto(motoDef.num);
      if (motoDef.get_moto_feetback(motoDef.num) == 0) {
        if((motoDef.num>=1) && (motoDef.num<=4)) {
          delay_ms_whx(1700);
          motoDef.close_moto(motoDef.num);
          motoDef.state = state_run_second;
        } else if((motoDef.num>=5) && (motoDef.num<=16)) {
          delay_ms_whx(700);
          motoDef.close_moto(motoDef.num);
          motoDef.state = state_run_second;
        } else if((motoDef.num>=17) && (motoDef.num<=32)) {
          delay_ms_whx(500);
          motoDef.close_moto(motoDef.num);
          motoDef.state = state_run_second;
        }
      }
      if (close_800mm_moto >= DELAY_MOTO_STOP) {
          motoDef.close_moto(motoDef.num);
          motoDef.state = state_run_second;
      }
      break;
    case state_run_second:
      // check infrared  output 0 signal when it cover
      delay_ms_whx(500);
      motoDef.num = 0;
      IWDG_Feed();
      Report_State(HERAD, report_data, sizeof(report_data));
      delay_ms_whx(100);
      GPIO_SetBits(GPIOC, GPIO_Pin_10);  // EN1
      GPIO_SetBits(GPIOC, GPIO_Pin_12);  // EN2
      steper_moto_out();
      MotorSetpperMove(38000);
      Report_State(HERAD, report_data, sizeof(report_data));
      delay_ms_whx(100);
      motoDef.state = state_run_out_finish;
      close_3min_cargo = 0;
    case state_run_out_finish:
      IWDG_Feed();
      OPEN_ELECTRIC_LOCK;
      if (CHECK_RED_SIGNAL == 1) {  // sensor
        delay_ms(100);
        if (CHECK_RED_SIGNAL == 1) {  // sensor
          flag_door_time = 0;
          motoDef.state = state_run_second_half;
        }
      }
      if (close_3min_cargo >= DELAY_CARGO_STILL) {
          motoDef.state = state_run_third;
      }
      break;
    case state_run_second_half:
      IWDG_Feed();
      if ((CHECK_RED_SIGNAL == 1) && (flag_door_time < 50) && (flag_door_time >=10)) {
          motoDef.state = state_run_out_finish;
        }
      if ((CHECK_RED_SIGNAL == 0) && (flag_door_time >= 50)) {
        motoDef.state = state_run_third;
      }
      if (close_3min_cargo >= DELAY_CARGO_STILL) {
         motoDef.state = state_run_third;
      }
      break;
    case state_run_third:  // push motor
      IWDG_Feed();
      CLOSE_ELECTRIC_LOCK;
      IWDG_Feed();
      flag_calc_times = 0;
					flag_new_sensor = 0;
      calc_times = 0;
      delay_time = 400;
      init_moto();
      motoDef.num = 0;
      motoDef.state = state_report;
      flag_finish = 1;
      Report_State(FINISH, report_data, sizeof(report_data));
      delay_ms_whx(100);
      Report_State(HERAD, report_data, sizeof(report_data));
      delay_ms_whx(100);
      break;
    case state_report:
      // Report_State(CMD_RECARGO,&state,1);  //出货信息上报
      if (NORCH_SENSOR_A_MOTO == 1) {
        // DBG_LOG("error");  
      }
      if (errorDef.android_state) {  //收到ANDROID消息
        errorDef.android_state = 0;
        motoDef.state = state_stop;
        machine.state = state_stop;
        errorDef.error_count = 0;
        errorDef.android_state = 0;
      } else {
        errorDef.error_count++;
        delay_ms(5);
        if (errorDef.error_count >= 10) {
          motoDef.state = state_stop;
          machine.state = state_stop;
          errorDef.android_state = 0;
          errorDef.error_count = 0;
        }
      }
      break;
  }
}

/**
 * start repay tools
 * @param argc
 * @param argv
 */
void Start_Repay() {
  if(motoDef.lock_num >= 32) {
    open_lock(motoDef.lock_num-32);
    delay_ms_whx(50);
    close_lock(motoDef.lock_num-32);
    motoDef.lock_num = 0;
    flag_finish = 1;
    memset(g_start_cmd, 0, sizeof(g_start_cmd));
  }
}
