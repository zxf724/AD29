#ifndef _MOTO_H
#define _MOTO_H

#include "sys.h"

#define SUCCESS 1
#define FAIL 0

#define PINMAX 32
#define FEETBACK_MAX  32

// check   tail of the cargo road
#define CHECK_TRACK_1 55
#define CHECK_TRACK_2 79
#define CHECK_TRACK_3 80
#define CHECK_TRACK_4 81

// define moto num
#define IN_PLUSE_NUM 50
#define MAX_IN_PLUSE_NUM 100

#define TEST 0

// moto speed
#define MOTOR_X_SPEED_SLOWDOWN_COUNT 1000

#define MOTOR_X_START_PLUS 900
#define MOTOR_X_FAST_PLUS 80
// infrared
#define INFRARED_SENSOR_TEST 78

#define CHECK_CARGO 80
#define CHECK_CARGO_DOOR 79
#define ONE_STEP 1000

#define MOTO(x) MOTO_##x
#define LOCK(x) LOCK_##x
#define MOTO_FEEDBACK(x) MOTO_FEEDBACK_##x
#define INFRARED_SENSOR(x) INFRARED_SENSOR_##x  // infrared sensor
#define PUSH_MOTOR(x) PUSH_MOTOR_##x

// borrow motor
#define MOTO_1 \
  { GPIOE, GPIO_Pin_2, GPIO_Mode_Out_PP }
#define MOTO_2 \
  { GPIOE, GPIO_Pin_3, GPIO_Mode_Out_PP }
#define MOTO_3 \
  { GPIOE, GPIO_Pin_4, GPIO_Mode_Out_PP }
#define MOTO_4 \
  { GPIOE, GPIO_Pin_5, GPIO_Mode_Out_PP }
#define MOTO_5 \
  { GPIOE, GPIO_Pin_6, GPIO_Mode_Out_PP }
#define MOTO_6 \
  { GPIOC, GPIO_Pin_13, GPIO_Mode_Out_PP }
#define MOTO_7 \
  { GPIOF, GPIO_Pin_0, GPIO_Mode_Out_PP }
#define MOTO_8 \
  { GPIOF, GPIO_Pin_1, GPIO_Mode_Out_PP }
#define MOTO_9 \
  { GPIOF, GPIO_Pin_2, GPIO_Mode_Out_PP }
#define MOTO_10 \
  { GPIOF, GPIO_Pin_3, GPIO_Mode_Out_PP }

#define MOTO_11 \
  { GPIOF, GPIO_Pin_4, GPIO_Mode_Out_PP }
#define MOTO_12 \
  { GPIOF, GPIO_Pin_5, GPIO_Mode_Out_PP }
#define MOTO_13 \
  { GPIOF, GPIO_Pin_6, GPIO_Mode_Out_PP }
#define MOTO_14 \
  { GPIOF, GPIO_Pin_7, GPIO_Mode_Out_PP }
#define MOTO_15 \
  { GPIOF, GPIO_Pin_8, GPIO_Mode_Out_PP }
#define MOTO_16 \
  { GPIOF, GPIO_Pin_9, GPIO_Mode_Out_PP }
#define MOTO_17 \
  { GPIOF, GPIO_Pin_10, GPIO_Mode_Out_PP }
#define MOTO_18 \
  { GPIOC, GPIO_Pin_0, GPIO_Mode_Out_PP }
#define MOTO_19 \
  { GPIOC, GPIO_Pin_1, GPIO_Mode_Out_PP }
#define MOTO_20 \
  { GPIOC, GPIO_Pin_2, GPIO_Mode_Out_PP }

#define MOTO_21 \
  { GPIOC, GPIO_Pin_3, GPIO_Mode_Out_PP }
#define MOTO_22 \
  { GPIOA, GPIO_Pin_0, GPIO_Mode_Out_PP }
#define MOTO_23 \
  { GPIOA, GPIO_Pin_1, GPIO_Mode_Out_PP }
#define MOTO_24 \
  { GPIOA, GPIO_Pin_4, GPIO_Mode_Out_PP }
#define MOTO_25 \
  { GPIOA, GPIO_Pin_5, GPIO_Mode_Out_PP }
#define MOTO_26 \
  { GPIOA, GPIO_Pin_6, GPIO_Mode_Out_PP }
#define MOTO_27 \
  { GPIOA, GPIO_Pin_7, GPIO_Mode_Out_PP }
