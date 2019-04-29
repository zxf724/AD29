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

void Screen_CommandReceive_Poll(void) 
{
  uint16_t index = 0;
  uint8_t len = 0;
	 char* p = NULL;
  while(app_uart_get(&CmdRecBuf[index],SCREEN) == NRF_SUCCESS) 
  { 
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

  char* p = NULL;
	while(app_uart_get(&CmdRecBuf[index],GUN) == NRF_SUCCESS) 
  {  
		if (CmdRecBuf[index] == '\n' && CmdRecBuf[index - 1] == '\r') 
		{
			CmdRecBuf[index+1] = '\0';
			p = (char*)&CmdRecBuf[0];
			strcpy((char*)g_bar_code,p);
			errorDef.bar_code_state = 1;
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
    if(CmdRecBuf[0] == FHEADER && CmdRecBuf[length] == FEND)
    {
        switch(CmdRecBuf[1])
				{
					case CMD_TIME:
								Get_Time(&CmdRecBuf[2]);
						break;
					case CMD_MOTO:
                Get_Mote_Data(&CmdRecBuf[2]);
						break;	
					case CMD_LOCK:
								Get_Lock_Data(&CmdRecBuf[2]);
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
   
