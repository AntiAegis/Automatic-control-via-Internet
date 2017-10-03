/*
 *	Author	: Nguyen Chinh Thuy.
 *	Date	: 04/09/2017.
 *	Version	: 1.0.1.
 */
/******************************************************************************
 *	Include
 *****************************************************************************/
#include "wifi.h"
#include "../ui/ui.h"


/******************************************************************************
 *	Private
 *****************************************************************************/
/* State register */
typedef struct
{
    unsigned Null       : 1;    // Meet '\0' character or not?
    unsigned intMode    : 2;    // In the Setup progress.
    unsigned flgUartInt : 1;    // UART3 interrupt flag.
} s_regstate;

s_regstate  sRegState;
//-----------------------------------------------------------------------------
/* Int mode */
#define WIFI_INT_MODE_NONE      0
#define WIFI_INT_MODE_SETUP     1
#define WIFI_INT_MODE_DATA      2
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
    espConnectWifi("T.H.E.Team", "the.team");
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
//    espConnectTCP(WIFI_SERVER_IP, WIFI_SERVER_PORT);
//    wifiStartTimerCount(timeout);
//    while(!wifiCheckATCmdComplete("OK"))
//        wifiStartTimerCount(timeout);

    /* Get IP address of ESP8266 */
//    espGetIP();
//    wifiStartTimerCount(timeout);
//    while(!wifiCheckATCmdComplete("OK"))
//        wifiStartTimerCount(timeout);
}
//-----------------------------------------------------------------------------
/*
 *  Function:
 *
 *  Input   :
 *
 *  Output  :   (bool) True if matched, False if not.
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
 *  Output  :   1 if matched strMain, -1 if matched strSub, 0 if not matched.
 */
static int wifiCheckATCmdCompleteRobust(char* strMain, char *strSub)
{
    /* Declare */
    char *ch;
    int res;
    uint8_t pos = 0;

    /* Wait */
    while(!sRegState.Null);                 // While unless matching '\0'
    wifiStopTimerCount();                   // Stop Timer count
    sRegState.Null = false;                 // Reset [Null] field

    /* Find for string */
DO_AGAIN:
    ch = strstr((char*)(wifiBuff + pos), strMain);
    if(ch == NULL)
    {
        ch = strstr((char*)(wifiBuff + pos), strSub);
        if(ch == NULL)
            res = 0;
        else
            res = -1;
    }
    else
        res = 1;

    /* Reset */
    pos = wifiClearBuffer(wifiBuff, buffCount, pos);
    if(pos >= buffCount)
        buffCount = 0;
    else
        goto DO_AGAIN;

    /* Return */
    return res;
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
static void wifiRefreshBuffer(uint8_t buff[], uint8_t len)
{
    uint8_t i;
    for(i = 0; i < len; i++)
        buff[i] = 0;
}


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
    /* Print state into screen */
    lcdChangeLine(1);
    lcdDisplay("Connect to WiFi ");

	/* GPIO setup */
	gpioOutputSetup(WIFI_PORT_CTL, WIFI_PIN_RST | WIFI_PIN_EN);

	/* UART setup */
    sRegState.intMode = WIFI_INT_MODE_SETUP;
    uartSetup(ui32ClkFreq, WIFI_MODULE, 115200, UART_INT_RT | UART_INT_RX);

    /* Setup timer */
    timerSetup(WIFI_TIMER, WIFI_TIMER_SUB, TIMER_CFG_PERIODIC_UP, true, 1, WIFI_TIMEOUT);
	
	/* ESP8266 setup */
    espSetupProtocol(WIFI_TIMEOUT);
    sRegState.intMode = WIFI_INT_MODE_NONE;
    sRegState.flgUartInt = false;

	/* Notify */
	icdiSendStr("\r\n>>> [WiFi] module is enabled.\r\n");
    lcdChangeLine(1);
    lcdDisplay("WiFi connected  ");

	/* If no error, return false */
	return false;
}
//-----------------------------------------------------------------------------
/*
 *  Function:   Check whether ESP8266 is connected with the server.
 *
 *  Input   :   (void)
 *
 *  Output  :   (bool) True if connection has been established.
 */
