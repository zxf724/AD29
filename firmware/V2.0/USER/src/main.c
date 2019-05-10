#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"
#include "fifo.h"
#include "analysis_protocol.h"
#include "prjlib.h"
#include "rtc.h"
#include "moto.h"
#include "schedule.h"
#include "wdg.h"
#include "key.h"
#include "ananlysis_data.h"
#include "prjlib.h"

static void funControl(int argc, char* argv[]);
extern Moto motoDef;
extern mPin Pin_Array[PINMAX];
int time = 0;
_calendar_obj fff;
int main(void)
{		
	CMD_ENT_DEF(MOTO, funControl);
  Cmd_AddEntrance(CMD_ENT(MOTO));
	delay_init();	    	   
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
  uart1_init(115200);
	uart2_init(9600);
	Moto_Init();


	DBG_LOG("system start");
  if(RTC_Init())
		DBG_LOG("RTC Init fail");
	IWDG_Init(6,1024);    //与分频数为64,重载值为625,溢出时间为1s	
	fff.w_year = 2019;
	fff.w_month = 3;
	fff.w_date = 26;
	fff.hour = 14;
	fff.min = 00;
	fff.sec = 0;
	
  while(1) {
	IWDG_Feed();
	time = mytime_2_utc_sec(&fff,0);
	Gun_CommandReceive_Poll(); 
	Screen_CommandReceive_Poll();
	Start_Schedule();
	// open_all_door();
	led_light();
	test_fun();
		// printf("motoDef.state = %d",motoDef.state);
	}
}

static void funControl(int argc, char* argv[]) {
	
	uint8_t i = 0;
  argv++;
  argc--;
  if(ARGV_EQUAL("ALL_OPEN")) 
	{  
	   for(i = 1;i < 57;i++)
		{ 
		   motoDef.open_moto(i);
		}
	}else if(ARGV_EQUAL("ALL_CLOSE")) 
	{  
	   for(i = 0;i < 57;i++)
		 {
		   motoDef.close_moto(i);
		 }
	}else if(ARGV_EQUAL("RUN")) 
	{  
		  motoDef.open_moto(uatoi(argv[1]));
	}else if(ARGV_EQUAL("CLOSE")) 
	{  
		  motoDef.close_moto(uatoi(argv[1]));
	}
}

void led_light(void) {
	RTC_Get();
	// showcase,time can change according to the demand.
	// if((fff.hour > 6) || (fff.hour <= 24)) {
	if(1) {
		GPIO_SetBits(GPIOD,GPIO_Pin_0);
	} else {
		GPIO_ResetBits(GPIOD,GPIO_Pin_0);
	}
	// logo part, time  can change according to the demand.
	// if((fff.hour > 6) || (fff.hour <= 24)) {
	if(1) {
		GPIO_SetBits(GPIOD,GPIO_Pin_1);
	} else {
		GPIO_ResetBits(GPIOD,GPIO_Pin_1);
	}
	
}

void test_fun() {
	// motoDef.open_moto(1);
	// motoDef.open_moto(2);
	// if(motoDef.read_moto(CHECK_DROP)) {
	// 	DBG_LOG("signal feedback55");
	// }
	delay_ms(2000);
	motoDef.open_moto();
	delay_ms(2000);
}