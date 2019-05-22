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

#define	MOTOR_NUM		13

uint8_t  CmdRecBuf[COMMAND_MAX] = {0};
extern uint8_t     g_bar_code[50];
extern mError errorDef;
uint8_t dat[26] = {0x7E,0x05,0x00,0x00,0x00,0x00,0x10,0x00,0x00,0x00,0x00,
									0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7E};  // 1+1+4+1+16+2+1 = 26

extern app_fifo_t  rx_fifo_Screen_Def;
extern uint8_t gs_screen_rx_buff[1024];
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
  uint16_t index = 0;
	uint8_t i = 0;
  char* p = NULL;
	while(app_uart_get(&CmdRecBuf[index],GUN) == NRF_SUCCESS) 
  {
		IWDG_Feed();
		if (CmdRecBuf[index] == '\n' && CmdRecBuf[index - 1] == '\r') 
		{
			CmdRecBuf[index+1] = '\0';
			p = (char*)&CmdRecBuf[0];
			strcpy((char*)g_bar_code,p);
 			errorDef.bar_code_state = 1;
			//send date
			for(i=0;i<=15;i++) {
				dat[7+i] = CmdRecBuf[i];
				DBG_LOG("dat[%d] = %d",(i+7),dat[7+i]);
			}
			Uart_Send_Data(SCREEN, (char *)dat,sizeof(dat));
		/*while (!isgraph(*p)) 
			{
				p++;
			}
			Cmd_Handle(p);*/
			index = 0;
		}else
		{
			delay_ms(2);
			index++; 
		}
  }
}

void Uart_Protocol_Cmd_Analy(uint8_t* CmdRecBuf,uint8_t length) {
	static uint8_t i = 0;

	for (i=0; i<17; i++){
		 	 DBG_LOG("CmdRecBuf[%d] = 0x%x",i,CmdRecBuf[i]);
		}	
	// crc16 test  already test 
	uint16_t crc_data_count = CRC_16(0xffff,CmdRecBuf+2,13);
	uint16_t crc_data = (CmdRecBuf[15] << 8) | CmdRecBuf[16];
	DBG_LOG("crc_data_count is 0x%04x",crc_data_count);
	DBG_LOG("crc_data is 0x%04x",crc_data);
	//and crc16
  if((CmdRecBuf[0] == FHEADER) && (CmdRecBuf[17] == FHEADER)) {
        switch(CmdRecBuf[1]) {
					case CMD_TIME:
						Get_Time(CmdRecBuf);
						break;
					case CMD_MOTO:
						DBG_LOG("data is %d",CmdRecBuf[MOTOR_NUM]);
            Get_Mote_Data(&CmdRecBuf[MOTOR_NUM]);
						break;
					case CMD_LOCK:
						Get_Lock_Data(&CmdRecBuf[MOTOR_NUM]);
						DBG_LOG("data is %x",CmdRecBuf[MOTOR_NUM]);
						// Uart_Send_Data(SCREEN, dat_tmp,sizeof(dat_tmp));
						// send_back(tmp); 
						break;
					case CMD_GUN:
            Get_Gun_Data(&CmdRecBuf[2]);
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
		DBG_LOG("open all the door");
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

