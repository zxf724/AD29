#ifndef _MOTO_H
#define _MOTO_H

#include "sys.h"

#define SUCCESS 1
#define FAIL    0

#define PINMAX	77//81

/*¼ì²âµôÂäµÄÒý½Å*/
#define CHECK_DROP   54

#define DOOR_CARGO   54
#define MOTO_CARGO   55

#define CHECK_CARGO       80
#define CHECK_CARGO_DOOR  79

#define MOTO(x) MOTO_##x
#define LOCK(x)	LOCK_##x
#define MOTO_FEEDBACK(x) MOTO_FEEDBACK_##x	
#define LOCK_FEEDBACK(x) LOCK_FEEDBACK_##x

//borrow motor
#define MOTO_1  {GPIOE,GPIO_Pin_2,GPIO_Mode_Out_PP}
#define MOTO_2  {GPIOE,GPIO_Pin_3,GPIO_Mode_Out_PP}
#define MOTO_3  {GPIOE,GPIO_Pin_4,GPIO_Mode_Out_PP}
#define MOTO_4  {GPIOE,GPIO_Pin_5,GPIO_Mode_Out_PP}
#define MOTO_5  {GPIOE,GPIO_Pin_6,GPIO_Mode_Out_PP}
#define MOTO_6  {GPIOC,GPIO_Pin_13,GPIO_Mode_Out_PP}
#define MOTO_7  {GPIOF,GPIO_Pin_0,GPIO_Mode_Out_PP}
#define MOTO_8  {GPIOF,GPIO_Pin_1,GPIO_Mode_Out_PP}
#define MOTO_9  {GPIOF,GPIO_Pin_2,GPIO_Mode_Out_PP}
#define MOTO_10 {GPIOF,GPIO_Pin_3,GPIO_Mode_Out_PP}

#define MOTO_11 {GPIOF,GPIO_Pin_4,GPIO_Mode_Out_PP}
#define MOTO_12 {GPIOF,GPIO_Pin_5,GPIO_Mode_Out_PP}
#define MOTO_13 {GPIOF,GPIO_Pin_6,GPIO_Mode_Out_PP}
#define MOTO_14 {GPIOF,GPIO_Pin_7,GPIO_Mode_Out_PP}
#define MOTO_15 {GPIOF,GPIO_Pin_8,GPIO_Mode_Out_PP}
#define MOTO_16 {GPIOF,GPIO_Pin_9,GPIO_Mode_Out_PP}
#define MOTO_17 {GPIOF,GPIO_Pin_10,GPIO_Mode_Out_PP}
#define MOTO_18 {GPIOC,GPIO_Pin_0,GPIO_Mode_Out_PP}
#define MOTO_19 {GPIOC,GPIO_Pin_1,GPIO_Mode_Out_PP}
#define MOTO_20 {GPIOC,GPIO_Pin_2,GPIO_Mode_Out_PP}

#define MOTO_21 {GPIOC,GPIO_Pin_3,GPIO_Mode_Out_PP}
#define MOTO_22 {GPIOA,GPIO_Pin_0,GPIO_Mode_Out_PP}
#define MOTO_23 {GPIOA,GPIO_Pin_1,GPIO_Mode_Out_PP}
#define MOTO_24 {GPIOA,GPIO_Pin_4,GPIO_Mode_Out_PP}
#define MOTO_25 {GPIOA,GPIO_Pin_5,GPIO_Mode_Out_PP}
#define MOTO_26 {GPIOA,GPIO_Pin_6,GPIO_Mode_Out_PP}
#define MOTO_27 {GPIOA,GPIO_Pin_7,GPIO_Mode_Out_PP}
#define MOTO_28 {GPIOC,GPIO_Pin_4,GPIO_Mode_Out_PP}
#define MOTO_29 {GPIOC,GPIO_Pin_5,GPIO_Mode_Out_PP}
#define MOTO_30 {GPIOB,GPIO_Pin_0,GPIO_Mode_Out_PP}

#define MOTO_31 {GPIOB,GPIO_Pin_1,GPIO_Mode_Out_PP}
#define MOTO_32 {GPIOB,GPIO_Pin_2,GPIO_Mode_Out_PP}

