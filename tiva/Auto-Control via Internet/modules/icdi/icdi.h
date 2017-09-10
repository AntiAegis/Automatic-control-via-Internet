/*
 *	Author	: Nguyen Chinh Thuy.
 *	Date	: 08/09/2017.
 *	Version	: 1.0.1.
 */

#ifndef	ICDI_H_
#define	ICDI_H_

/******************************************************************************
 *	Include
 *****************************************************************************/
/* Standard */
#include <stdint.h>
#include <stdbool.h>

/* Driver */
#include <UART.1.0.1.h>

/* Project */
#include "../pin_def.h"


/******************************************************************************
 *	Definition
 *****************************************************************************/


/******************************************************************************
 *	Function
 *****************************************************************************/
/*
 *  Setup ICDI module.
 */
bool icdiSetup();


/*
 *
 */
void icdiSendChar(char ch);


/*
 *
 */
void icdiSendStr(char *str);


/*
 *
 */
void icdiSendArr(uint8_t arr[], uint16_t len);


/*
 *
 */
void UART0_Handler();


#endif	/* ICDI_H_ */
