/*
 *	Author	: Nguyen Chinh Thuy.
 *	Date	: 14/09/2017.
 */
#ifndef	UI_H_
#define	UI_H_
/******************************************************************************
 *	Include
 *****************************************************************************/
#include <stdint.h>
#include <stdbool.h>

#include <GPIO.1.0.1.h>
#include <UART.1.0.1.h>

#include "../pin_def.h"
#include "../keypad/keypad.h"
#include "../lcd/lcd.h"
#include "../actuator/actuator.h"
#include "../wifi/wifi.h"


/******************************************************************************
 *	Definition
 *****************************************************************************/
/* Pages of the User Interface */
typedef enum
{
    page_idle   = 0,        // Welcome page
    page_usr    = 1,        // Username input page
    page_pass   = 2,        // Password input page
    page_result = 3,        // Result notification page.
} t_page;

/* Username and password length */
#define UI_LEN      6


/******************************************************************************
 *	Function
 *****************************************************************************/
/*
 *
 */
void uiServing();


/******************************************************************************
 *  Function
 *****************************************************************************/
/*
 *
 */
static void uiCallPage();

/*
 *
 */
static void uiPageIdle();

/*
 *
 */
static void uiPageUsr(char *usr);

/*
 *
 */
static void uiPagePass(char *pass);

/*
 *
 */
static void uiPageResult();


#endif	/* UI_H_ */