//LOCK1-22  --> schematic  MOTOR 33-54
#define LOCK_1	{GPIOF,GPIO_Pin_11,GPIO_Mode_Out_PP}	//33
#define LOCK_2  {GPIOF,GPIO_Pin_12,GPIO_Mode_Out_PP}	//34
#define LOCK_3  {GPIOF,GPIO_Pin_15,GPIO_Mode_Out_PP}	//35
#define LOCK_4  {GPIOG,GPIO_Pin_0,GPIO_Mode_Out_PP}		//36
#define LOCK_5  {GPIOE,GPIO_Pin_8,GPIO_Mode_Out_PP}		//37
#define LOCK_6  {GPIOE,GPIO_Pin_9,GPIO_Mode_Out_PP}		//38
#define LOCK_7  {GPIOE,GPIO_Pin_12,GPIO_Mode_Out_PP}	//39	
#define LOCK_8  {GPIOE,GPIO_Pin_13,GPIO_Mode_Out_PP}	//40
#define LOCK_9  {GPIOB,GPIO_Pin_10,GPIO_Mode_Out_PP}	//41
#define LOCK_10  {GPIOB,GPIO_Pin_11,GPIO_Mode_Out_PP}	//42

#define LOCK_11  {GPIOB,GPIO_Pin_14,GPIO_Mode_Out_PP}	//43
#define LOCK_12  {GPIOB,GPIO_Pin_15,GPIO_Mode_Out_PP}	//44	
#define LOCK_13  {GPIOD,GPIO_Pin_10,GPIO_Mode_Out_PP}	//45
#define LOCK_14  {GPIOD,GPIO_Pin_11,GPIO_Mode_Out_PP}	//46
#define LOCK_15  {GPIOD,GPIO_Pin_14,GPIO_Mode_Out_PP}	//47
#define LOCK_16  {GPIOD,GPIO_Pin_15,GPIO_Mode_Out_PP}	//48
#define LOCK_17  {GPIOG,GPIO_Pin_4,GPIO_Mode_Out_PP}	//49
#define LOCK_18  {GPIOG,GPIO_Pin_5,GPIO_Mode_Out_PP}	//50
#define LOCK_19  {GPIOG,GPIO_Pin_8,GPIO_Mode_Out_PP}	//51
#define LOCK_20  {GPIOC,GPIO_Pin_6,GPIO_Mode_Out_PP}	//52

#define LOCK_21  {GPIOC,GPIO_Pin_9,GPIO_Mode_Out_PP}	//53
#define LOCK_22  {GPIOA,GPIO_Pin_8,GPIO_Mode_Out_PP}	//54

// MOTO_FEEDBACK schematic 33-
#define LOCK_FEEDBACK_1 {GPIOF,GPIO_Pin_13,GPIO_Mode_Out_PP}  //DEM33
#define LOCK_FEEDBACK_2 {GPIOF,GPIO_Pin_14,GPIO_Mode_Out_PP}
#define LOCK_FEEDBACK_3 {GPIOG,GPIO_Pin_0,GPIO_Mode_Out_PP}
#define LOCK_FEEDBACK_4 {GPIOG,GPIO_Pin_1,GPIO_Mode_Out_PP}
#define LOCK_FEEDBACK_5 {GPIOE,GPIO_Pin_10,GPIO_Mode_Out_PP}
#define LOCK_FEEDBACK_6 {GPIOE,GPIO_Pin_11,GPIO_Mode_Out_PP}
#define LOCK_FEEDBACK_7 {GPIOE,GPIO_Pin_14,GPIO_Mode_Out_PP}
#define LOCK_FEEDBACK_8 {GPIOE,GPIO_Pin_15,GPIO_Mode_Out_PP}
#define LOCK_FEEDBACK_9 {GPIOB,GPIO_Pin_12,GPIO_Mode_Out_PP}
#define LOCK_FEEDBACK_10 {GPIOB,GPIO_Pin_13,GPIO_Mode_Out_PP}	//DEM42

