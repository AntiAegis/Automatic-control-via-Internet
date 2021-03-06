/*
 *  Author      : Thuy Nguyen-Chinh.
 *  Date        : Sep 04, 2017
 *  Description : This file is responsible for initializing the whole of system.
 *  Version     : 1.0.1.
 */
#ifndef	SYSTEM_H_
#define	SYSTEM_H_
/******************************************************************************
 *	Include
 *****************************************************************************/
/* Standard */
#include <stdint.h>
#include <stdbool.h>

/* Driver */
#include "../lib/GPIO.1.0.1.h"
#include "../lib/CLK.1.0.1.h"
#include "../lib/UART.1.0.1.h"
#include "../lib/TIMER.1.0.1.h"

/* Project */
#include "../modules/pin_def.h"
#include "../modules/icdi/icdi.h"
#include "../modules/actuator/actuator.h"
#include "../modules/keypad/keypad.h"
#include "../modules/lcd/lcd.h"
#include "../modules/wifi/wifi.h"
#include "../modules/account/account.h"


/******************************************************************************
 *	Definition
 *****************************************************************************/
/* Modules of system */
typedef enum
{
    modid_Icdi  = 0,
	modid_Act	= 1,
	modid_Kp	= 2,
	modid_Lcd	= 3,
	modid_Wifi	= 4,
} e_modid;
#define	modid(x)	(modid_##x)

typedef struct
{
	e_modid modid;		// Module ID
	bool (*callback)();	// Setup callback
} s_sysmod;


/******************************************************************************
 *	Function
 *****************************************************************************/
/*
 *	System setups modules.
 */
bool
systemSetup
();


#endif	/* SYSTEM_H_ */

