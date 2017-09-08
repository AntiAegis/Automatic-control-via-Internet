/*
 *	Author	: Nguyen Chinh Thuy.
 *	Date	: 04/09/2017.
 *	Version	: 1.0.1.
 */
/******************************************************************************
 *	Include
 *****************************************************************************/
#include "wifi.h"


/******************************************************************************
 *	Private
 *****************************************************************************/
/* State register */
typedef struct
{
    unsigned Null       : 1;    // Meet '\0' character or not?
    unsigned isSetup    : 1;    // In the Setup progress.
    unsigned flgUartInt : 1;    // UART3 interrupt flag.
} s_regstate;

 s_regstate  sRegState;
//-----------------------------------------------------------------------------
/* Buffer */
uint8_t     buffCount;
uint8_t     wifiBuff[256];
//-----------------------------------------------------------------------------
/*
 *  Function:
 *
 *  Input   :
 *
 *  Output  :
 */
static void espSetupProtocol(uint32_t timeout)
{
    /* Initialize */
    espSetup(WIFI_MODULE, WIFI_PORT_CTL, WIFI_PIN_RST, WIFI_PIN_EN);
    wifiStartTimerCount(timeout);
    while(!wifiCheckATCmdComplete("ready"))
        wifiStartTimerCount(timeout);

    /* Check module */
    espCheckModule();
    wifiStartTimerCount(timeout);
    while(!wifiCheckATCmdComplete("OK"))
        wifiStartTimerCount(timeout);

    /* Connect to Access-Point */
    espConnectWifi("Hoi-gay-kin-tang-8", "lebaloc8197");
    wifiStartTimerCount(timeout);
    while(!wifiCheckATCmdComplete("OK"))
        wifiStartTimerCount(timeout);

    /* Mode of connection */
//    espMultiConnection();
//    wifiStartTimerCount(timeout);
//    while(!wifiCheckATCmdComplete("OK"))
//        wifiStartTimerCount(timeout);

    /* Configure server */
//    espMakeServer(8000);
//    wifiStartTimerCount(timeout);
//    while(!wifiCheckATCmdComplete("OK"))
//        wifiStartTimerCount(timeout);

    /* Connect to the server */
    espConnectTCP(WIFI_SERVER_IP, WIFI_SERVER_PORT);
    wifiStartTimerCount(timeout);
    while(!wifiCheckATCmdComplete("OK"))
        wifiStartTimerCount(timeout);

    /* Get IP address of ESP8266 */
    espGetIP();
    wifiStartTimerCount(timeout);
    while(!wifiCheckATCmdComplete("OK"))
        wifiStartTimerCount(timeout);
}
//-----------------------------------------------------------------------------
/*
 *  Function:
 *
 *  Input   :
 *
 *  Output  :   (bool) True of matched, False if not.
 */
static bool wifiCheckATCmdComplete(char* strCheck)
{
    /* Declare */
    char *ch;
    uint8_t pos = 0;

    /* Wait */
    while(!sRegState.Null);                 // While unless matching '\0'
    wifiStopTimerCount();                   // Stop Timer count
    sRegState.Null = false;                 // Reset [Null] field

    /* Find for string */
DO_AGAIN:
    ch = strstr((char*)(wifiBuff + pos), strCheck);

    /* Reset */
    pos = wifiClearBuffer(wifiBuff, buffCount, pos);
    if(pos >= buffCount)
        buffCount = 0;
    else
        goto DO_AGAIN;

    /* Return */
    return (bool)ch;
}
//-----------------------------------------------------------------------------
/*
 *  Function:
 *
 *  Input   :
 *
 *  Output  :
 */
static void wifiStartTimerCount(uint32_t timeout)
{
    TimerLoadSet(WIFI_TIMER, WIFI_TIMER_SUB, timeout);
    TimerEnable(WIFI_TIMER, WIFI_TIMER_SUB);
}
//-----------------------------------------------------------------------------
/*
 *  Function:
 *
 *  Input   :
 *
 *  Output  :
 */
