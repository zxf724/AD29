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

/*正常S型曲线参数生成的表格*/
uint16_t Motor1TimeTable[2 * (STEP_AA + STEP_UA + STEP_RA) + 1] = {0};
uint16_t Motor1StepTable[2 * (STEP_AA + STEP_UA + STEP_RA) + 1] = {0};
uint16_t Motor2TimeTable[2 * (STEP_AA + STEP_UA + STEP_RA) + 1] = {0};
uint16_t Motor2StepTable[2 * (STEP_AA + STEP_UA + STEP_RA) + 1] = {0};
uint16_t Motor3TimeTable[2 * (STEP_AA + STEP_UA + STEP_RA) + 1] = {0};
uint16_t Motor3StepTable[2 * (STEP_AA + STEP_UA + STEP_RA) + 1] = {0};

/*将参数降为2/3 S型曲线参数生成的表格*/
uint16_t Motor1_23TimeTable[2 * (STEP_AA + STEP_UA + STEP_RA) + 1] = {0};
uint16_t Motor1_23StepTable[2 * (STEP_AA + STEP_UA + STEP_RA) + 1] = {0};
uint16_t Motor2_23TimeTable[2 * (STEP_AA + STEP_UA + STEP_RA) + 1] = {0};
uint16_t Motor2_23StepTable[2 * (STEP_AA + STEP_UA + STEP_RA) + 1] = {0};
uint16_t Motor3_23TimeTable[2 * (STEP_AA + STEP_UA + STEP_RA) + 1] = {0};
uint16_t Motor3_23StepTable[2 * (STEP_AA + STEP_UA + STEP_RA) + 1] = {0};
/*将参数降为1/3 S型曲线参数生成的表格*/
uint16_t Motor1_13TimeTable[2 * (STEP_AA + STEP_UA + STEP_RA) + 1] = {0};
uint16_t Motor1_13StepTable[2 * (STEP_AA + STEP_UA + STEP_RA) + 1] = {0};
uint16_t Motor2_13TimeTable[2 * (STEP_AA + STEP_UA + STEP_RA) + 1] = {0};
uint16_t Motor2_13StepTable[2 * (STEP_AA + STEP_UA + STEP_RA) + 1] = {0};
uint16_t Motor3_13TimeTable[2 * (STEP_AA + STEP_UA + STEP_RA) + 1] = {0};
uint16_t Motor3_13StepTable[2 * (STEP_AA + STEP_UA + STEP_RA) + 1] = {0};

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

void MotorSetpperMove(uint32_t xstep) {
  uint32_t iX = 0, iX_slow = xstep;
  uint16_t plusX = MOTOR_X_START_PLUS;
  uint16_t ipX = 0;
  uint16_t slow_count = 3, cut_down = 0;
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

      IWDG_Feed();

      // X轴速度控制,确保脉冲完整
      switch (statusX) {
        case motor_start:
          plusX = MOTOR_X_START_PLUS;
          // 十分之一
          if (iX >= 100) {
            statusX = motor_start_fast;
          }
          break;
        case motor_start_fast:
          if ((iX > 100) && (iX <= 500)) {
            slow_count = 5;
            cut_down = 5;
          } else if ((iX > 500) && (iX < 7000)) {
            slow_count = 20;
            cut_down = 1;
          } else if ((iX > 7000) && (iX >= 10000)) {
            slow_count = 350;
            cut_down = 1;
          } else if (iX > 10000) {
            slow_count = 580;
            cut_down = 1;
          }
          if (iX % slow_count == 0) {
            plusX -= cut_down;
          }
          if (plusX <= MOTOR_X_FAST_PLUS) {
            plusX = MOTOR_X_FAST_PLUS;
            statusX = motor_fast;
          }
          break;
        case motor_fast:
          break;
        case motor_slowdown:
          break;
        case motor_slow:
          break;
        case motor_stop:
          break;
        default:
          break;
      }
    }
  }
  GPIO_ResetBits(GPIOB, GPIO_Pin_3);
  GPIO_ResetBits(GPIOB, GPIO_Pin_4);
}

