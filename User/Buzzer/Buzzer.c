/****************************************************************
 * Copyright (C) 2017, XinLi, all right reserved.
 * File name:    Buzzer.c
 * Date:         2017.08.28
 * Description:  Buzzer driver.
*****************************************************************/

/****************************************************************
 *                        Header include
*****************************************************************/
#include "Buzzer.h"
#include <stdbool.h>

/****************************************************************
 *                       Global variables
*****************************************************************/
static __IO uint8_t     buzzerCount = 0;
static __IO BuzzerState buzzerState = BuzzerStateOff;

/****************************************************************
 *                     Function declaration
*****************************************************************/
static void Buzzer_Init(void);
static void Buzzer_PwmInit(void);
static void Buzzer_NvicInit(void);
static void Buzzer_Off(void);
static void Buzzer_Ring(void);
static void Buzzer_Drip(void);
static void Buzzer_Didi(void);
static void Buzzer_DidiDi(void);
static void Buzzer_Warning(void);
static void Buzzer_Danger(void);

/****************************************************************
 *                     Function definition
*****************************************************************/

/****************************************************************
 * Function:    Buzzer_Init
 * Description: Buzzer initialization.
 * Input:
 * Output:
 * Return:
*****************************************************************/
static void Buzzer_Init(void)
{
  static bool init_flag = false;
  
  if(init_flag == false)
  {
    init_flag = true;
    
    Buzzer_PwmInit();
    Buzzer_NvicInit();
  }
}

/****************************************************************
 * Function:    Buzzer_PwmInit
 * Description: Buzzer pwm initialization.
 * Input:
 * Output:
 * Return:
*****************************************************************/
static void Buzzer_PwmInit(void)
{
  GPIO_InitTypeDef        GPIO_InitStructure    = {0};
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure = {0};
  TIM_OCInitTypeDef       TIM_OCInitStructure   = {0};
  
  RCC_AHB1PeriphClockCmd(BUZZER_RCC_AHB1Periph_GPIO, ENABLE);
  RCC_APB1PeriphClockCmd(BUZZER_RCC_APB1Periph_TIM, ENABLE);
  
  GPIO_InitStructure.GPIO_Pin   = BUZZER_GPIO_Pin;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(BUZZER_GPIO, &GPIO_InitStructure);
  
  GPIO_PinAFConfig(BUZZER_GPIO, BUZZER_GPIO_PinSource, BUZZER_GPIO_AF_TIM);
  
  TIM_TimeBaseStructure.TIM_Prescaler     = BUZZER_TIM_Prescaler;
  TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_Period        = BUZZER_TIM_Period;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseInit(BUZZER_TIM, &TIM_TimeBaseStructure);
  
  TIM_OCInitStructure.TIM_OCMode      = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse       = 0;
  TIM_OCInitStructure.TIM_OCPolarity  = BUZZER_TIM_OCPolarity;
  BUZZER_TIM_OCInit(BUZZER_TIM, &TIM_OCInitStructure);
  
  BUZZER_TIM_OCPreloadConfig(BUZZER_TIM, TIM_OCPreload_Enable);
  
  TIM_ARRPreloadConfig(BUZZER_TIM, ENABLE);
  
  TIM_Cmd(BUZZER_TIM, ENABLE);
}

