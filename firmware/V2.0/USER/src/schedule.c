#include "moto.h"
#include "analysis_protocol.h"
#include "schedule.h"
#include "string.h"
#include "gun.h"
#include "usart.h"
#include "ananlysis_data.h"
#include "delay.h"
#include "gun.h"
#include "wdg.h"

extern uint8_t g_start_cmd[7];
extern Moto motoDef;
extern Machine machine;
extern mError errorDef;

void Start_Schedule() {
	uint8_t state = 0;
	switch(machine.state) {
		case state_stop:
			if(!Set_Gun()) {		//开扫码枪 
				machine.state = state_report;
				machine.gun_state = state_report;
			} else if(Set_Gun() == 0xff) {
				machine.state = state_report;
				machine.gun_state = state_error;
			}
			if(Set_Moto() < 33 && Set_Moto() > 0) {		//出货电机
				machine.state = state_report;
				machine.moto_state = state_report;
			} else if(Set_Moto() >= 33)  {	//回收门锁电机
				machine.state = state_report;
				machine.lock_state = state_report;
			} else if(Set_Moto() == 0xff) {
				if(motoDef.num >= 33) machine.lock_state = state_error;
				else machine.moto_state = state_error;
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
			if(machine.moto_state == state_error) {
				state = 0;
				Report_State(CMD_REMOTO,&state,1);	  //上报出货电机错误
				machine.moto_state = state_stop;
				motoDef.num = 0;
			} else if(machine.moto_state == state_report) {
				state = 1;
				// Report_State(CMD_REMOTO,&state,1);
				machine.state = state_borrow;       //进入借物流程
				machine.moto_state = state_stop;
			}
			if(machine.lock_state == state_error) {
				state = 0;
				Report_State(CMD_RELOCK,&state,1);		  //上报门锁错误
				machine.moto_state = state_stop;
				motoDef.num = 0;
			}else if(machine.lock_state == state_report) {
				state = 1;
				Report_State(CMD_RELOCK,&state,1);
				machine.state = state_repay;           //进入还物流程.
				// DBG_LOG("machine.state = state_repay");
				machine.lock_state = state_stop;
			}

			if(machine.gun_state == state_error) {   
				state = 0;
				Report_State(CMD_REGUN,&state,1);		//上报扫码枪错误
				machine.gun_state = state_stop;
				memset(g_start_cmd,0,sizeof(g_start_cmd));
			} else if(machine.gun_state == state_report) {
				state = 1;
				Report_State(CMD_REGUN,&state,1);//上报扫码枪正确
				machine.gun_state = state_stop;
				memset(g_start_cmd,0,sizeof(g_start_cmd));
			}				 
			 break; 
	 }
}

/**
 * start borrow tools
 * @param argc 
 * @param argv 
 */
void Start_Borrow()
{
	IWDG_Feed();
	static uint8_t flag_signal_transfer = 0;
	switch(motoDef.state) {
		case state_stop:
			if(motoDef.num) {
				motoDef.state = state_run_first;
			} else { 
				machine.state = state_stop;
			}
			break;
		case state_run_first: // input 
				motoDef.open_moto(motoDef.num);
				// DBG_LOG("hello,world!");
			if(motoDef.read_moto(CHECK_TRACK)) {  // CHECK_TRACK
				flag_signal_transfer = 1;
				motoDef.close_moto(motoDef.num);
				motoDef.state = state_run_second;
			}
			break;
		case state_run_second:
			//check infrared  output 0 signal when it cover
			if(!(motoDef.read_moto(INFRARED_SENSOR_TEST))) {
				if(flag_signal_transfer == 1) {
					delay_ms_whx(100);
					OPEN_ELECTRIC_LOCK;
					PUSH_MOTOR(RIGHT);
					IWDG_Feed();
					delay_ms_whx(1000);
					motoDef.state = state_run_third;
				}
			}
		break;
		case state_run_third:  // push motor
			delay_ms_whx(1000);			
			if(motoDef.read_moto(INFRARED_SENSOR_TEST)) {
				IWDG_Feed();
				PUSH_MOTOR(LEFT);
				delay_ms_whx(5000);
				IWDG_Feed();
				CLOSE_ELECTRIC_LOCK;
				flag_signal_transfer = 0;
				// clear num
				motoDef.num = 0;
				motoDef.state = state_report;
			}
			break;
		case state_report:
		  	// Report_State(CMD_RECARGO,&state,1);  //出货信息上报
		  	if(errorDef.android_state) { //收到ANDROID消息
				errorDef.android_state = 0;
				motoDef.state = state_stop;
				errorDef.error_count = 0;
				errorDef.android_state = 0;
			} else {
		   	errorDef.error_count++;
				delay_ms(5);
				if(errorDef.error_count >= 10) {
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
void Start_Repay()
{
	//static uint8_t check_flag = 0;
	switch(motoDef.state) {
		case state_stop:
			if(motoDef.num) {
				motoDef.open_moto(motoDef.num);
				motoDef.state = state_door_open;
				// DBG_LOG("hello,world!");
			} else
				machine.state = state_stop;
			break;
		case state_door_open:
			delay_ms(3000);
			motoDef.close_moto(motoDef.num);
			// DBG_LOG("hello,world!");
			motoDef.state = state_report;
			break;
		case state_report:
	      memset(g_start_cmd,0,sizeof(g_start_cmd));
		    if(motoDef.num) {
				motoDef.num = 0;
				motoDef.state = state_stop;
			}
			break;
	}
}