/**************************************************************************************
电机运行参数初始化*/
void MotorRunParaInitial(void) {
  /*FIXME:用户可以改变该参数实现S型曲线的升降特性*/
  CalcMotorPeriStep_CPF(M_FRE_START, M_FRE_AA, M_T_AA, M_T_UA, M_T_RA,
                        Motor1TimeTable, Motor1StepTable);
  CalcMotorPeriStep_CPF(M_FRE_START, M_FRE_AA, M_T_AA, M_T_UA, M_T_RA,
                        Motor2TimeTable, Motor2StepTable);
  CalcMotorPeriStep_CPF(M_FRE_START, M_FRE_AA, M_T_AA, M_T_UA, M_T_RA,
                        Motor3TimeTable, Motor3StepTable);

  /*更改参数降为2/3生成的表格*/
  CalcMotorPeriStep_CPF(M_FRE_START * 2.0 / 3, M_FRE_AA * 2.0 / 3,
                        M_T_AA * 2.0 / 3, M_T_UA * 2.0 / 3, M_T_RA * 2.0 / 3,
                        Motor1_23TimeTable, Motor1_23StepTable);
  CalcMotorPeriStep_CPF(M_FRE_START * 2.0 / 3, M_FRE_AA * 2.0 / 3,
                        M_T_AA * 2.0 / 3, M_T_UA * 2.0 / 3, M_T_RA * 2.0 / 3,
                        Motor2_23TimeTable, Motor2_23StepTable);
  CalcMotorPeriStep_CPF(M_FRE_START * 2.0 / 3, M_FRE_AA * 2.0 / 3,
                        M_T_AA * 2.0 / 3, M_T_UA * 2.0 / 3, M_T_RA * 2.0 / 3,
                        Motor3_23TimeTable, Motor3_23StepTable);

  /*更改参数降为1/3生成的表格*/
  CalcMotorPeriStep_CPF(M_FRE_START * 1.0 / 3, M_FRE_AA * 1.0 / 3,
                        M_T_AA * 1.0 / 3, M_T_UA * 1.0 / 3, M_T_RA * 1.0 / 3,
                        Motor1_13TimeTable, Motor1_13StepTable);
  CalcMotorPeriStep_CPF(M_FRE_START * 1.0 / 3, M_FRE_AA * 1.0 / 3,
                        M_T_AA * 1.0 / 3, M_T_UA * 1.0 / 3, M_T_RA * 1.0 / 3,
                        Motor2_13TimeTable, Motor2_13StepTable);
  CalcMotorPeriStep_CPF(M_FRE_START * 1.0 / 3, M_FRE_AA * 1.0 / 3,
                        M_T_AA * 1.0 / 3, M_T_UA * 1.0 / 3, M_T_RA * 1.0 / 3,
                        Motor3_13TimeTable, Motor3_13StepTable);
}

/*计算S型曲线算法的每一步定时器周期及步进数*/
void CalcMotorPeriStep_CPF(float fstart, float faa, float taa, float tua,
                           float tra, uint16_t MotorTimeTable[],
                           uint16_t MotorStepTable[]) {
  int i;
  float fi;

  for (i = 0; i < STEP_AA; i++) {
    fi = GetFreAtTime(fstart, faa, taa, tua, tra, taa / STEP_AA * i);
    MotorTimeTable[i] = F2TIME_PARA / fi;
    MotorStepTable[i] = fi * (taa / STEP_AA) / STEP_PARA;
  }
  for (i = STEP_AA; i < STEP_AA + STEP_UA; i++) {
    fi = GetFreAtTime(fstart, faa, taa, tua, tra,
                      taa + (tua / STEP_UA) * (i - STEP_AA));
    MotorTimeTable[i] = F2TIME_PARA / fi;
    MotorStepTable[i] = fi * (tua / STEP_UA) / STEP_PARA;
  }
  for (i = STEP_AA + STEP_UA; i < STEP_AA + STEP_UA + STEP_RA; i++) {
    fi = GetFreAtTime(fstart, faa, taa, tua, tra,
                      taa + tua + tra / STEP_RA * (i - STEP_AA - STEP_UA));
    MotorTimeTable[i] = F2TIME_PARA / fi;
    MotorStepTable[i] = fi * (tra / STEP_RA) / STEP_PARA;
  }
  fi = GetFreAtTime(fstart, faa, taa, tua, tra, taa + tua + tra);
  MotorTimeTable[STEP_AA + STEP_UA + STEP_RA] = F2TIME_PARA / fi;
  MotorStepTable[STEP_AA + STEP_UA + STEP_RA] =
      fi * (tra / STEP_RA) / STEP_PARA;

  for (i = STEP_AA + STEP_UA + STEP_RA + 1;
       i < 2 * (STEP_AA + STEP_UA + STEP_RA) + 1; i++) {
    MotorTimeTable[i] = MotorTimeTable[2 * (STEP_AA + STEP_UA + STEP_RA) - i];
    MotorStepTable[i] = MotorStepTable[2 * (STEP_AA + STEP_UA + STEP_RA) - i];
  }
}

/*根据S型曲线参数获取某个时刻的频率*/
float GetFreAtTime(float fstart, float faa, float taa, float tua, float tra,
                   float t) {
  //根据公式计算从开始到最高速过冲中，t时刻的转动频率
  if (t >= 0 && t <= taa) {
    //加加速阶段
    return fstart + 0.5 * faa * t * t;
  } else if (taa < t && t <= (taa + tua)) {
    //匀加速阶段
    return fstart + 0.5 * faa * taa * taa + (t - taa) * faa * taa;
  } else if ((taa + tua) < t && t <= (taa + tua + tra)) {
    //减加速阶段
    return fstart + 0.5 * faa * taa * taa + (tua)*faa * taa +
           0.5 * faa * taa * tra -
           0.5 * faa * taa * (taa + tua + tra - t) * (taa + tua + tra - t) /
               (tra);
  }
  return 0;
}