/****************************************************************
 * Function:    Buzzer_NvicInit
 * Description: Buzzer NVIC initialization.
 * Input:
 * Output:
 * Return:
*****************************************************************/
static void Buzzer_NvicInit(void)
{
  NVIC_InitTypeDef NVIC_InitStructure = {0};
  
  NVIC_InitStructure.NVIC_IRQChannel                   = BUZZER_TIM_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = BUZZER_TIM_IRQ_PreemptionPriority;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority        = BUZZER_TIM_IRQ_SubPriority;
  NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/****************************************************************
 * Function:    Buzzer_Off
 * Description: Buzzer off.
 * Input:
 * Output:
 * Return:
*****************************************************************/
static void Buzzer_Off(void)
{
  Buzzer_Init();
  
  BUZZER_TIM_SetCompare(BUZZER_TIM, 0);
  
  buzzerState = BuzzerStateOff;
}

/****************************************************************
 * Function:    Buzzer_Ring
 * Description: Buzzer ring.
 * Input:
 * Output:
 * Return:
*****************************************************************/
static void Buzzer_Ring(void)
{
  Buzzer_Init();
  
  TIM_SetAutoreload(BUZZER_TIM, (BUZZER_TIM_Period + 1) / 2 - 1);
  BUZZER_TIM_SetCompare(BUZZER_TIM, (BUZZER_TIM_Period + 1) / 2);
  
  buzzerState = BuzzerStateRing;
}

/****************************************************************
 * Function:    Buzzer_Drip
 * Description: Buzzer drip.
 * Input:
 * Output:
 * Return:
*****************************************************************/
static void Buzzer_Drip(void)
{
  Buzzer_Init();
  
  TIM_SetAutoreload(BUZZER_TIM, (BUZZER_TIM_Period + 1) / 2 - 1);
  BUZZER_TIM_SetCompare(BUZZER_TIM, (BUZZER_TIM_Period + 1) / 4);
  
  TIM_ClearFlag(BUZZER_TIM, TIM_FLAG_Update);
  TIM_ITConfig(BUZZER_TIM, TIM_IT_Update, ENABLE);
  
  buzzerCount = 0;
  buzzerState = BuzzerStateDrip;
}

/****************************************************************
 * Function:    Buzzer_Didi
 * Description: Buzzer didi.
 * Input:
 * Output:
 * Return:
*****************************************************************/
static void Buzzer_Didi(void)
{
  Buzzer_Init();
  
  TIM_SetAutoreload(BUZZER_TIM, (BUZZER_TIM_Period + 1) / 4 - 1);
  BUZZER_TIM_SetCompare(BUZZER_TIM, (BUZZER_TIM_Period + 1) / 8);
  
  TIM_ClearFlag(BUZZER_TIM, TIM_FLAG_Update);
  TIM_ITConfig(BUZZER_TIM, TIM_IT_Update, ENABLE);
  
  buzzerCount = 1;
  buzzerState = BuzzerStateDidi;
}

/****************************************************************
 * Function:    Buzzer_DidiDi
 * Description: Buzzer didi di.
 * Input:
 * Output:
 * Return:
*****************************************************************/
static void Buzzer_DidiDi(void)
{
  Buzzer_Init();
  
  TIM_SetAutoreload(BUZZER_TIM, (BUZZER_TIM_Period + 1) / 8 - 1);
  BUZZER_TIM_SetCompare(BUZZER_TIM, (BUZZER_TIM_Period + 1) / 16);
  
  TIM_ClearFlag(BUZZER_TIM, TIM_FLAG_Update);
  TIM_ITConfig(BUZZER_TIM, TIM_IT_Update, ENABLE);
  
  buzzerCount = 2;
  buzzerState = BuzzerStateDidiDi;
}

/****************************************************************
 * Function:    Buzzer_Warning
 * Description: Buzzer warning.
 * Input:
 * Output:
 * Return:
*****************************************************************/
static void Buzzer_Warning(void)
{
  Buzzer_Init();
  
  TIM_SetAutoreload(BUZZER_TIM, (BUZZER_TIM_Period + 1) / 2 - 1);
  BUZZER_TIM_SetCompare(BUZZER_TIM, (BUZZER_TIM_Period + 1) / 4);
  
  buzzerState = BuzzerStateWarning;
}

/****************************************************************
 * Function:    Buzzer_Danger
 * Description: Buzzer danger.
 * Input:
 * Output:
 * Return:
*****************************************************************/
static void Buzzer_Danger(void)
{
  Buzzer_Init();
  
  TIM_SetAutoreload(BUZZER_TIM, (BUZZER_TIM_Period + 1) / 4 - 1);
  BUZZER_TIM_SetCompare(BUZZER_TIM, (BUZZER_TIM_Period + 1) / 8);
  
  buzzerState = BuzzerStateDanger;
}

/****************************************************************
 * Function:    Buzzer_SetState
 * Description: Set buzzer state.
 * Input:       state: Buzzer state.
 * Output:
 * Return:
*****************************************************************/
void Buzzer_SetState(BuzzerState state)
{
  if(state != buzzerState)
  {
    if(state == BuzzerStateOff)
    {
      Buzzer_Off();
    }
    else if(state == BuzzerStateRing)
    {
      Buzzer_Ring();
    }
    else if(state == BuzzerStateDrip)
    {
      Buzzer_Drip();
    }
    else if(state == BuzzerStateDidi)
    {
      Buzzer_Didi();
    }
    else if(state == BuzzerStateDidiDi)
    {
      Buzzer_DidiDi();
    }
    else if(state == BuzzerStateWarning)
    {
      Buzzer_Warning();
    }
    else if(state == BuzzerStateDanger)
    {
      Buzzer_Danger();
    }
  }
}

/****************************************************************
 * Function:    Buzzer_GetState
 * Description: Get buzzer state.
 * Input:
 * Output:
 * Return:      Buzzer state.
*****************************************************************/
BuzzerState Buzzer_GetState(void)
{
  return buzzerState;
}

/****************************************************************
 * Function:    BUZZER_TIM_IRQHandler
 * Description: This function handles TIM handler.
 * Input:
 * Output:
 * Return:
*****************************************************************/
void BUZZER_TIM_IRQHandler(void)
{
  if(TIM_GetITStatus(BUZZER_TIM, TIM_IT_Update) != RESET)
  {
    TIM_ClearITPendingBit(BUZZER_TIM, TIM_IT_Update);
    
    if((buzzerState == BuzzerStateDrip) || (buzzerState == BuzzerStateDidi) || (buzzerState == BuzzerStateDidiDi))
    {
      if(buzzerCount > 0)
      {
        buzzerCount--;
      }
      else
      {
        TIM_ITConfig(BUZZER_TIM, TIM_IT_Update, DISABLE);
        Buzzer_Off();
      }
    }
    else
    {
      TIM_ITConfig(BUZZER_TIM, TIM_IT_Update, DISABLE);
    }
  }
}
