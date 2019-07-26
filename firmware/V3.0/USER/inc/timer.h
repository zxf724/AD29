#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"

#define TIMER_LIST_MAX 5

#define PWM1_PreemptionPriority 1  //論撰
#define PWM1_SubPriority 0         //論脯
#define PWM2_PreemptionPriority 1  //論撰
#define PWM2_SubPriority 1         //論脯

typedef void (*tmr_fnct_ptr)(void);
typedef uint8_t TMR_STATE;

typedef struct {
  uint32_t expire;       /* ???????             */
  uint32_t count;        /*?????*/
  tmr_fnct_ptr callback; /* ????                   */
  uint8_t state;         /* ?????                  */
  uint8_t opt;           /* ???? 1?? 0??         */
} timer_t;

void timer_enable(uint8_t num, uint16_t val, void (*timer_backcall_func)(void));
void timer_disable(uint8_t num);
void TIM3_Int_Init(u16 arr, u16 psc);
void TIM4_Int_Init(u16 arr, u16 psc);
void TIM2_Int_Init(u16 arr, u16 psc);
void Initial_PWM_Motor1(void);
void Initial_PWM_Motor2(void);
void MotorRunParaInitial(void);

void timer_task(void);

#endif
