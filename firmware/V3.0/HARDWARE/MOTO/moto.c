#include "moto.h"
#include <stdio.h>
#include "delay.h"
#include "prjlib.h"
#include "usart.h"
#include "wdg.h"

Machine machine = {0, 0, 0};
Moto motoDef = {Open_xMoto, Close_xMoto, Read_xMoto, 0, 0};
mError errorDef = {0, 0};
extern uint8_t moto_num_carry[32];

mPin Pin_Array[PINMAX] = {
    // borrow motor array number:0-31
    MOTO(1),
    MOTO(2),
    MOTO(3),
    MOTO(4),
    MOTO(5),
    MOTO(6),
    MOTO(7),
    MOTO(8),
    MOTO(9),
    MOTO(10),
    MOTO(11),
    MOTO(12),
    MOTO(13),
    MOTO(14),
    MOTO(15),
    MOTO(16),
    MOTO(17),
    MOTO(18),
    MOTO(19),
    MOTO(20),
    MOTO(21),
    MOTO(22),
    MOTO(23),
    MOTO(24),
    MOTO(25),
    MOTO(26),
    MOTO(27),
    MOTO(28),
    MOTO(29),
    MOTO(30),
    MOTO(31),
    MOTO(32),
    // repay lock,array number: 32-53
    LOCK(1),
    LOCK(2),
    LOCK(3),
    LOCK(4),
    LOCK(5),
    LOCK(6),
    LOCK(7),
    LOCK(8),
    LOCK(9),
    LOCK(10),
    LOCK(11),
    LOCK(12),
    LOCK(13),
    LOCK(14),
    LOCK(15),
    LOCK(16),
    LOCK(17),
    LOCK(18),
    LOCK(19),
    LOCK(20),
    LOCK(21),
    LOCK(22),
    // borrow motor feedback signal, array num 54
    MOTO_FEEDBACK(1),
    // lock feedback signal, array num 55-76
    LOCK_FEEDBACK(1),
    LOCK_FEEDBACK(2),
    LOCK_FEEDBACK(3),
    LOCK_FEEDBACK(4),
    LOCK_FEEDBACK(5),
    LOCK_FEEDBACK(6),
    LOCK_FEEDBACK(7),
    LOCK_FEEDBACK(8),
    LOCK_FEEDBACK(9),
    LOCK_FEEDBACK(10),
    LOCK_FEEDBACK(11),
    LOCK_FEEDBACK(12),
    LOCK_FEEDBACK(13),
    LOCK_FEEDBACK(14),
    LOCK_FEEDBACK(15),
    LOCK_FEEDBACK(16),
    LOCK_FEEDBACK(17),
    LOCK_FEEDBACK(18),
    LOCK_FEEDBACK(19),
    LOCK_FEEDBACK(20),
    LOCK_FEEDBACK(21),
    LOCK_FEEDBACK(22),
    // infrared sensor init num is 77
    INFRARED_SENSOR(1),
    // add 3 lock feedbacks 78 79 80
    MOTO_FEEDBACK(2),
    MOTO_FEEDBACK(3),
    MOTO_FEEDBACK(4),
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
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
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
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
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
      delay(150);
      GPIO_SetBits(GPIOB, GPIO_Pin_3);
      GPIO_SetBits(GPIOB, GPIO_Pin_4);
      delay(150);
      GPIO_ResetBits(GPIOB, GPIO_Pin_3);
      GPIO_ResetBits(GPIOB, GPIO_Pin_4);
    }
  }
  return 1;
}

uint8_t init_moto(void) {
  static uint8_t j = 0;
  static uint8_t i = 0;
  enum { stop, open, close } switch_motor;
  // 如果检测到行程开关常闭，往外走，直到检测到行程开关断开。
  if (TOUR_SWITCH == 0) {
    switch_motor = close;
  }  // 如果检测到行程开关常开，往内走，直到检测到行程开关闭合。
  if (TOUR_SWITCH == 1) {
    switch_motor = open;
  }

  switch (switch_motor) {
    case close:
      //常闭，在里面那一段
      GPIO_SetBits(GPIOC, GPIO_Pin_10);  // EN1
      GPIO_SetBits(GPIOC,
                   GPIO_Pin_11);         // DIR1   GPIO_SetBits() -> out
                                         // GPIO_ResetBits() -> in
      GPIO_SetBits(GPIOC, GPIO_Pin_12);  // EN2
      GPIO_SetBits(GPIOD,
                   GPIO_Pin_0);  // DIR2   GPIO_SetBits() -> out
                                 // GPIO_ResetBits() -> in
      while (TOUR_SWITCH != 1) {
        MicroStep_Motro(1);
        i++;
        if (i >= MAX_IN_PLUSE_NUM) break;
      }
      //步数  in
      GPIO_ResetBits(GPIOD, GPIO_Pin_0);
      GPIO_ResetBits(GPIOC, GPIO_Pin_11);
      MicroStep_Motro(IN_PLUSE_NUM);
      switch_motor = stop;
      break;
    case open:
      //常开，在外面那一段
      GPIO_SetBits(GPIOC, GPIO_Pin_10);  // EN1
      GPIO_ResetBits(
          GPIOC,
          GPIO_Pin_11);  // DIR1   GPIO_SetBits() -> out  GPIO_ResetBits() -> in
      GPIO_SetBits(GPIOC, GPIO_Pin_12);  // EN2
      GPIO_ResetBits(
          GPIOD,
          GPIO_Pin_0);  // DIR2   GPIO_SetBits() -> out  GPIO_ResetBits() -> in
      while (TOUR_SWITCH != 0) {
        MicroStep_Motro(1);
        j++;
        if (j >= MAX_IN_PLUSE_NUM) break;
      }
      //步数
      GPIO_ResetBits(GPIOD, GPIO_Pin_0);
      GPIO_ResetBits(GPIOC, GPIO_Pin_11);
      MicroStep_Motro(IN_PLUSE_NUM);
      switch_motor = stop;
      break;
    case stop:
      break;
    default:
      break;
  }
  return 0;
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
  uint32_t iX = 0, iX_slow = xstep;
  uint32_t plusX = MOTOR_X_START_PLUS;
  uint32_t ipX = 0;
  MotorStatusEnum statusX = motor_start;

  xstep *= 2;

  if (xstep > MOTOR_X_SPEED_SLOWDOWN_COUNT) {
    iX_slow = xstep - MOTOR_X_SPEED_SLOWDOWN_COUNT;
  }

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
            if (plusX > 50) {
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
        if (iX > 80000) {
          statusX = motor_slowdown;
        }
      }
    }
  }
}
