/*
 *  Author      : Thuy Nguyen-Chinh.
 *  Date        : Sep 04, 2017
 *  Description :
 *  Version     : 1.0.1.
 */
#ifndef	ACTUATOR_H_
#define	ACTUATOR_H_
/******************************************************************************
 *	Include
 *****************************************************************************/
/* Standard */
#include <stdint.h>
#include <stdbool.h>

/* Driver */
#include "../../lib/GPIO.1.0.1.h"
#include "../../lib/UART.1.0.1.h"

/* Project */
#include "../pin_def.h"
#include "../icdi/icdi.h"


/******************************************************************************
 *	Definition
 *****************************************************************************/
#define ACT_DELAY_INTERVAL      2000    // ms


/******************************************************************************
 *	Function
 *****************************************************************************/
/*
 *  Setup [Actuator] module.
 */
bool actSetup();


/*
 *  [Actuator] module serves for requests.
 */
void actServe(bool result);


/*
 *  Idle state.
 */
void actIdle();


#endif	/* ACTUATOR_H_ */

