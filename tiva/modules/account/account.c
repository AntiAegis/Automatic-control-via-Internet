/******************************************************************************
 *  Author      : Thuy Nguyen-Chinh
 *  Date        : 02 Oct 2017
 *
 *  IDE         : Sublime Text 3
 *
 *  Description : Account provider and pre-verifier.
 *****************************************************************************/

/******************************************************************************
 *  Include
 *****************************************************************************/
#include "account.h"


/******************************************************************************
 *  Definition
 *****************************************************************************/


/******************************************************************************
 *  Private
 *****************************************************************************/
/*
 *  Function: xxxxxxxxxx
 *
 *  Input   : xxxxx: xxxxxxxxxx
 *            xxxxx: xxxxxxxxxx
 *
 *  Output  : xxxxx: xxxxxxxxxx
 *            xxxxx: xxxxxxxxxx
 */
static int sum(uint8_t data[], uint8_t len)
{
    int res = 0, i;
    for(i = 0; i < len; i++)
        res += data[i];
    return res;
}
//-----------------------------------------------------------------------------


/******************************************************************************
 *  Function
 *****************************************************************************/
/*
 *  Function: Pre-heck the validity of login username.
 *
 *  Input   : usrname: Login username.
 *
 *  Output  : (bool) True if username is correct to the framework.
 */
bool accPreCheck(char* usrname)
{
    /* Declare */
    int i;
    char data[ACC_LEN+1];           // Buffer to load username

    uint8_t oddgr[ACC_LEN];         // Group of odd digits
    uint8_t oddlen = 0;             // Number of odd digits
    
    uint8_t evengr[ACC_LEN];        // Group of even digits
    uint8_t evenlen = 0;            // Number of even digits

    uint8_t oddn = 0;               // Number of odd digit exceeds 4

    int validity;                   // Result value

    /* Convert string to numbers */
    strcpy(data, usrname);
    for(i = 0; i < ACC_LEN; i++)
    {
        if((data[i] >= '0') && (data[i] <= '9'))
            data[i] -= '0';
        else if((data[i] >= 'A') && (data[i] <= 'D'))
            data[i] -= 'A' + 10;
    }

    /* Find for odd group */
    for(i = 0; i < ACC_LEN; i++)
        if(data[i] % 2)
            oddgr[oddlen++] = data[i];

    /* Find for number of odd digits which have value that exceed 4 */
    for(i = 0; i < oddlen; i++)
        if(oddgr[i] > 4)
            oddn++;

    /* Find for even group */
    for(i = 0; i < ACC_LEN; i++)
        if(!(data[i] % 2))
            evengr[evenlen++] = data[i];

    /* Verify the validity */
    validity  = 2*sum(oddgr, oddlen);
    validity += sum(evengr, evenlen);
    validity += oddn;

    /* Return */
    if(validity % 10)
        return false;
    else
        return true;
}
//-----------------------------------------------------------------------------
