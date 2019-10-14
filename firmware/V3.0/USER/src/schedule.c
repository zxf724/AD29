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

uint8_t close_800mm_moto = 0;
uint8_t close_3min_cargo = 0;
uint8_t flag_finish = 1;

void Start_Schedule() {
  uint8_t state = 0;
  switch (machine.state) {
    case state_stop:
      if (!Set_Gun()) {  //��ɨ��ǹ
        machine.state = state_report;
        machine.gun_state = state_report;
      } else if (Set_Gun() == 0xff) {
        machine.state = state_report;
        machine.gun_state = state_error;
      }
      if (motoDef.num < 33 && motoDef.num > 0) {  //�������
        machine.state = state_report;
        machine.moto_state = state_report;
      } else if (Set_Moto() >= 33) {  // �����������
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
        Report_State(CMD_REMOTO, &state, 1);  //�ϱ������������
        machine.moto_state = state_stop;
        motoDef.num = 0;
      } else if (machine.moto_state == state_report) {
        state = 1;
        // Report_State(CMD_REMOTO,&state,1);
        machine.state = state_borrow;  //�����������
        machine.moto_state = state_stop;
      }
      if (machine.lock_state == state_error) {
        state = 0;
        Report_State(CMD_RELOCK, &state, 1);  //�ϱ���������
        machine.moto_state = state_stop;
        motoDef.num = 0;
      } else if (machine.lock_state == state_report) {
        state = 1;
        Report_State(CMD_RELOCK, &state, 1);
        machine.state = state_repay;  //���뻹������.
        machine.lock_state = state_stop;
      }

      if (machine.gun_state == state_error) {
        state = 0;
        Report_State(CMD_REGUN, &state, 1);  //�ϱ�ɨ��ǹ����
        machine.gun_state = state_stop;
        memset(g_start_cmd, 0, sizeof(g_start_cmd));
      } else if (machine.gun_state == state_report) {
        state = 1;
        Report_State(CMD_REGUN, &state, 1);  //�ϱ�ɨ��ǹ��ȷ
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
  static uint8_t flag_one_time=1;
  IWDG_Feed();
  uint8_t check_num = 0;
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
          delay_ms_whx(2500);
          motoDef.close_moto(motoDef.num);
          motoDef.state = state_run_second;
        } else if((motoDef.num>=5) && (motoDef.num<=16)) {
          delay_ms_whx(1500);
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
      IWDG_Feed();
      GPIO_SetBits(GPIOC, GPIO_Pin_10);  // EN1
      GPIO_SetBits(GPIOC, GPIO_Pin_12);  // EN2
      steper_moto_out();
      if(flag_one_time == 1) {
        MotorSetpperMove(38000);
        flag_one_time = 0;
        close_3min_cargo = 0;
      }
      delay_ms_whx(100);
            IWDG_Feed();
      OPEN_ELECTRIC_LOCK;
      if (NORCH_SENSOR_B_DOOR == 1) {  // sensor
        delay_ms(100);
        if (NORCH_SENSOR_B_DOOR == 1) {  // sensor
          motoDef.state = state_run_second_half;
        }
      }
      if (close_3min_cargo >= DELAY_CARGO_STILL) {
          motoDef.state = state_run_third;
      }
      break;
    case state_run_second_half:
      IWDG_Feed();
      flag_one_time = 1;
      if (NORCH_SENSOR_B_DOOR == 0) {
        delay_ms_whx(1000);
        IWDG_Feed();
        if (NORCH_SENSOR_B_DOOR == 1) {
          motoDef.state = state_run_second;
        } else if (NORCH_SENSOR_B_DOOR == 0) {
          motoDef.state = state_run_third;
        }
      }
      if (close_3min_cargo >= DELAY_CARGO_STILL) {
         motoDef.state = state_run_third;
      }
      break;
    case state_run_third:  // push motor
      IWDG_Feed();
      flag_one_time = 1;
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
      break;
    case state_report:
      // Report_State(CMD_RECARGO,&state,1);  //������Ϣ�ϱ�
      if (NORCH_SENSOR_A_MOTO == 1) {
        // DBG_LOG("error");  
      }
      if (errorDef.android_state) {  //�յ�ANDROID��Ϣ
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
        open_lock((motoDef.num-32));
        motoDef.state = state_door_open;
      } else
        machine.state = state_stop;
      break;
    case state_door_open:
      delay_ms(1000);
      close_lock((motoDef.num-32));
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
