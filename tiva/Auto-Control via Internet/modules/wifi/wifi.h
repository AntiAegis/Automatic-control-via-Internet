/*
 *	Author	: Nguyen Chinh Thuy.
 *	Date	: 04/09/2017.
 *	Version	: 1.0.1.
 */
#ifndef	WIFI_H_
#define	WIFI_H_
/******************************************************************************
 *	Include
 *****************************************************************************/
/* Standard */
#include <stdint.h>
#include <stdbool.h>

/* Driver */
#include <GPIO.1.0.1.h>
#include <UART.1.0.1.h>
#include <TIMER.1.0.1.h>

/* Project */
#include "../pin_def.h"
#include "../icdi/icdi.h"
#include "esp.h"
#include "frame.h"


/******************************************************************************
 *	Definition
 *****************************************************************************/
/* The device address */
#define WIFI_ADDR_SERVER        0x00
#define WIFI_ADDR               0x01

/* Service: Login */
#define WIFI_SERVICE_ANSWER     0x00
#define WIFI_SERVICE_LOGIN      0x01

/* Timeout in the ESP8266 setup */
#define WIFI_TIMEOUT            1200000

/* IP address and Port of the server */
#define WIFI_SERVER_IP          "192.168.100.19"
#define WIFI_SERVER_PORT        8000


/******************************************************************************
 *	Function
 *****************************************************************************/
/*
 *  Setup [WiFi] module.
 */
bool wifiSetup();

/*
 *  Send a data package.
 */
void wifiSendData(char *usr, char *pass);

/*
 *  Receive a data package.
 */
bool wifiRecData();

/*
 *  Interrupt of UART3, for getting received data.
 */
void UART3_Handler(void);

/*
 *  Interrupt of Timer0-subA, for timeout in ESP8266 setup.
 */
void TIMER0A_Handler();


/******************************************************************************
 *  Private
 *****************************************************************************/
static void espSetupProtocol();
static bool wifiCheckATCmdComplete(char* strCheck);
static void wifiStartTimerCount(uint32_t timeout);
static void wifiStopTimerCount(void);
static uint8_t wifiClearBuffer(uint8_t buff[], uint8_t len, uint8_t start);


#endif	/* WIFI_H_ */
