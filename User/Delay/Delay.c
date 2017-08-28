/****************************************************************
 * Copyright (C) 2016, XinLi, all right reserved.
 * File name:    Delay.c
 * Date:         2016.03.22
 * Description:  Delay driver.
*****************************************************************/

/****************************************************************
 *                        Header include
*****************************************************************/
#include "Delay.h"

/****************************************************************
 *                       Global variables
*****************************************************************/


/****************************************************************
 *                     Function declaration
*****************************************************************/


/****************************************************************
 *                     Function definition
*****************************************************************/

/****************************************************************
 * Function:    Delay_us
 * Description: Microsecond delay.
 * Input:       nus
 * Output:
 * Return:
*****************************************************************/
void Delay_us(uint64_t nus)
{
  uint64_t nms = 0;
  
  if(nus == 0)
  {
    return;
  }
  
  nms = nus / 1000;
  nus = nus % 1000;
  
  if(nms > 0)
  {
    Delay_ms(nms);
  }
  
  if(nus > 0)
  {
    RCC_ClocksTypeDef RCC_ClockFreq;
    
    RCC_GetClocksFreq(&RCC_ClockFreq);                              /* Get the frequencies of different on chip clocks. */
    
    if(RCC_ClockFreq.HCLK_Frequency < 8000000)
    {
      SysTick->CTRL |= SysTick_CLKSource_HCLK;                      /* Configures the SysTick clock source. */
      SysTick->LOAD = RCC_ClockFreq.HCLK_Frequency / 1000000 * nus; /* Time load (SysTick-> LOAD is 24bit). */
    }
    else
    {
      SysTick->CTRL &= SysTick_CLKSource_HCLK_Div8;                 /* Configures the SysTick clock source. */
      SysTick->LOAD = RCC_ClockFreq.HCLK_Frequency / 8000000 * nus; /* Time load (SysTick-> LOAD is 24bit). */
    }
    
    SysTick->VAL = 0;                                               /* Empty counter. */
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;                       /* Start the countdown. */
    
    while((SysTick->CTRL&(1UL<<16)) != (1UL<<16));                  /* Wait time is reached. */
    
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;                      /* Close counter. */
  }
}

/****************************************************************
 * Function:    Delay_ms
 * Description: Millisecond delay.
 * Input:       nms
 * Output:
 * Return:
*****************************************************************/
void Delay_ms(uint64_t nms)
{
  if(nms == 0)
  {
    return;
  }
  
  while(nms > 500)
  {
    RCC_ClocksTypeDef RCC_ClockFreq;
    
    RCC_GetClocksFreq(&RCC_ClockFreq);                            /* Get the frequencies of different on chip clocks. */
    
    if(RCC_ClockFreq.HCLK_Frequency < 8000000)
    {
      SysTick->CTRL |= SysTick_CLKSource_HCLK;                    /* Configures the SysTick clock source. */
      SysTick->LOAD = RCC_ClockFreq.HCLK_Frequency / 1000 * 500;  /* Time load (SysTick-> LOAD is 24bit). */
    }
    else
    {
      SysTick->CTRL &= SysTick_CLKSource_HCLK_Div8;               /* Configures the SysTick clock source. */
      SysTick->LOAD = RCC_ClockFreq.HCLK_Frequency / 8000 * 500;  /* Time load (SysTick-> LOAD is 24bit). */
    }
    
    SysTick->VAL = 0;                                             /* Empty counter. */
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;                     /* Start the countdown. */
    
    while((SysTick->CTRL&(1UL<<16)) != (1UL<<16));                /* Wait time is reached. */
    
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;                    /* Close counter. */
    
    nms -= 500;
  }
  
  RCC_ClocksTypeDef RCC_ClockFreq;
  
  RCC_GetClocksFreq(&RCC_ClockFreq);                              /* Get the frequencies of different on chip clocks. */
  
  if(RCC_ClockFreq.HCLK_Frequency < 8000000)
  {
    SysTick->CTRL |= SysTick_CLKSource_HCLK;                      /* Configures the SysTick clock source. */
    SysTick->LOAD = RCC_ClockFreq.HCLK_Frequency / 1000 * nms;    /* Time load (SysTick-> LOAD is 24bit). */
  }
  else
  {
    SysTick->CTRL &= SysTick_CLKSource_HCLK_Div8;                 /* Configures the SysTick clock source. */
    SysTick->LOAD = RCC_ClockFreq.HCLK_Frequency / 8000 * nms;    /* Time load (SysTick-> LOAD is 24bit). */
  }
  
  SysTick->VAL = 0;                                               /* Empty counter. */
  SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;                       /* Start the countdown. */
  
  while((SysTick->CTRL&(1UL<<16)) != (1UL<<16));                  /* Wait time is reached. */
  
  SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;                      /* Close counter. */
}

/****************************************************************
 * Function:    Delay_s
 * Description: Second delay.
 * Input:       ns
 * Output:
 * Return:
*****************************************************************/
void Delay_s(uint64_t ns)
{
  while(ns > 0)
  {
    Delay_ms(1000);
    ns--;
  }
}
