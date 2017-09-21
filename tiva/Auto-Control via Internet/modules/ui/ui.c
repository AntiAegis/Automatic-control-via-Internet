/*
 *	Author	: Nguyen Chinh Thuy.
 *	Date	: 14/09/2017.
 */
/******************************************************************************
 *	Include
 *****************************************************************************/
#include "ui.h"


/******************************************************************************
 *	Private
 *****************************************************************************/
 /* Pages state */
t_page  pageState;

/* Username and password */
char usr[UI_LEN+1];
char pass[UI_LEN+1];

//-----------------------------------------------------------------------------
/*
 *  Function:
 *
 *  Input   :
 *
 *  Output  :
 */
static void uiCallPage()
{
    switch(pageState)
    {
    case page_idle:
        uiPageIdle();
        break;
    case page_usr:
        uiPageUsr(usr);
        break;
    case page_pass:
        uiPagePass(pass);
        break;
    case page_result:
        uiPageResult(usr, pass);
        break;
    }
}
//-----------------------------------------------------------------------------
/*
 *  Function:   Display welcome string.
 *
 *  Input   :   (void).
 *
 *  Output  :   (void).
 */
static void uiPageIdle()
{
    lcdClearScreen();
    lcdChangeLine(1);
    lcdDisplay("   Welcome to   ");
    lcdChangeLine(2);
    lcdDisplay("  our building  ");
}
//-----------------------------------------------------------------------------
/*
 *  Function:
 *
 *  Input   :
 *
 *  Output  :
 */
static void uiPageUsr(char *usr)
{
    /* Declare */
    static char buffer[UI_LEN];
    static int8_t count = 0;

    /* Title line */
    lcdClearScreen();
    lcdChangeLine(1);
    lcdDisplay("Username:");

    /* Update buffer */
    if(readBtn == BTN(Clr))
    {
        if(--count <= 0)
        {
            count = 0;              // Reset count
            pageState = page_idle;  // Set the page to Idle
            flgBtnInt = true;       // To call [uiServing] function
            readBtn = BTN(None);
            return;
        }
    }
    else if(readBtn == BTN(Cfm))
    {
    }
    else if((readBtn >= BTN(0)) && (readBtn <= BTN(9)))
    {
        buffer[count++] = readBtn - BTN(0) + '0';
    }
    else if((readBtn >= BTN(A)) && (readBtn <= BTN(D)))
    {
        buffer[count++] = readBtn - BTN(A) + 'A';
    }

    /* Content line */
    lcdChangeLine(2);
    lcdAddStr(buffer, count);

    /* Enough characters */
    if(count == UI_LEN)
    {
        count = 0;
        pageState = page_pass;
        flgBtnInt = true;
        readBtn = BTN(None);
        strncpy(usr, buffer, 6);
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
static void uiPagePass(char *pass)
{
    /* Declare */
    static char buffer[UI_LEN];
    static int8_t count = 0;

    /* Title line */
    lcdClearScreen();
    lcdChangeLine(1);
    lcdDisplay("Password:");

    /* Update buffer */
    if(readBtn == BTN(Clr))
    {
        if(--count <= 0)
        {
            count = 0;              // Reset count
            pageState = page_usr;   // Set the page to Username
            flgBtnInt = true;       // To call [uiServing] function
            readBtn = BTN(None);
            return;
        }
    }
    else if(readBtn == BTN(Cfm))
    {
    }
    else if((readBtn >= BTN(0)) && (readBtn <= BTN(9)))
    {
        buffer[count++] = readBtn - BTN(0) + '0';
    }
    else if((readBtn >= BTN(A)) && (readBtn <= BTN(D)))
    {
        buffer[count++] = readBtn - BTN(A) + 'A';
    }

    /* Content line */
    lcdChangeLine(2);
    lcdAddStr(buffer, count);

    /* Enough characters */
    if(count == UI_LEN)
    {
        count = 0;
        pageState = page_result;
        flgBtnInt = true;
        readBtn = BTN(None);
        strncpy(pass, buffer, 6);
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
static void uiPageResult(char *usr, char *pass)
{
    /* Declare */
    bool match_result = false;

    /* Send data */
    wifiSendData(usr, pass);

    /* Receive data */
    match_result = wifiRecData();

    /* Display the result */
    lcdClearScreen();
    lcdChangeLine(1);
    if(match_result)
    {
        lcdDisplay("     Correct    ");
        actServe(1, ACT_DELAY_INTERVAL);
    }
    else
    {
        lcdDisplay("     Wrong      ");
    }

    /* Reset username and password */
    memset(usr, 0, 7);
    memset(pass, 0, 7);
}
//-----------------------------------------------------------------------------


/******************************************************************************
 *  Public
 *****************************************************************************/
/*
 *  Function:
 *
 *  Input   :
 *
 *  Output  :
 */
void uiServing()
{
    while(flgBtnInt)
    {
        flgBtnInt = false;
        uiCallPage();
    }
}
//-----------------------------------------------------------------------------