#define LOCK_FEEDBACK_11 {GPIOD,GPIO_Pin_8,GPIO_Mode_Out_PP}	//DEM43
#define LOCK_FEEDBACK_12 {GPIOD,GPIO_Pin_9,GPIO_Mode_Out_PP}
#define LOCK_FEEDBACK_13 {GPIOD,GPIO_Pin_12,GPIO_Mode_Out_PP}
#define LOCK_FEEDBACK_14 {GPIOD,GPIO_Pin_13,GPIO_Mode_Out_PP}
#define LOCK_FEEDBACK_15 {GPIOG,GPIO_Pin_2,GPIO_Mode_Out_PP}
#define LOCK_FEEDBACK_16 {GPIOG,GPIO_Pin_3,GPIO_Mode_Out_PP}
#define LOCK_FEEDBACK_17 {GPIOG,GPIO_Pin_6,GPIO_Mode_Out_PP}
#define LOCK_FEEDBACK_18 {GPIOG,GPIO_Pin_7,GPIO_Mode_Out_PP}
#define LOCK_FEEDBACK_19 {GPIOC,GPIO_Pin_7,GPIO_Mode_Out_PP}
#define LOCK_FEEDBACK_20 {GPIOC,GPIO_Pin_8,GPIO_Mode_Out_PP}	//DEM52

#define LOCK_FEEDBACK_21 {GPIOA,GPIO_Pin_11,GPIO_Mode_Out_PP}	//DEM53
#define LOCK_FEEDBACK_22 {GPIOA,GPIO_Pin_12,GPIO_Mode_Out_PP}	//DEM54
//motor feedback
#define MOTO_FEEDBACK_1 {GPIOA,GPIO_Pin_15,GPIO_Mode_Out_PP}	//DEM55

#define MOTO_58_RUN_LEFT  	PCout(10) = 1;PCout(11) = 0						                
#define MOTO_58_RUN_RIGHT	  PCout(10) = 0;PCout(11) = 1

#define READ_MOTO_58        PCin(12)  

// push mortor
#define PUSH_MOTOR_LEFT  {GPIO_SetBits(GPIOC,GPIO_Pin_10);GPIO_ResetBits(GPIOC,GPIO_Pin_11)}   //GPIO_SetBits()  //GPIO_ResetBits()
#define PUSH_MOTOR_RIGHT {GPIO_SetBits(GPIOC,GPIO_Pin_11);GPIO_ResetBits(GPIOC,GPIO_Pin_10)}

//electromagnetic lock
#define OPEN_ELECTRIC_LOCK  GPIO_SetBits(GPIOC,GPIO_Pin_12)
#define CLOSE_ELECTRIC_LOCK GPIO_ResetBits(GPIOC,GPIO_Pin_12)

	
enum {
  state_stop = 0,
  state_repay,
  state_borrow,
  state_run_first,
  state_error,
  state_run_second,
  state_run_third,
  state_report,
  state_gun_open,
  state_door_open
  //new state
};

/*motor Structure
 * open_moto£º open motor function
 * close_moto£º close motor function
 * read_moto£º  read motor function
 * num£º the motor number
 * state£ºrunning state
*/
typedef struct {
	uint8_t(*open_moto)(uint8_t);
	uint8_t(*close_moto)(uint8_t);
	uint8_t(*read_moto)(uint8_t);
	uint8_t num;
	uint8_t state;
} Moto;

/*all the machine state Structure
 *moto_state:  borrow motor state
 *lock_state:  repay motor state
 *gun_state:   scan gun state
 *state: machine state
*/
typedef struct {
	uint8_t lock_state;
	uint8_t moto_state;
	uint8_t gun_state;
	uint8_t state;
} Machine;

/*error structure
 *android_state: android reapy state
 *error_count:  error count
 *bar_code_state: bar state
*/
typedef struct
{
	uint8_t android_state;
	uint8_t error_count;
	uint8_t bar_code_state;
}mError;

/*new data*/
typedef struct
{
	GPIO_TypeDef* port;
	uint16_t pin;
	GPIOMode_TypeDef mode;
}mPin;

void Moto_Init(void);
uint8_t Open_xMoto(uint8_t num);
uint8_t Close_xMoto(uint8_t num);
uint8_t Read_xMoto(uint8_t num);
uint8_t Set_Moto(void);
uint8_t Set_Lock(void);
uint8_t Check_Moto(uint8_t num);
#endif

