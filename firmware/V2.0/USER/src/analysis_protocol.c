#include "analysis_protocol.h"
#include "fifo.h"
#include "usart.h"	
#include "prjlib.h"
#include "ananlysis_data.h"
#include "delay.h"
#include "moto.h"
#include "key.h"
#include "wdg.h"
#include "string.h"
#include "stdlib.h"

#define	MOTOR_NUM		13

uint8_t  CmdRecBuf[COMMAND_MAX] = {0};
extern uint8_t     g_bar_code[50];
extern mError errorDef;

extern app_fifo_t  rx_fifo_Screen_Def;
extern app_fifo_t  rx_fifo_Gun_Def; 
extern uint8_t gs_screen_rx_buff[1024];

uint8_t data[8] = {0};

void Screen_CommandReceive_Poll(void) 
{
  uint16_t index = 0;
  uint32_t len = 0;
	uint8_t i = 0;
   len = fifo_length(&rx_fifo_Screen_Def);
	if(len >= 12) {
		IWDG_Feed();
		delay_ms(100);
		len = fifo_length(&rx_fifo_Screen_Def);
		for(i=0;i<len;i++)
		app_uart_get(&CmdRecBuf[i],SCREEN);   //one bit
		len = 0;
		Uart_Protocol_Cmd_Analy(CmdRecBuf,index);
	}

  //while(app_uart_get(&CmdRecBuf[index],SCREEN) == NRF_SUCCESS)
	if(0)
  {
		
#if 1
    if(index >= 8) {
     //Uart_Protocol_Cmd_Analy(CmdRecBuf,index); 
    } else {
			// if(index == 6)
			// 	len = CmdRecBuf[index];
		  // index++;
      // delay_ms(2);
		}
#else
		if (CmdRecBuf[index] == '\n' && CmdRecBuf[index - 1] == '\r') 
		{
			CmdRecBuf[index+1] = '\0';
			p = (char*)&CmdRecBuf[0];
		  while (!isgraph(*p)) 
			{
				p++;
			}			
			Cmd_Handle(p);
			index = 0;
		}else 
		{
			delay_ms(2);
			index++;
		}
#endif
   }
}

void Gun_CommandReceive_Poll(void) 
{
  uint32_t len = 0;
	uint8_t i = 0;
	uint8_t data_tmp[16] = {0};
  len = fifo_length(&rx_fifo_Gun_Def);
	if(len >= 16) {
		IWDG_Feed();
		delay_ms(100);
		len = fifo_length(&rx_fifo_Gun_Def);
		for(i=0;i<len;i++)
		app_uart_get(&CmdRecBuf[i],GUN);   //one bit
		len = 0;
		data_tmp[0] = (uint8_t)(CmdRecBuf[0] - 48);
		if (data_tmp[0] == 1) {
			for(i=0;i<16;i++) {
			data_tmp[i] = (uint8_t)(CmdRecBuf[i] - 48);
			DBG_LOG("data_tmp[%d] = %d",i,data_tmp[i]);
			}
			for(i=0;i<=7;i++) {
				data[i] =(data_tmp[i*2]*10)+data_tmp[i*2+1];
				DBG_LOG("data[%d] = 0x%02x",i,data[i]);
			}
			Report_State(0x05,data,sizeof(data));
		}
	}
}

void Uart_Protocol_Cmd_Analy(uint8_t* CmdRecBuf,uint8_t length) {
	uint8_t i = 0;
	static uint8_t report_data[8] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08};
	static uint8_t start_screen[6] = {0x04,0xE4,0x04,0x00,0xFF,0x14};
	static uint8_t stop_screen[6] = {0x04,0xE5,0x04,0x00,0xFF,0x13};

	for (i=0; i<=17; i++) {
		 	//  // DBG_LOG("CmdRecBuf[%d] = 0x%02x",i,CmdRecBuf[i]);
		}	
	// crc16 test  already test 
	uint16_t crc_data_count = CRC_16(0xffff,CmdRecBuf+1,14);
	//and crc16
  if((CmdRecBuf[0] == FHEADER) && (CmdRecBuf[17] == FHEADER)) {
        switch(CmdRecBuf[1]) {
					case CMD_TIME:
						Get_Time(CmdRecBuf);
						Report_State(0x80,(uint8_t*)report_data,sizeof(report_data));
						break;
					case CMD_MOTO:
            Get_Mote_Data(&CmdRecBuf[MOTOR_NUM]);
						break;
					case CMD_LOCK:
						Get_Lock_Data(&CmdRecBuf[MOTOR_NUM]);
						// Uart_Send_Data(SCREEN, report_data,sizeof(dat_tmp));
						break;
					case CMD_GUN:
            			static uint8_t start_screen[6] = {0x04,0xE4,0x04,0x00,0xFF,0x14};
						for(uint8_t i=0;i<=3;i++) {
							delay_ms(20);
							Uart_Send_Data(GUN,start_screen,sizeof(start_screen)-1);
						}
						break;
					case CMD_SCREEN_CLOSE:		//no use
						break;
					case CMD_CARGO:
                Get_Cargo_Data(&CmdRecBuf[2]);
						break;
					case CMD_BACK:
                Get_Back_Data(&CmdRecBuf[2]);
						break;	
				}
    } 
}

void open_all_door(void) {
	//check the key
	static uint8_t key = 0;
	static uint8_t i = 0;
	key = KEY_Scan(1);
	if(key) {
		switch (key) {
		case KEY_ALL_NUM:				
		// DBG_LOG("open all the door");
		for(i=33;i<=54;i++) {
			IWDG_Feed();
			Open_xMoto(i);
			delay_ms(200);
			Close_xMoto(i);
		}
		break;
	default:
		break;
		} //end of switch
	}
}

