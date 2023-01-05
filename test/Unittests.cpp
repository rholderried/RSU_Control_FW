#include <stdio.h>
#include <string.h>
#include <unity.h>
#include "Interface.h"
#include "Test_Application.h"
// #include "Common.h"


static const char* cInterfaceCommands[] = {"MS", "MN", "GS"};
static const char* cInterfaceIDs[] = {"-s", "-a", "-v", "-p"};

const char *msg     = "GS    -s 1   -a 2. 34 -v 23 -p 1  \n";
// const char *msg2    = "MN -s 1 -a 2.34 -v 23 -p 1\n";
// const char *msg3    = "MN -s 3  -a 2.34  -v 23 -p 1\n";


void setUp (void)
{
    ;
}

void tearDown (void)
{
    ;
}


bool dummy (tsINTERFACE_RECEIVE_RESULTS *sResults, char* cReturnString, uint8_t ui8ReturnStringMaxLen, uint8_t *pui8ReturnStrLen)
{

    uint8_t ui8OptStrLen = 0;
    char cRet[MAXIMUM_RETURN_STRING_BYTE_LENGTH] = {'\0'};
    int i = 0;

    strcat(cRet, cInterfaceCommands[sResults->ui8CmdNum]);
    if (sResults->ui8ParNum > 0)
        strcat(cRet, "-> ");

    while (i < sResults->ui8ParNum)
    {
        uint8_t len = 0;
        char str[20] = {'\0'};
        len = snprintf(str, 20, "%s : %3.4f", cInterfaceIDs[sResults->ui8ParID[i]], sResults->fParVal[i]);
        strcat(cRet, str);
        i++;
        if (i < sResults->ui8ParNum)
            strcat(cRet, ", ");
    }

    *pui8ReturnStrLen = snprintf(cReturnString, ui8ReturnStringMaxLen, cRet);

    return true;
}

void TransmitSimulator (char* cMsg, uint16_t ui16Len)
{
    printf(cMsg);
}

int main(void)
{
    // InitInterfaces(dummy, cInterfaceCommands, 3, cInterfaceIDs, 4);
    // InterfaceAddTransmitCallback(0, TransmitSimulator);

    // for (int i = 0; msg[i] != '\0'; i++)
    //     InterfaceReceiveString(msg[i], 0);
    
    // // for (int i = 0; msg2[i] != '\0'; i++)
    // //     InterfaceReceiveString(msg2[i], 0);

    // // for (int i = 0; msg3[i] != '\0'; i++)
    // //     InterfaceReceiveString(msg3[i], 0);
    // return 0;

    Test_Module(Test_Startup, 5000);
}