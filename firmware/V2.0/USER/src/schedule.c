#include "moto.h"
#include "analysis_protocol.h"
#include "schedule.h"
#include "string.h"
#include "gun.h"
#include "usart.h"
#include "ananlysis_data.h"
#include "delay.h"
#include "gun.h"

extern uint8_t g_start_cmd[7];
extern Moto motoDef;
extern Machine machine;
extern mError errorDef;

void Start_Schedule()
{
	 uint8_t state = 0;
   switch(machine.state)
	 {
		 case state_stop:
				if(!Set_Gun()) //开扫码枪
				{
					machine.state = state_report;
					machine.gun_state = state_report;
				}else if(Set_Gun() == 0xff)
				{
				  machine.state = state_report;
				  machine.gun_state = state_error;
				}
				if(Set_Moto() < 33 && Set_Moto() > 0) //出货电机
				{
					machine.state = state_report;
					machine.moto_state = state_report;
				}else if(Set_Moto() >= 33) //回收门锁电机
				{
					machine.state = state_report;
					machine.lock_state = state_report;
				}else if(Set_Moto() == 0xff)
				{
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
			 break;
			  Start_Borrow();
		 case state_report:
			   machine.state = state_stop;
			   if(machine.moto_state == state_error)
				 {
					  state = 0;
						Report_State(CMD_REMOTO,&state,1);	  //上报出货电机错误
					  machine.moto_state = state_stop;
					  motoDef.num = 0;
				 }else if(machine.moto_state == state_report)
				 {
					  state = 1;
					  Report_State(CMD_REMOTO,&state,1);
					  machine.state = state_borrow;       //进入借物流程
				    machine.moto_state = state_stop;
				 }
				 
				 if(machine.lock_state == state_error)
				 {
					  state = 0;
					  Report_State(CMD_RELOCK,&state,1);		  //上报门锁错误
					  machine.moto_state = state_stop;
					  motoDef.num = 0;
				 }else if(machine.lock_state == state_report)
				 {
					  state = 1;
					  Report_State(CMD_RELOCK,&state,1);
					  machine.state = state_repay;           //进入还物流程
				    machine.lock_state = state_stop;
				 }

				 if(machine.gun_state == state_error)
				 {   
						state = 0;
					  Report_State(CMD_REGUN,&state,1);		//上报扫码枪错误
				    	machine.gun_state = state_stop;
					  memset(g_start_cmd,0,sizeof(g_start_cmd));
				 }else if(machine.gun_state == state_report)
			    {
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
	uint8_t state = 0;
	switch(motoDef.state) {
		case state_stop:
			  if(motoDef.num) {
					motoDef.state = state_run_first;
				} else {
					machine.state = state_stop;
				}
			break;
		case state_run_first: //启动送货履带
			if(!Check_Moto(CHECK_DROP)) {
				motoDef.close_moto(motoDef.num);
				motoDef.state = state_run_second;
				motoDef.num = 0;  //清除电机标志
			}
		break;
		case state_run_second:	//启动出货电机
			motoDef.open_moto(MOTO_CARGO);
			if(!Check_Moto(CHECK_CARGO)) {
				motoDef.close_moto(MOTO_CARGO);
				motoDef.open_moto(DOOR_CARGO);
				motoDef.state = state_run_third;
			}
			motoDef.state = state_run_third;
		break;
		case state_run_third:  //开出货口的门
			if(!motoDef.read_moto(DOOR_CARGO)) {
				motoDef.close_moto(DOOR_CARGO);
				motoDef.state = state_report;
			}
			motoDef.state = state_report;
			break;
		case state_report:
			state = 1;
		  	Report_State(CMD_RECARGO,&state,1);  //出货信息上报
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
	switch(motoDef.state)
	{
		case state_stop:
			  if(motoDef.num)
				{
					motoDef.state = state_door_open;
					motoDef.open_moto(motoDef.num);
				}
				else
					machine.state = state_stop;
			break;
		case state_door_open:
				delay_ms(3000);
				motoDef.close_moto(motoDef.num);
				// if(motoDef.read_moto(motoDef.num + 24))
				// {
				//    motoDef.close_moto(motoDef.num);
				// 	 check_flag = 1;
				// }else if(check_flag && !motoDef.read_moto(motoDef.num))
				// {
				// 	check_flag = 0;
				//     motoDef.state = state_report;
				// }
			break;
		case state_report:
	      memset(g_start_cmd,0,sizeof(g_start_cmd));
		    if(motoDef.num) motoDef.num = 0;
			  motoDef.state = state_stop;
			break;
	}
}
