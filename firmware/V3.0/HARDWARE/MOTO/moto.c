#include "moto.h"
#include <stdio.h>
#include "delay.h"
#include "prjlib.h"
#include "usart.h"
#include "wdg.h"

Machine machine = {0, 0, 0};
Moto motoDef = {Open_xMoto, Close_xMoto,       Read_xMoto, 0,0,
                0,          get_moto_feetback, open_lock};
mError errorDef = {0, 0};
extern uint16_t calc_times;
extern uint8_t moto_num_carry[32];
extern uint8_t flag_calc_times;
extern uint16_t delay_time;

mPin Pin_Array[PINMAX] = {
    // borrow motor array number:0-31
    MOTO(1),  MOTO(2),  MOTO(3),  MOTO(4),  MOTO(5),  MOTO(6),  MOTO(7),
    MOTO(8),  MOTO(9),  MOTO(10), MOTO(11), MOTO(12), MOTO(13), MOTO(14),
    MOTO(15), MOTO(16), MOTO(17), MOTO(18), MOTO(19), MOTO(20), MOTO(21),
    MOTO(22), MOTO(23), MOTO(24), MOTO(25), MOTO(26), MOTO(27), MOTO(28),
    MOTO(29), MOTO(30), MOTO(31), MOTO(32),
};

mPin Pin_Array_FEEDBACK[FEETBACK_MAX] = {
    MOTO_FEEDBACK(1),  MOTO_FEEDBACK(2),  MOTO_FEEDBACK(3),  MOTO_FEEDBACK(4),
    MOTO_FEEDBACK(5),  MOTO_FEEDBACK(6),  MOTO_FEEDBACK(7),  MOTO_FEEDBACK(8),
    MOTO_FEEDBACK(9),  MOTO_FEEDBACK(10), MOTO_FEEDBACK(11), MOTO_FEEDBACK(12),
    MOTO_FEEDBACK(13), MOTO_FEEDBACK(14), MOTO_FEEDBACK(15), MOTO_FEEDBACK(16),
    MOTO_FEEDBACK(17), MOTO_FEEDBACK(18), MOTO_FEEDBACK(19), MOTO_FEEDBACK(20),
    MOTO_FEEDBACK(21), MOTO_FEEDBACK(22), MOTO_FEEDBACK(23), MOTO_FEEDBACK(24),
    MOTO_FEEDBACK(25), MOTO_FEEDBACK(26), MOTO_FEEDBACK(27), MOTO_FEEDBACK(28),
    MOTO_FEEDBACK(29), MOTO_FEEDBACK(30), MOTO_FEEDBACK(31), MOTO_FEEDBACK(32),
};

mPin Pin_Array_LOCK[LOCK_MAX] = {
    LOCK(1),  LOCK(2),  LOCK(3),  LOCK(4),  LOCK(5),  LOCK(6),
    LOCK(7),  LOCK(8),  LOCK(9),  LOCK(10), LOCK(11), LOCK(12),
    LOCK(13), LOCK(14), LOCK(15), LOCK(16), LOCK(17), LOCK(18),
    LOCK(19), LOCK(20), LOCK(21), LOCK(22),
};