#define MOTO_28 \
  { GPIOC, GPIO_Pin_4, GPIO_Mode_Out_PP }
#define MOTO_29 \
  { GPIOC, GPIO_Pin_5, GPIO_Mode_Out_PP }
#define MOTO_30 \
  { GPIOB, GPIO_Pin_0, GPIO_Mode_Out_PP }

#define MOTO_31 \
  { GPIOB, GPIO_Pin_1, GPIO_Mode_Out_PP }
#define MOTO_32 \
  { GPIOB, GPIO_Pin_2, GPIO_Mode_Out_PP }

// MOTO_FEEDBACK schematic 1-32
#define MOTO_FEEDBACK_1 \
  { GPIOD, GPIO_Pin_10, GPIO_Mode_Out_PP }  // DEM1
#define MOTO_FEEDBACK_2 \
  { GPIOD, GPIO_Pin_11, GPIO_Mode_Out_PP }  // DEM2
#define MOTO_FEEDBACK_3 \
  { GPIOD, GPIO_Pin_12, GPIO_Mode_Out_PP }  // DEM3
#define MOTO_FEEDBACK_4 \
  { GPIOD, GPIO_Pin_13, GPIO_Mode_Out_PP }  // DEM4
#define MOTO_FEEDBACK_5 \
  { GPIOD, GPIO_Pin_14, GPIO_Mode_Out_PP }  // DEM5
#define MOTO_FEEDBACK_6 \
  { GPIOD, GPIO_Pin_15, GPIO_Mode_Out_PP }  // DEM6
#define MOTO_FEEDBACK_7 \
  { GPIOG, GPIO_Pin_2, GPIO_Mode_Out_PP }  // DEM7
#define MOTO_FEEDBACK_8 \
  { GPIOG, GPIO_Pin_3, GPIO_Mode_Out_PP }  // DEM8
#define MOTO_FEEDBACK_9 \
  { GPIOG, GPIO_Pin_4, GPIO_Mode_Out_PP }  // DEM9
#define MOTO_FEEDBACK_10 \
  { GPIOG, GPIO_Pin_5, GPIO_Mode_Out_PP }  // DEM10

#define MOTO_FEEDBACK_11 \
  { GPIOG, GPIO_Pin_6, GPIO_Mode_Out_PP }  // DEM11
#define MOTO_FEEDBACK_12 \
  { GPIOG, GPIO_Pin_7, GPIO_Mode_Out_PP }  // DEM12
#define MOTO_FEEDBACK_13 \
  { GPIOD, GPIO_Pin_8, GPIO_Mode_Out_PP }  // DEM13
#define MOTO_FEEDBACK_14 \
  { GPIOC, GPIO_Pin_6, GPIO_Mode_Out_PP }  // DEM14
#define MOTO_FEEDBACK_15 \
  { GPIOC, GPIO_Pin_7, GPIO_Mode_Out_PP }  // DEM15
#define MOTO_FEEDBACK_16 \
  { GPIOC, GPIO_Pin_8, GPIO_Mode_Out_PP }  // DEM16
#define MOTO_FEEDBACK_17 \
  { GPIOC, GPIO_Pin_9, GPIO_Mode_Out_PP }  // DEM17
#define MOTO_FEEDBACK_18 \
  { GPIOA, GPIO_Pin_8, GPIO_Mode_Out_PP }  // DEM18
#define MOTO_FEEDBACK_19 \
  { GPIOA, GPIO_Pin_8, GPIO_Mode_Out_PP }  // DEM19 -- DEM18
#define MOTO_FEEDBACK_20 \
  { GPIOA, GPIO_Pin_15, GPIO_Mode_Out_PP }  // DEM20

#define MOTO_FEEDBACK_21 \
  { GPIOD, GPIO_Pin_1, GPIO_Mode_Out_PP }  // DEM21
#define MOTO_FEEDBACK_22 \
  { GPIOD, GPIO_Pin_2, GPIO_Mode_Out_PP }  // DEM22
#define MOTO_FEEDBACK_23 \
  { GPIOD, GPIO_Pin_3, GPIO_Mode_Out_PP }  // DEM23
#define MOTO_FEEDBACK_24 \
  { GPIOD, GPIO_Pin_6, GPIO_Mode_Out_PP }  // DEM24
