/****************************************************************
 * Copyright (C) 2017, XinLi, all right reserved.
 * File name:    Buzzer.h
 * Date:         2017.08.28
 * Description:  Buzzer driver.
*****************************************************************/

#ifndef __BUZZER_H
#define __BUZZER_H

/****************************************************************
 *                        Header include
*****************************************************************/
#include "stm32f4xx.h"

/****************************************************************
 *                       Macro definition
*****************************************************************/

/******************** Interface configuration *******************/

#define BUZZER_RCC_AHB1Periph_GPIO          RCC_AHB1Periph_GPIOF
#define BUZZER_GPIO                         GPIOF
#define BUZZER_GPIO_Pin                     GPIO_Pin_8

#define BUZZER_GPIO_PinSource               GPIO_PinSource8
#define BUZZER_GPIO_AF_TIM                  GPIO_AF_TIM13

#define BUZZER_RCC_APB1Periph_TIM           RCC_APB1Periph_TIM13
#define BUZZER_TIM                          TIM13

#define BUZZER_TIM_Prescaler                (8399)
#define BUZZER_TIM_Period                   (9999)

#define BUZZER_TIM_OCPolarity               TIM_OCPolarity_High

#define BUZZER_TIM_OCInit                   TIM_OC1Init
#define BUZZER_TIM_OCPreloadConfig          TIM_OC1PreloadConfig

#define BUZZER_TIM_SetCompare               TIM_SetCompare1

#define BUZZER_TIM_IRQn                     TIM8_UP_TIM13_IRQn
#define BUZZER_TIM_IRQHandler               TIM8_UP_TIM13_IRQHandler
#define BUZZER_TIM_IRQ_PreemptionPriority   (0)
#define BUZZER_TIM_IRQ_SubPriority          (0)

/****************************************************************/

/****************************************************************
 *                       Type definition
*****************************************************************/
typedef enum
{
  BuzzerStateOff     = 0,
  BuzzerStateRing    = 1,
  BuzzerStateDrip    = 2,
  BuzzerStateDidi    = 3,
  BuzzerStateDidiDi  = 4,
  BuzzerStateWarning = 5,
  BuzzerStateDanger  = 6
}BuzzerState;

/****************************************************************
 *                     Structure definition
*****************************************************************/



#ifdef __cplusplus
 extern "C" {
#endif  /* __cplusplus */

/****************************************************************
 *                     Variable declaration
*****************************************************************/


/****************************************************************
 *                     Function declaration
*****************************************************************/
void Buzzer_SetState(BuzzerState state);
BuzzerState Buzzer_GetState(void);

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif	/* __BUZZER_H */
