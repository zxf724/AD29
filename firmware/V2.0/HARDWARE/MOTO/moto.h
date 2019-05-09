#ifndef _MOTO_H
#define _MOTO_H

#include "sys.h"

#define SUCCESS 1
#define FAIL    0

#define PINMAX       81

/*¼ì²âµôÂäµÄÒý½Å*/
#define CHECK_DROP   79

#define DOOR_CARGO   54
#define MOTO_CARGO   55

#define CHECK_CARGO       80
#define CHECK_CARGO_DOOR  79

#define MOTO(x) MOTO_##x
#define DEM(x)  DEM_##x

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

#define MOTO_33 {GPIOF,GPIO_Pin_11,GPIO_Mode_Out_PP}
#define MOTO_34 {GPIOF,GPIO_Pin_12,GPIO_Mode_Out_PP}
#define MOTO_35 {GPIOF,GPIO_Pin_15,GPIO_Mode_Out_PP}

#define MOTO_36 {GPIOG,GPIO_Pin_0,GPIO_Mode_Out_PP}

#define MOTO_37 {GPIOE,GPIO_Pin_8,GPIO_Mode_Out_PP}
#define MOTO_38 {GPIOE,GPIO_Pin_9,GPIO_Mode_Out_PP}
#define MOTO_39 {GPIOE,GPIO_Pin_12,GPIO_Mode_Out_PP}
#define MOTO_40 {GPIOE,GPIO_Pin_13,GPIO_Mode_Out_PP}

#define MOTO_41 {GPIOB,GPIO_Pin_10,GPIO_Mode_Out_PP}
#define MOTO_42 {GPIOB,GPIO_Pin_11,GPIO_Mode_Out_PP}
#define MOTO_43 {GPIOB,GPIO_Pin_14,GPIO_Mode_Out_PP}
#define MOTO_44 {GPIOB,GPIO_Pin_15,GPIO_Mode_Out_PP}

#define MOTO_45 {GPIOD,GPIO_Pin_10,GPIO_Mode_Out_PP}
#define MOTO_46 {GPIOD,GPIO_Pin_11,GPIO_Mode_Out_PP}
#define MOTO_47 {GPIOD,GPIO_Pin_14,GPIO_Mode_Out_PP}
#define MOTO_48 {GPIOD,GPIO_Pin_15,GPIO_Mode_Out_PP}

#define MOTO_49 {GPIOG,GPIO_Pin_4,GPIO_Mode_Out_PP}
#define MOTO_50 {GPIOG,GPIO_Pin_5,GPIO_Mode_Out_PP}
#define MOTO_51 {GPIOG,GPIO_Pin_8,GPIO_Mode_Out_PP}

#define MOTO_52 {GPIOC,GPIO_Pin_6,GPIO_Mode_Out_PP}
#define MOTO_53 {GPIOC,GPIO_Pin_9,GPIO_Mode_Out_PP}

#define MOTO_54 {GPIOA,GPIO_Pin_8,GPIO_Mode_Out_PP}

#define MOTO_56 {GPIOD,GPIO_Pin_0,GPIO_Mode_Out_PP}
#define MOTO_57 {GPIOD,GPIO_Pin_1,GPIO_Mode_Out_PP}


#define DEM_33  {GPIOF,GPIO_Pin_13,GPIO_Mode_IPU}
#define DEM_34  {GPIOF,GPIO_Pin_14,GPIO_Mode_IPU}

#define DEM_35  {GPIOG,GPIO_Pin_1,GPIO_Mode_IPU}

#define DEM_36  {GPIOE,GPIO_Pin_7,GPIO_Mode_IPU}
#define DEM_37  {GPIOE,GPIO_Pin_10,GPIO_Mode_IPU}
#define DEM_38  {GPIOE,GPIO_Pin_11,GPIO_Mode_IPU}
#define DEM_39  {GPIOE,GPIO_Pin_14,GPIO_Mode_IPU}
#define DEM_40  {GPIOE,GPIO_Pin_15,GPIO_Mode_IPU}

#define DEM_41  {GPIOB,GPIO_Pin_12,GPIO_Mode_IPU}
#define DEM_42  {GPIOB,GPIO_Pin_13,GPIO_Mode_IPU}

#define DEM_43  {GPIOD,GPIO_Pin_8,GPIO_Mode_IPU}
#define DEM_44  {GPIOD,GPIO_Pin_9,GPIO_Mode_IPU}
#define DEM_45  {GPIOD,GPIO_Pin_12,GPIO_Mode_IPU}
#define DEM_46  {GPIOD,GPIO_Pin_13,GPIO_Mode_IPU}

#define DEM_47  {GPIOG,GPIO_Pin_2,GPIO_Mode_IPU}
#define DEM_48  {GPIOG,GPIO_Pin_3,GPIO_Mode_IPU}
#define DEM_49  {GPIOG,GPIO_Pin_6,GPIO_Mode_IPU}
#define DEM_50  {GPIOG,GPIO_Pin_7,GPIO_Mode_IPU}

#define DEM_51  {GPIOC,GPIO_Pin_7,GPIO_Mode_IPU}
#define DEM_52  {GPIOC,GPIO_Pin_8,GPIO_Mode_IPU}

#define DEM_53  {GPIOA,GPIO_Pin_11,GPIO_Mode_IPU}
#define DEM_54  {GPIOA,GPIO_Pin_12,GPIO_Mode_IPU}
#define DEM_55  {GPIOA,GPIO_Pin_15,GPIO_Mode_IPD}

#define DEM_56  {GPIOD,GPIO_Pin_2,GPIO_Mode_IPU}
#define DEM_57  {GPIOD,GPIO_Pin_3,GPIO_Mode_IPU}


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