void Moto_Init() {
  uint8_t i = 0;
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |
                             RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD |
                             RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOF |
                             RCC_APB2Periph_GPIOG | RCC_APB2Periph_AFIO,
                         ENABLE);
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);

  /*出货履带58*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_ResetBits(GPIOC, GPIO_Pin_10);
  GPIO_ResetBits(GPIOC, GPIO_Pin_11);

  /*出货履带检测*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  // logo light gpio init && sound control
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  // other light 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOG, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;


  // sound control
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  // push motor gpio init begin
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  // electromagnetic lock
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  //	infrared sensor init gpio
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  //	switch to open all the repy lock
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  for (i = 0; i < PINMAX; i++) {
    GPIO_InitStructure.GPIO_Pin = Pin_Array[i].pin;
    GPIO_InitStructure.GPIO_Mode = Pin_Array[i].mode;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(Pin_Array[i].port, &GPIO_InitStructure);
    GPIO_ResetBits(Pin_Array[i].port, Pin_Array[i].pin);
  }

  for (i = 0; i < FEETBACK_MAX; i++) {
    GPIO_InitStructure.GPIO_Pin = Pin_Array_FEEDBACK[i].pin;
    GPIO_InitStructure.GPIO_Mode = Pin_Array_FEEDBACK[i].mode;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(Pin_Array_FEEDBACK[i].port, &GPIO_InitStructure);
    GPIO_ResetBits(Pin_Array_FEEDBACK[i].port, Pin_Array_FEEDBACK[i].pin);
  }

  for (i = 0; i < LOCK_MAX; i++) {
    GPIO_InitStructure.GPIO_Pin = Pin_Array_LOCK[i].pin;
    GPIO_InitStructure.GPIO_Mode = Pin_Array_LOCK[i].mode;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(Pin_Array_LOCK[i].port, &GPIO_InitStructure);
    GPIO_ResetBits(Pin_Array_LOCK[i].port, Pin_Array_LOCK[i].pin);
  }

  // MicroStep Motor 1 2 PUL-
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4;  // PUL1-  PUL2-
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  // MicroStep Motor 1 2 EN DIR
  GPIO_InitStructure.GPIO_Pin =
      GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;  // EN1- DIR1- EN2-
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  // MicroStep Motor 2 DIR2-
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;  // DIR2-
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  //	tour switch
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOF, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  // notch sensor
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;  // PUL1-  PUL2-
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  // notch sensor
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;  // PUL1-  PUL2-
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
  GPIO_Init(GPIOF, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
}

/**
 * open motor  success return 0 not success return 1
 * @param argc
 * @param argv
 */
uint8_t Open_xMoto(uint8_t num) {
  if (num < 1 || num > 56) return 1;
  GPIO_SetBits(Pin_Array[num - 1].port, Pin_Array[num - 1].pin);
  return 0;
}

/**
 * close motor success return 0 not success return 1
 * @param argc
 * @param argv
 */
uint8_t Close_xMoto(uint8_t num) {
  if (num < 1 || num > 56) return 1;
  GPIO_ResetBits(Pin_Array[num - 1].port, Pin_Array[num - 1].pin);
  return 0;
}

uint8_t get_moto_feetback(uint8_t num) {
  uint8_t bit;
  if ((num < 1) || num > 32) return 0;
  bit = GPIO_ReadInputDataBit(Pin_Array_FEEDBACK[num - 1].port,
                              Pin_Array_FEEDBACK[num - 1].pin);
  return bit;
}

/**
 * open motor  success return 0 not success return 1
 * @param argc
 * @param argv
 */
uint8_t close_lock(uint8_t num) {
  if (num < 1 || num > 56) return 0;
  GPIO_ResetBits(Pin_Array_LOCK[num - 1].port, Pin_Array_LOCK[num - 1].pin);
}

/**
 * close motor success return 0 not success return 1
 * @param argc
 * @param argv
 */
uint8_t open_lock(uint8_t num) {
  if (num < 1 || num > 56) return 0;
  GPIO_SetBits(Pin_Array_LOCK[num - 1].port, Pin_Array_LOCK[num - 1].pin);
}

/**
 * read motor state
 * @param argc
 * @param argv
 */
uint8_t Read_xMoto(uint8_t num) {
  // if(num < 20 || num > 81)
  // 	return 0xff;
  return GPIO_ReadInputDataBit(Pin_Array[num - 1].port, Pin_Array[num - 1].pin);
}

/**
 * open motor
 * @param argc
 * @param argv
 */
uint8_t Set_Moto() {
  if (motoDef.num) {
    if (!motoDef.open_moto(motoDef.num)) {
      return motoDef.num;
    } else {
      return 0xff;
    }
  }
  return 0;
}

uint8_t MicroStep_Motro(uint32_t Step) {
  for (uint32_t i = 0; i <= Step; i++) {
    for (uint32_t j = 0; j <= 100; j++) {
      delay(800);
      GPIO_SetBits(GPIOB, GPIO_Pin_3);
      GPIO_SetBits(GPIOB, GPIO_Pin_4);
      delay(800);
      GPIO_ResetBits(GPIOB, GPIO_Pin_3);
      GPIO_ResetBits(GPIOB, GPIO_Pin_4);
    }
  }
  return 1;
}

