/*
 *  Author      : Thuy Nguyen-Chinh.
 *  Date        : Sep 04, 2017
 *  Description :
 *  Version     : 1.0.1.
 */
/******************************************************************************
 *	Include
 *****************************************************************************/
#include "actuator.h"


/******************************************************************************
 *	Private
 *****************************************************************************/
/*
 *  Function:
 *
 *  Input   :
 *
 *  Output  :
 */

//-----------------------------------------------------------------------------


/******************************************************************************
 *	Public
 *****************************************************************************/
/*
 *  Function: Setup module [WiFi].
 *
 *  Input   : (void).
 *
 *  Output  : (bool) True if error, False if no error.
 */
bool actSetup()
{
    gpioLEDSetup(1);
    gpioLEDSetup(2);
    gpioLEDSetup(3);
    gpioLEDSetup(4);
    icdiSendStr("\r\n>>> [Actuator] module is enabled.\r\n");
    return false;
}
//-----------------------------------------------------------------------------
/*
 *  Function: Module [Actuator] serves for requests.
 *
 *  Input   : result    : Matching result.
 *
 *  Output  : (void).
 */
void actServe(bool result)
{
    if(result)
    {
        gpioHigh(ACT_PORT, ACT_PIN);
        clkDelayMs(ACT_DELAY_INTERVAL);
        gpioLow(ACT_PORT, ACT_PIN);
    }
    else
    {
    }
}
//-----------------------------------------------------------------------------
/*
 *  Function: [Actuator] is in the Idle mode.
 *
 *  Input   : (void).
 *
 *  Output  : (void).
 */
void actIdle()
{
    gpioToggle(GPIO_PORT_LED_34, GPIO_PIN_LED_4);
}
//-----------------------------------------------------------------------------
