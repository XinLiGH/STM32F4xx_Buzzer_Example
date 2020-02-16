/****************************************************************
 * Copyright (C) 2017, XinLi, all right reserved.
 * File name:    main.c
 * Date:         2017.08.28
 * Description:  Buzzer example.
*****************************************************************/

/****************************************************************
 *                        Header include
*****************************************************************/
#include "main.h"
#include "Delay.h"
#include "Buzzer.h"

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
 * Function:    main
 * Description: Program entry.
 * Input:
 * Output:
 * Return:
*****************************************************************/
int main(void)
{
  for(;;)
  {
    Delay_s(5);
    Buzzer_SetState(BuzzerStateDrip);
    Delay_s(5);
    Buzzer_SetState(BuzzerStateDidi);
    Delay_s(5);
    Buzzer_SetState(BuzzerStateDidiDi);
    Delay_s(5);
    Buzzer_SetState(BuzzerStateWarning);
    Delay_s(5);
    Buzzer_SetState(BuzzerStateDanger);
    Delay_s(5);
    Buzzer_SetState(BuzzerStateRing);
    Delay_s(5);
    Buzzer_SetState(BuzzerStateOff);
  }
}
