#include "analysis_protocol.h"
#include "fifo.h"
#include "usart.h"	
#include "prjlib.h"
#include "ananlysis_data.h"
#include "delay.h"
#include "moto.h"

uint8_t  CmdRecBuf[COMMAND_MAX] = {0};
extern uint8_t     g_bar_code[50];
extern mError errorDef;
// static uint8_t dat_tmp[11] = {0x7E,0x82,0x00,0x01,0x02,0x03,0x01,0x01,0x00,0x00,0x7E};
static uint8_t dat_tmp[11] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
uint8_t dat[13] = {0x7E,0x05,0x00,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x00,0x00,0x7E};


void Screen_CommandReceive_Poll(void) 
{
  uint16_t index = 0;
  uint8_t len = 0;
	char* p = NULL;
  while(app_uart_get(&CmdRecBuf[index],SCREEN) == NRF_SUCCESS) 
  {
		DBG_LOG("test!");
#if 1		
    if(index == len + 8)
    {
      Uart_Protocol_Cmd_Analy(CmdRecBuf,index); 
    }else
		{
			if(index == 6)
				len = CmdRecBuf[index];			
		  index++;
      delay_ms(2);			
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
		if (CmdRecBuf[index] == '\n' && CmdRecBuf[index - 1] == '\r') 
		{
			CmdRecBuf[index+1] = '\0';
			p = (char*)&CmdRecBuf[0];
			strcpy((char*)g_bar_code,p);
			errorDef.bar_code_state = 1;
		//	DBG_LOG("data is %s",p);
			//send date
			for(i=1;i<=3;i++) {
				dat[7] = CmdRecBuf[0];
				dat[8] = CmdRecBuf[1];
				dat[9] = CmdRecBuf[2];
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

void Uart_Protocol_Cmd_Analy(uint8_t* CmdRecBuf,uint8_t length)
{
		static uint8_t tmp_dat, i = 0;
    if(CmdRecBuf[0] == FHEADER && CmdRecBuf[length] == FEND)
    {
        switch(CmdRecBuf[1])
				{
					case CMD_TIME:
								Get_Time(&CmdRecBuf[2]);
						break;
					case CMD_MOTO:
								for(i=0;i<=11;i++) {
									DBG_LOG("CmdRecBuf[%d] = %02x",i,CmdRecBuf[i]);
								}
								tmp_dat = CmdRecBuf[7];
								DBG_LOG("tmp_dat = %02x",tmp_dat);
								DBG_LOG("data is %d",CmdRecBuf[7]);
                Get_Mote_Data(&tmp_dat);
						break;	
					case CMD_LOCK:
								Get_Lock_Data(&CmdRecBuf[7]);
								DBG_LOG("data is %x",CmdRecBuf[7]);
								// Uart_Send_Data(SCREEN, dat_tmp,sizeof(dat_tmp));
								//send_back(tmp);
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
   

// void send_back(uint8_t *dat) {
// 		uint8_t dat_tmp[11] = {0x7E,0x82,0x00,0x01,0x02,0x03,0x01,0x01,0x00,0x00,0x7E};
// 		dat_tmp[1] = dat;
// 		Uart_Send_Data(SCREEN, dat_tmp,sizeof(dat_tmp));
// }