static void wifiStopTimerCount(void)
{
    TimerDisable(WIFI_TIMER, WIFI_TIMER_SUB);
}
//-----------------------------------------------------------------------------
/*
 *  Function:
 *
 *  Input   :
 *
 *  Output  :
 */
static uint8_t wifiClearBuffer(uint8_t buff[], uint8_t len, uint8_t start)
{
    uint8_t i;
    for(i = start; i < len; i++)
        if(buff[i] != 0)
            buff[i] = 0;
        else
            return i+1;
    return len;
}
//-----------------------------------------------------------------------------
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
 *  Function:   Setup [WiFi] module.
 *
 *	Input	:	(void).
 *
 *	Output	:	(bool).
 */
bool wifiSetup()
{
	/* GPIO setup */
	gpioOutputSetup(WIFI_PORT_CTL, WIFI_PIN_RST | WIFI_PIN_EN);

	/* UART setup */
    sRegState.isSetup = true;
    uartSetup(ui32ClkFreq, WIFI_MODULE, 115200, UART_INT_RT | UART_INT_RX);

    /* Setup timer */
    timerSetup(WIFI_TIMER, WIFI_TIMER_SUB, TIMER_CFG_PERIODIC_UP, true, 1, WIFI_TIMEOUT);
	
	/* ESP8266 setup */
    espSetupProtocol(WIFI_TIMEOUT);

	/* Notify */
	icdiSendStr(">>> [WiFi] module is enabled.\n");

	/* If no error, return false */
    sRegState.isSetup = false;
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
void wifiSendData(char *usr, char *pass)
{
    /* Declare */
    uint8_t frame[30];
    uint8_t data[20];
    uint16_t framelen;

    /* Encode frame */
    sprintf((char*)data, "%s %s", usr, pass);
    data[strlen(usr)] = 0;
    frameEncode(WIFI_ADDR, WIFI_SERVICE_LOGIN,
                strlen(usr)+strlen(pass)+1, data, &framelen, frame);

    /* Send */
    uartSendArr(WIFI_MODULE, frame, framelen);
}
//-----------------------------------------------------------------------------
/*
 *  Function:
 *
 *  Input   :
 *
 *  Output  :
 */
bool wifiRecData()
{
    /* Declare */
    uint8_t addr, fnc;
    uint8_t num;
    uint8_t data[5];

    /* Wait for received data */
WAIT:
    while(!sRegState.flgUartInt);
    sRegState.flgUartInt = false;

    /* Analyze data */
    if(frameDecode(wifiBuff[buffCount-1], &addr, &fnc, &num, data))
    {
        if(addr==WIFI_ADDR_SERVER && fnc==WIFI_SERVICE_ANSWER)
        {
            if(num==1)
            {
                if(data[0])
                    return true;
                else
                    return false;
            }
        }
    }
    else
        goto WAIT;
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
void UART3_Handler(void)
{
    /* Declare */
    char rec;

    /* Clear interrupt */
    UARTIntClear(WIFI_MODULE, UART_INT_RX | UART_INT_RT);
    sRegState.flgUartInt = true;

    /* Send received data into ICDI */
    rec = uartGetChar(WIFI_MODULE);
    icdiSendChar(rec);

    /* Setup progress */
    if(sRegState.isSetup)
    {
        wifiBuff[buffCount] = rec;
        if(wifiBuff[buffCount++] == '\0')
            sRegState.Null = true;
        else
            sRegState.Null = false;
    }

    /* Serving progress */
    else
        wifiBuff[buffCount++] = rec;
}
//-----------------------------------------------------------------------------
/*
 *  Function:
 *
 *  Input   :
 *
 *  Output  :
 */
void TIMER0A_Handler()
{
    sRegState.Null = true;  // Overflow, escape the loop
    timerIntExit(WIFI_TIMER, WIFI_TIMER_SUB);
}
//-----------------------------------------------------------------------------

