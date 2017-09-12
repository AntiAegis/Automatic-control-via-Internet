/*
 *	Date	: 10/09/2017.
 *	Content	: 
 */
#ifndef ACCOUNT_H_
#define ACCOUNT_H_
/******************************************************************************
 *	Include
 *****************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include <jansson.h>
#include "../json/json.h"

#include "../frame/frame.h"
#include "../fileproc/fileproc.h"


/******************************************************************************
 *	Definition
 *****************************************************************************/


/******************************************************************************
 *	Function
 *****************************************************************************/
/*
 *	Compare the login admin account to the stored admin account.
 */
bool compareAdminAccount(char *usr, char *pass);


/******************************************************************************
 *	Private
 *****************************************************************************/



#endif /* ACCOUNT_H_ */