void MicroStep_Motro_init(uint32_t Step) {
  if (NORCH_SENSOR_A_MOTO == 1) {
    delay(1000);
    GPIO_SetBits(GPIOB, GPIO_Pin_3);
    GPIO_SetBits(GPIOB, GPIO_Pin_4);
    delay(1000);
    GPIO_ResetBits(GPIOB, GPIO_Pin_3);
    GPIO_ResetBits(GPIOB, GPIO_Pin_4);
  }
}

uint8_t init_moto(void) {
  uint32_t sent_over_38000 = 0;
  // steper_moto_out();
  // MotorSetpperMove(300);
  // delay_ms_whx(100);
  GPIO_SetBits(GPIOC, GPIO_Pin_10);  // EN1
  GPIO_SetBits(GPIOC, GPIO_Pin_12);  // EN2
  steper_moto_in();
  calc_times = 0;
  sent_over_38000 = 0; 
  while (calc_times <= 20) {
      sent_over_38000++;
      IWDG_Feed();
      delay(600);
      GPIO_SetBits(GPIOB, GPIO_Pin_3);
      GPIO_SetBits(GPIOB, GPIO_Pin_4);
      delay(600);
      GPIO_ResetBits(GPIOB, GPIO_Pin_3);
      GPIO_ResetBits(GPIOB, GPIO_Pin_4);
      if(sent_over_38000 >= SENT_OVER_38000) break;
  }
  return 1;
}
typedef enum {
  motor_start,
  motor_start_fast,
  motor_fast,
  motor_slowdown,
  motor_slow,
  motor_stop
} MotorStatusEnum;

typedef enum { start, half, quoater, before } plus_iX;

/*speed up */
void MotorSetpperMove(uint32_t xstep) {
  uint32_t iX = 0;
  uint32_t plusX = MOTOR_X_START_PLUS;
  uint32_t ipX = 0;
  MotorStatusEnum statusX = motor_start;

  xstep *= 2;

  GPIO_SetBits(GPIOB, GPIO_Pin_3);
  GPIO_SetBits(GPIOB, GPIO_Pin_4);

  while (iX < xstep) {
    delay(1);

    // X轴输出脉冲
    if (ipX++ >= plusX && iX < xstep) {
      ipX = 0;
      iX++;
      if ((iX % 2) == 0) {
        GPIO_ResetBits(GPIOB, GPIO_Pin_3);
        GPIO_ResetBits(GPIOB, GPIO_Pin_4);
      } else {
        GPIO_SetBits(GPIOB, GPIO_Pin_3);
        GPIO_SetBits(GPIOB, GPIO_Pin_4);
      }
      if ((iX % 2) == 0) {
        switch (statusX) {
          case motor_start:
            plusX = MOTOR_X_START_PLUS;
            // 100个脉冲
            if (iX >= 1000) {
              statusX = motor_start_fast;
            }
            break;
          case motor_start_fast:
            if (plusX > 200) {
              plusX--;
            } else if (iX % 100 == 0) {
              plusX--;
            }
            if (plusX <= MOTOR_X_FAST_PLUS) {
              plusX = MOTOR_X_FAST_PLUS;
              statusX = motor_fast;
            }
            break;
          case motor_fast:
            break;
          case motor_slowdown:
            if (iX % 50 == 0) {
              plusX++;
            }
            if (plusX >= MOTOR_X_START_PLUS) {
              plusX = MOTOR_X_START_PLUS;
              statusX = motor_slow;
            }
            break;
          case motor_slow:
            break;
          case motor_stop:
            break;
          default:
            break;
        }
        IWDG_Feed();
        if (iX > 50000) {
          statusX = motor_slowdown;
        }
      }
    }
  }
}

void steper_moto_in(void) {  
  GPIO_ResetBits(GPIOD, GPIO_Pin_0);
  GPIO_ResetBits(GPIOC, GPIO_Pin_11);
}

void steper_moto_out(void) {
  GPIO_SetBits(GPIOD, GPIO_Pin_0);
  GPIO_SetBits(GPIOC, GPIO_Pin_11);
}