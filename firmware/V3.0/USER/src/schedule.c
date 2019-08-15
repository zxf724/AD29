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
uint8_t flag_finish = 1;

void Start_Schedule() {
  uint8_t state = 0;
  switch (machine.state) {
    case state_stop:
      if (!Set_Gun()) {  //开扫码枪
        machine.state = state_report;
        machine.gun_state = state_report;
      } else if (Set_Gun() == 0xff) {
        machine.state = state_report;
        machine.gun_state = state_error;
      }
      if (Set_Moto() < 33 && Set_Moto() > 0) {  //出货电机
        machine.state = state_report;
        machine.moto_state = state_report;
      } else if (Set_Moto() >= 33) {  // 回收门锁电机
        machine.state = state_report;
        machine.lock_state = state_report;
      } else if (Set_Moto() == 0xff) {
        if (motoDef.num >= 33)
          machine.lock_state = state_error;
        else
          machine.moto_state = state_error;
        machine.state = state_report;
      }
      // Report_Bar_Code();
      break;
    case state_repay:
      Start_Repay(); 
      break;
    case state_borrow:
      Start_Borrow();
      break;
    case state_report:
      machine.state = state_stop;
      if (machine.moto_state == state_error) {
        state = 0;
        Report_State(CMD_REMOTO, &state, 1);  //上报出货电机错误
        machine.moto_state = state_stop;
        motoDef.num = 0;
      } else if (machine.moto_state == state_report) {
        state = 1;
        // Report_State(CMD_REMOTO,&state,1);
        machine.state = state_borrow;  //进入借物流程
        machine.moto_state = state_stop;
      }
      if (machine.lock_state == state_error) {
        state = 0;
        Report_State(CMD_RELOCK, &state, 1);  //上报门锁错误
        machine.moto_state = state_stop;
        motoDef.num = 0;
      } else if (machine.lock_state == state_report) {
        state = 1;
        Report_State(CMD_RELOCK, &state, 1);
        machine.state = state_repay;  //进入还物流程.
        machine.lock_state = state_stop;
      }

      if (machine.gun_state == state_error) {
        state = 0;
        Report_State(CMD_REGUN, &state, 1);  //上报扫码枪错误
        machine.gun_state = state_stop;
        memset(g_start_cmd, 0, sizeof(g_start_cmd));
      } else if (machine.gun_state == state_report) {
        state = 1;
        Report_State(CMD_REGUN, &state, 1);  //上报扫码枪正确
        machine.gun_state = state_stop;
        memset(g_start_cmd, 0, sizeof(g_start_cmd));
      }
      break;
  }
}

/**
 * start borrow tools
 * @param argc
 * @param argv
 */