bool wifiCheckServerConnection()
{
    /* Declare */
    char str[15];
    uint8_t tmp;

    /* Setup interrupt mode */
    tmp = sRegState.intMode;
    sRegState.intMode = WIFI_INT_MODE_DATA;

    /* Send AT command */
    strcpy(str, "AT+CIPSTATUS\r\n");
    uartSendStr(WIFI_MODULE, str);

    /* Analyze the received data */
    while(1)
    {
        while(!sRegState.flgUartInt);
        sRegState.flgUartInt = false;
        if((wifiBuff[buffCount-7]=='4'  || wifiBuff[buffCount-7]=='5') &&
            wifiBuff[buffCount-6]=='\r' && wifiBuff[buffCount-5]=='\n' &&
            wifiBuff[buffCount-4]=='\r' && wifiBuff[buffCount-3]=='\n' &&
            wifiBuff[buffCount-2]=='O'  && wifiBuff[buffCount-1]=='K')
        {
            sRegState.intMode = tmp;
            wifiRefreshBuffer(wifiBuff, 255);
            buffCount = 0;
            return false;
        }
        if(wifiBuff[buffCount-7]=='3'  &&
           wifiBuff[buffCount-6]=='\r' && wifiBuff[buffCount-5]=='\n' &&
           wifiBuff[buffCount-4]=='\r' && wifiBuff[buffCount-3]=='\n' &&
           wifiBuff[buffCount-2]=='O'  && wifiBuff[buffCount-1]=='K')
        {
            sRegState.intMode = tmp;
            wifiRefreshBuffer(wifiBuff, 255);
            buffCount = 0;
            return true;
        }
    }
}
//-----------------------------------------------------------------------------
/*
 *  Function:   Connect to server
 *
 *  Input   :   (void)
 *
 *  Output  :   (void)
 */
void wifiConnectServer()
{
    /* Declare */
    int8_t tmp, false_count = 0;

    /* Select interrupt mode */
    sRegState.intMode = WIFI_INT_MODE_SETUP;
    gpioLEDState(3, 1);

    /* Try to connect to the server */
    while(1)
    {
        //-Check the connection with server-//
        if(wifiCheckServerConnection())
            break;

        //-Connect to the server-//
        espConnectTCP(WIFI_SERVER_IP, WIFI_SERVER_PORT);
        wifiStartTimerCount(WIFI_TIMEOUT);
        while(!(tmp = wifiCheckATCmdCompleteRobust("OK", "ERROR")))
            wifiStartTimerCount(WIFI_TIMEOUT);
        if(tmp == 1)
            break;
        if(tmp == -1)
        {
            if(++false_count == WIFI_MAX_TRY)
            {
                lcdClearScreen();
                lcdDisplay("Connection fail");
                clkDelayMs(1000);
                lcdClearScreen();
                lcdDisplay("Check the server");
                lcdChangeLine(1);
                lcdDisplay("Then reset KIT");
                while(1);
            }
            clkDelayMs(2000);
        }
    }

    /* Close interrupt mode */
    gpioLEDState(3, 0);
    sRegState.intMode = WIFI_INT_MODE_NONE;
}
//-----------------------------------------------------------------------------
/*
 *  Function: Disconnect from the server.
 *
 *  Input   : (void)
 *
 *  Output  : (void)
 */
void wifiDisconnectServer()
{
    espDisconnectTCP();
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
    uint8_t frame[8 + 2*UI_LEN];
    uint8_t data[20];
    uint16_t framelen;

    /* Encode frame */
    sprintf((char*)data, "%s %s", usr, pass);
    data[strlen(usr)] = 0;
    frameEncode(WIFI_ADDR, WIFI_SERVICE_LOGIN,
                strlen(usr)+strlen(pass)+1, data, &framelen, frame);

    /* Send */
    sprintf((char*)data, "AT+CIPSEND=%d\r\n", framelen);
    uartSendStr(WIFI_MODULE, (char*)data);
    clkDelayMs(10);
    uartSendArr(WIFI_MODULE, frame, framelen);
    sRegState.flgUartInt = false;
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
    sRegState.intMode = WIFI_INT_MODE_DATA;
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
                {
                    sRegState.intMode = WIFI_INT_MODE_NONE;
                    return true;
                }
                else
                {
                    sRegState.intMode = WIFI_INT_MODE_NONE;
                    return false;
                }
            }
        }
        else
            goto WAIT;
    }
    else
        goto WAIT;

    /* Return */
    sRegState.intMode = WIFI_INT_MODE_NONE;
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

    /* Read received data */
    rec = uartGetChar(WIFI_MODULE);
    icdiSendChar(rec);

    /* Interrupt mode */
    switch(sRegState.intMode)
    {
    //-Setup progress-//
    case WIFI_INT_MODE_SETUP:
        wifiBuff[buffCount] = rec;
        if(wifiBuff[buffCount++] == '\0')
            sRegState.Null = true;
        else
            sRegState.Null = false;
        break;
    //-Read received data from the server-//
    case WIFI_INT_MODE_DATA:
        wifiBuff[buffCount++] = rec;
        break;
    }
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