#define MOTO_FEEDBACK_25 \
  { GPIOD, GPIO_Pin_7, GPIO_Mode_Out_PP }  // DEM25
#define MOTO_FEEDBACK_26 \
  { GPIOG, GPIO_Pin_9, GPIO_Mode_Out_PP }  // DEM26
#define MOTO_FEEDBACK_27 \
  { GPIOG, GPIO_Pin_10, GPIO_Mode_Out_PP }  // DEM27
#define MOTO_FEEDBACK_28 \
  { GPIOG, GPIO_Pin_11, GPIO_Mode_Out_PP }  // DEM28
#define MOTO_FEEDBACK_29 \
  { GPIOG, GPIO_Pin_12, GPIO_Mode_Out_PP }  // DEM29
#define MOTO_FEEDBACK_30 \
  { GPIOG, GPIO_Pin_13, GPIO_Mode_Out_PP }  // DEM30

#define MOTO_FEEDBACK_31 \
  { GPIOG, GPIO_Pin_14, GPIO_Mode_Out_PP }  // DEM31
#define MOTO_FEEDBACK_32 \
  { GPIOB, GPIO_Pin_5, GPIO_Mode_Out_PP }  // DEM32

// infrared sensor
#define INFRARED_SENSOR_1 \
  { GPIOD, GPIO_Pin_4, GPIO_Mode_IPD }

#define MOTO_58_RUN_LEFT \
  PCout(10) = 1;         \
  PCout(11) = 0
#define MOTO_58_RUN_RIGHT \
  PCout(10) = 0;          \
  PCout(11) = 1

#define READ_MOTO_58 PCin(12)

// push mortor  left = in; right = out
#define PUSH_MOTOR_LEFT                 \
  {                                     \
    GPIO_SetBits(GPIOC, GPIO_Pin_10);   \
    GPIO_ResetBits(GPIOC, GPIO_Pin_11); \
  }  // GPIO_SetBits()  //GPIO_ResetBits()
#define PUSH_MOTOR_RIGHT                \
  {                                     \
    GPIO_SetBits(GPIOC, GPIO_Pin_11);   \
    GPIO_ResetBits(GPIOC, GPIO_Pin_10); \
  }
#define PUSH_MOTOR_CLEAR                \
  {                                     \
    GPIO_ResetBits(GPIOC, GPIO_Pin_11); \
    GPIO_ResetBits(GPIOC, GPIO_Pin_10); \
  }

// electromagnetic lock
#define OPEN_ELECTRIC_LOCK GPIO_SetBits(GPIOB, GPIO_Pin_7)
#define CLOSE_ELECTRIC_LOCK GPIO_ResetBits(GPIOB, GPIO_Pin_7)

#define TOUR_SWITCH GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_5)
#define NEW_SENSOR GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_4)

enum {
  state_stop = 0,
  state_repay,
  state_borrow,
  state_run_first,
  state_error,
  state_run_second,
  state_run_second_half,
  state_run_third,
  //   state_run_fifth,
  state_report,
  state_gun_open,
  state_door_open
  // new state
};

/*motor Structure
 * open_moto£º open motor function
 * close_moto£º close motor function
 * read_moto£º  read motor function
 * num£º the motor number
 * state£ºrunning state
 */
typedef struct {
  uint8_t (*open_moto)(uint8_t);
  uint8_t (*close_moto)(uint8_t);
  uint8_t (*read_moto)(uint8_t);
  uint8_t num;
  uint8_t state;
  uint8_t (*get_moto_feetback)(uint8_t);
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
typedef struct {
  uint8_t android_state;
  uint8_t error_count;
  uint8_t bar_code_state;
} mError;

/*new data*/
typedef struct {
  GPIO_TypeDef* port;
  uint16_t pin;
  GPIOMode_TypeDef mode;
} mPin;

void Moto_Init(void);
void sound_control(void);
uint8_t Open_xMoto(uint8_t num);
uint8_t Close_xMoto(uint8_t num);
uint8_t Read_xMoto(uint8_t num);
uint8_t get_moto_feetback(uint8_t num);
uint8_t Set_Moto(void);
uint8_t Set_Lock(void);
uint8_t MicroStep_Motro(uint32_t Step);
uint8_t check_num(void);
uint8_t init_moto(void);
void MotorSetpperMove(uint32_t xstep);
void MicroStep_Motro_init(uint32_t Step);
#endif
