#include "moto.h"
#include <stdio.h>
#include "delay.h"
Machine machine = {0,0,0};
Moto motoDef = {Open_xMoto,Close_xMoto,Read_xMoto,0,0};
mError errorDef = {0,0};

mPin Pin_Array[PINMAX] = 
	{
		MOTO(1), MOTO(2), MOTO(3), MOTO(4), MOTO(5), MOTO(6), MOTO(7), MOTO(8), MOTO(9), MOTO(10),
		MOTO(11),MOTO(12),MOTO(13),MOTO(14),MOTO(15),MOTO(16),MOTO(17),MOTO(18),MOTO(19),MOTO(20),
		MOTO(21),MOTO(22),MOTO(23),MOTO(24),MOTO(25),MOTO(26),MOTO(27),MOTO(28),MOTO(29),MOTO(30),
		MOTO(31),MOTO(32),MOTO(33),MOTO(34),MOTO(35),MOTO(36),MOTO(37),MOTO(38),MOTO(39),MOTO(40),
		MOTO(41),MOTO(42),MOTO(43),MOTO(44),MOTO(45),MOTO(46),MOTO(47),MOTO(48),MOTO(49),MOTO(50),
		MOTO(51),MOTO(52),MOTO(53),MOTO(54),MOTO(56),MOTO(57),
		DEM(33), DEM(34) ,DEM(35) ,DEM(36), DEM(37), DEM(38), DEM(39), //63
		DEM(40), DEM(41) ,DEM(42) ,DEM(43), DEM(44), DEM(45), DEM(46),DEM(47),DEM(48),DEM(49),
		DEM(50), DEM(51) ,DEM(52) ,DEM(53), DEM(54), DEM(55), DEM(56),DEM(57)
	};
	
void Moto_Init()
{
	uint8_t i = 0;
  GPIO_InitTypeDef GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOF|RCC_APB2Periph_GPIOG|RCC_APB2Periph_AFIO,ENABLE);
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); 
	/*出货履带58*/
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10|GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOC,GPIO_Pin_10);	
	GPIO_ResetBits(GPIOC,GPIO_Pin_11);
		/*出货履带检测*/
  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	for(i = 0;i < PINMAX;i++)
	{
		GPIO_InitStructure.GPIO_Pin  = Pin_Array[i].pin;
		GPIO_InitStructure.GPIO_Mode = Pin_Array[i].mode;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
		GPIO_Init(Pin_Array[i].port, &GPIO_InitStructure);
		GPIO_ResetBits(Pin_Array[i].port,Pin_Array[i].pin);
	}
}

uint8_t Open_xMoto(uint8_t num)
{
	if(num < 1 || num > 56)
		return 1;
  GPIO_SetBits(Pin_Array[num-1].port,Pin_Array[num-1].pin); 
  return 0;		
}

uint8_t Close_xMoto(uint8_t num)
{
	if(num < 1 || num > 56)
		return 1;
  GPIO_ResetBits(Pin_Array[num-1].port,Pin_Array[num-1].pin);
  return 0;		
}

uint8_t Read_xMoto(uint8_t num)
{
	if(num < 56 || num > 81)
		return 0xff;

	return GPIO_ReadInputDataBit(Pin_Array[num-1].port,Pin_Array[num-1].pin);
}


uint8_t Check_Moto(uint8_t num)
{
	static uint8_t checkflag = 0;
  if(motoDef.read_moto(num) == 0xff)
	{
     return 1;
	}else if(motoDef.read_moto(num))
	{
		delay_ms(5);
		if(motoDef.read_moto(num))
			checkflag = 1;
	}else if(checkflag && !motoDef.read_moto(num))
	{
		
		delay_ms(5);
		if(!motoDef.read_moto(num))
		{
			checkflag = 0;
			return 0;
		}
	}
	return 1;
}

uint8_t Set_Moto()
{
  if(motoDef.num)
	{
	  if(!motoDef.open_moto(motoDef.num))
      return motoDef.num;
		else
			return 0xff;
	}
	return 0;
}
