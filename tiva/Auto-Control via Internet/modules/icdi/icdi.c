/*
 *	Author	: Nguyen Chinh Thuy.
 *	Date	: 08/09/2017.
 *	Version	: 1.0.1.
 */
/******************************************************************************
 *	Include
 *****************************************************************************/
#include "icdi.h"


/******************************************************************************
 *	Private
 *****************************************************************************/
bool flgICDI;
char buff[256];

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
 *  Function:
 *
 *  Input   :
 *
 *  Output  :
 */
bool icdiSetup()
{
    /* Setup UART */
    uartSetup(ui32ClkFreq, ICDI_MODULE, 115200, UART_INT_RT | UART_INT_RX);

    /* Notify */
    icdiSendStr(">>> [ICDI] module is enabled.\n");

    /* Return */
    return false;
}
//-----------------------------------------------------------------------------
/*
 *  Function:
 *
 *  Input   :
 *
 *  Output  :
 */
void icdiSendChar(char ch)
{
    uartSendChar(ICDI_MODULE, ch);
}
//-----------------------------------------------------------------------------
/*
 *  Function:
 *
 *  Input   :
 *
 *  Output  :
 */
void icdiSendStr(char *str)
{
    uartSendStr(ICDI_MODULE, str);
}
//-----------------------------------------------------------------------------
/*
 *  Function:
 *
 *  Input   :
 *
 *  Output  :
 */
void icdiSendArr(uint8_t arr[], uint16_t len)
{
    uartSendArr(ICDI_MODULE, arr, len);
}
//-----------------------------------------------------------------------------
/*
 *  Function:
 *
 *  Input   :
 *
 *  Output  :
 */
void UART0_Handler()
{
    /* Clear interrupt */
    UARTIntClear(ICDI_MODULE, UART_INT_RX | UART_INT_RT);

//    /* Redirect received data to ESP8266 */
//    uartSendChar(WIFI_MODULE, uartGetChar(ICDI_MODULE));

    /* Testing of Data transaction */
    //-Get data from Terminal-//
    uartGetStrNLCR(ICDI_MODULE, (uint8_t*)buff);
    buff[6] = 0; buff[13] = 0;
    icdiSendStr("\r\n>>> Username: ");
    icdiSendStr(buff);
    icdiSendStr("\tPassword: ");
    icdiSendStr(buff+7);
    icdiSendStr("\r\n");
    flgICDI = true;
}


