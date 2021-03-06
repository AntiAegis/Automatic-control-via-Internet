/*
 *  Author  : Nguyen Chinh Thuy.
 *  Date    : 30/10/2016.
 *  Version : 1.0.1.
 */
/******************************************************************************
 *  Include
 *****************************************************************************/
#include "esp.h"


/******************************************************************************
 *  Definition
 *****************************************************************************/


/******************************************************************************
 *  Private
 *****************************************************************************/
uint32_t    ESP_MODULE;
uint32_t	ESP_PORT;
uint32_t	ESP_PIN_RST;
uint32_t	ESP_PIN_EN;


/******************************************************************************
 *	Function
 *****************************************************************************/
 /*
  * Function:	espSetup
  * Purpose	:
  * Input	:
  * Output	:	None
  */
void espSetup(uint32_t ESP_MODULE_, uint32_t ESP_PORT_,
              uint32_t ESP_PIN_RST_, uint32_t ESP_PIN_EN_)
{
	/* Assign pins */
    ESP_MODULE  = ESP_MODULE_;
	ESP_PORT 	= ESP_PORT_;
	ESP_PIN_RST	= ESP_PIN_RST_;
	ESP_PIN_EN	= ESP_PIN_EN_;

	/* Enable module */
	gpioHigh(ESP_PORT, ESP_PIN_EN);

	/* Reset by Hardware */
	espResetHardware();
}
//-----------------------------------------------------------------------------
 /*
  * Function:
  * Purpose	:
  * Input	:
  * Output	:
  */
bool
espCheckModule(void)
{
    uartSendStr(ESP_MODULE, ESP_AT_CHECK_MODULE);
	return false;
}
//-----------------------------------------------------------------------------
 /*
  * Function:
  * Purpose	:
  * Input	:
  * Output	:
  */
void
espResetHardware(void)
{
    gpioLow(ESP_PORT, ESP_PIN_RST);
    clkDelayUs(1000);
	gpioHigh(ESP_PORT, ESP_PIN_RST);
	clkDelayUs(1000);
}
//-----------------------------------------------------------------------------
 /*
  * Function:
  * Purpose	:
  * Input	:
  * Output	:
  */
bool
espResetSofware(void)
{
    uartSendStr(ESP_MODULE, ESP_AT_RESET);
	return false;
}
//-----------------------------------------------------------------------------
 /*
  * Function:
  * Purpose	:
  * Input	:
  * Output	:
  */
bool
espCheckFirmware(void)
{
	uartSendStr(ESP_MODULE, ESP_AT_CHECK_FIRMWARE);
	return false;
}
//-----------------------------------------------------------------------------
 /*
  * Function:
  * Purpose	:
  * Input	:
  * Output	:
  */
bool
espConnectWifi(char strSSID[], char strPassword[])
{
	char strBuff[50];
	strcpy(strBuff, ESP_AT_CONNECT_WIFI);
	strcat(strBuff, "\"");
	strcat(strBuff, strSSID);
	strcat(strBuff, "\",\"");
	strcat(strBuff, strPassword);
	strcat(strBuff, "\"\r\n");

	uartSendStr(ESP_MODULE, strBuff);
	return false;
}
//-----------------------------------------------------------------------------
 /*
  * Function:
  * Purpose	:
  * Input	:
  * Output	:
  */
bool
espDisconnectWifi(void)
{
	uartSendStr(ESP_MODULE, ESP_AT_CONNECT_WIFI);
	return false;
}
//-----------------------------------------------------------------------------
 /*
  * Function:
  * Purpose	:
  * Input	:
  * Output	:
  */
bool
espMakeServer(uint16_t varPort)
{
    char strBuff[25];
	sprintf(strBuff, "%s%d\r\n", ESP_AT_MAKE_SERVER, varPort);
	uartSendStr(ESP_MODULE, strBuff);
	return false;
}
//-----------------------------------------------------------------------------
 /*
  * Function:
  * Purpose	:
  * Input	:
  * Output	:
  */
bool
espSingleConnection(void)
{
	uartSendStr(ESP_MODULE, ESP_AT_SET_TCP_SINGLE);
	return false;
}
//-----------------------------------------------------------------------------
 /*
  * Function:
  * Purpose	:
  * Input	:
  * Output	:
  */
bool
espMultiConnection(void)
{
	uartSendStr(ESP_MODULE, ESP_AT_SET_TCP_MULTI);
	return false;
}
//-----------------------------------------------------------------------------
 /*
  * Function:
  * Purpose	:
  * Input	:
  * Output	:
  */
bool
espGetIP(void)
{
	uartSendStr(ESP_MODULE, ESP_AT_GET_IP);
	return false;
}
//-----------------------------------------------------------------------------
 /*
  * Function:
  * Purpose	:
  * Input	:
  * Output	:
  */
bool
espConnectTCP(char strIPAddr[], uint16_t varPort)
{
    char strBuff[50];
	sprintf(strBuff, "%s\"TCP\",\"%s\",%d\r\n", ESP_AT_CONNECT_TCP, strIPAddr, varPort);
	uartSendStr(ESP_MODULE, strBuff);
	return false;
}
//-----------------------------------------------------------------------------
 /*
  * Function:
  * Purpose	:
  * Input	:
  * Output	:
  */
bool
espDisconnectTCP(void)
{
	uartSendStr(ESP_MODULE, ESP_AT_DISCONNECT_TCP);
	return false;
}
//-----------------------------------------------------------------------------
 /*
  * Function:
  * Purpose	:
  * Input	:
  * Output	:
  */
bool
espChecStatus(void)
{
	uartSendStr(ESP_MODULE, ESP_AT_CHECK_STATUS);
	return false;
}