void Start_Borrow() {
  static uint8_t flag_steper = 0, i = 0;
  IWDG_Feed();
  uint8_t check_num = 0;
  switch (motoDef.state) {
    case state_stop:
      if (motoDef.num) {
        motoDef.state = state_run_first;
      } else {
        machine.state = state_stop;
      }
      break;
    case state_run_first:  // input
      motoDef.open_moto(motoDef.num);
      if ((motoDef.num >= 17) && (motoDef.num <= 24)) {
        check_num = CHECK_TRACK_1;
      } else if ((motoDef.num >= 1) && (motoDef.num <= 8)) {
        check_num = CHECK_TRACK_2;
      } else if ((motoDef.num >= 9) && (motoDef.num <= 16)) {
        check_num = CHECK_TRACK_3;
      } else if ((motoDef.num >= 25) && (motoDef.num <= 32)) {
        check_num = CHECK_TRACK_4;
      }
      if ((motoDef.read_moto(check_num) == 0)) {  // CHECK_TRACK	change
        motoDef.close_moto(motoDef.num);
        motoDef.state = state_run_second;
      }
      break;
    case state_run_second:
      // check infrared  output 0 signal when it cover
      delay_ms_whx(500);
      IWDG_Feed();
      GPIO_SetBits(GPIOC, GPIO_Pin_10);  // EN1
      GPIO_ResetBits(
          GPIOC,
          GPIO_Pin_11);  // DIR1   GPIO_SetBits() -> out  GPIO_ResetBits() -> in
      GPIO_SetBits(GPIOC, GPIO_Pin_12);  // EN2
      GPIO_ResetBits(
          GPIOD,
          GPIO_Pin_0);  // DIR2   GPIO_SetBits() -> out  GPIO_ResetBits() -> in
      if (flag_steper == 0) {
        flag_steper = 1;
        MotorSetpperMove(40000);
      }
      delay_ms_whx(100);
      OPEN_ELECTRIC_LOCK;
      if (NEW_SENSOR == 1) {  // sensor
        // DBG_LOG("111111111111");
        motoDef.state = state_run_second_half;
      }
      break;
    case state_run_second_half:
      IWDG_Feed();
      if (NEW_SENSOR == 0) {
        delay_ms_whx(4000);
        IWDG_Feed();
        if (NEW_SENSOR == 1) {
          // DBG_LOG("222222222222");
          motoDef.state = state_run_second;
        } else if (NEW_SENSOR == 0) {
          // DBG_LOG("333333333333");
          motoDef.state = state_run_third;
        }
      }
      break;
    case state_run_third:  // push motor
      IWDG_Feed();
      if (NEW_SENSOR == 0) {
        // DBG_LOG("state_run_third");
        delay_ms_whx(500);
        CLOSE_ELECTRIC_LOCK;
        IWDG_Feed();
        // PUSH_MOTOR(LEFT);
        GPIO_SetBits(GPIOC, GPIO_Pin_10);  // EN1
        GPIO_SetBits(GPIOC, GPIO_Pin_11);  // DIR1   GPIO_SetBits() -> out
                                           // GPIO_ResetBits() -> in
        GPIO_SetBits(GPIOC, GPIO_Pin_12);  // EN2
        GPIO_SetBits(GPIOD, GPIO_Pin_0);   // DIR2   GPIO_SetBits() -> out
                                           // GPIO_ResetBits() -> in
        flag_steper = 0;

        TIM_Cmd(TIM2, ENABLE);  //使能TIMx
        TIM_Cmd(TIM4, ENABLE);  //使能TIMx
        flag_calc_times = 0;

        while (flag_calc_times != 1) {
          // MicroStep_Motro_init(1);
          delay(900);
          GPIO_SetBits(GPIOB, GPIO_Pin_3);
          GPIO_SetBits(GPIOB, GPIO_Pin_4);
          delay(900);
          GPIO_ResetBits(GPIOB, GPIO_Pin_3);
          GPIO_ResetBits(GPIOB, GPIO_Pin_4);
        }
        flag_new_sensor = 0;
        // while (TOUR_SWITCH != 0) {
        //   MicroStep_Motro_init(10);
        // }
        // clear num
        TIM_Cmd(TIM2, DISABLE);  //使能TIMx
        TIM_Cmd(TIM4, DISABLE);  //使能TIMx

        motoDef.num = 0;
        i = 0;
        flag_steper = 0;
        motoDef.state = state_report;
      }
      break;
    case state_report:
      // Report_State(CMD_RECARGO,&state,1);  //出货信息上报
      flag_finish = 1;
      if (TOUR_SWITCH == 1) {
        // DBG_LOG("error");
      }
      if (errorDef.android_state) {  //收到ANDROID消息
        errorDef.android_state = 0;
        motoDef.state = state_stop;
        errorDef.error_count = 0;
        errorDef.android_state = 0;
      } else {
        errorDef.error_count++;
        delay_ms(5);
        if (errorDef.error_count >= 10) {
          motoDef.state = state_stop;
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
  // static uint8_t check_flag = 0;
  switch (motoDef.state) {
    case state_stop:
      if (motoDef.num) {
        motoDef.open_moto(motoDef.num);
        motoDef.state = state_door_open;
      } else
        machine.state = state_stop;
      break;
    case state_door_open:
      delay_ms(1000);
      motoDef.close_moto(motoDef.num);
      motoDef.state = state_report;
      break;
    case state_report:
      flag_finish = 1;
      memset(g_start_cmd, 0, sizeof(g_start_cmd));
      motoDef.num = 0;
      motoDef.state = state_stop;
      break;
  }
}
