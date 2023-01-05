/**************************************************************************//**
 * \file Interface.cpp
 * \author Roman Holderried
 *
 * \brief Serial and wireless interface to the RSU.
 * 
 * <b> History </b>
 * 	- 2022-12-06 - File creation
 *****************************************************************************/

/******************************************************************************
 * Includes
 *****************************************************************************/
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "Interface.h"

/******************************************************************************
 * Defines
 *****************************************************************************/

/******************************************************************************
 * Private Globals
 *****************************************************************************/
static tsINTERFACE sInterface = tsINTERFACE_DEFAULTS;
static char cReturnString[MAXIMUM_RETURN_STRING_BYTE_LENGTH];
/******************************************************************************
 * Exported Globals
 *****************************************************************************/

/******************************************************************************
 * Function definitions
 *****************************************************************************/
void InitInterfaces (tPROCESS_COMMAND_CB cbProcessCommand, const char ** pcCommands, uint8_t ui8CommandLen, const char ** pcIDs, uint8_t ui8IDLen)
{
    sInterface.cbProcessCommand = cbProcessCommand;

    sInterface.pcCommands       = pcCommands;
    sInterface.ui8CommandLen    = ui8CommandLen;
    sInterface.pcIDs            = pcIDs;
    sInterface.ui8IDLen         = ui8IDLen;
}

//=============================================================================
bool InterfaceAddTransmitCallback(uint8_t ui8IfIdx, tTRANSMIT_CB txFn)
{
    if (ui8IfIdx < NUMBER_OF_INTERFACES - 1)
    {
        sInterface.sTransmitParameter.cbTransmit[ui8IfIdx] = txFn;
        return true;
    }
    else
        return false;
}

//=============================================================================
void InterfaceReceiveString (uint8_t ui8Data, uint8_t ui8IfIdx)
{
    static uint8_t ui8PossibleCommands[INTERFACE_MAX_NUMBER_OF_COMMANDS] = {0};
    // static teINTERFACE_COMMAND ePossibleCommands[sizeof(sizeof(cValidCommands)/sizeof(char*))];
    static uint8_t ui8PossibleCommandsLen = 0;

    if (ui8Data == '\n')
    {
        sInterface.sReceiveParameter.bConclude = true;
        sInterface.sTransmitParameter.ui8IfIdx = ui8IfIdx;
    }

    switch (sInterface.sReceiveParameter.eInterfaceState)
    {
        case eINTERFACE_RECEIVE_STATE_IDLE:
        {
            uint8_t i = 0;
            // uint8_t ui8NumOfCmds = sizeof(cValidCommands)/sizeof(char*);

            ui8PossibleCommandsLen = 0;

            // Check the very first letter of every possible command
            for (; i < sInterface.ui8CommandLen; i++)
            {
                if (ui8Data == sInterface.pcCommands[i][0])
                {
                    ui8PossibleCommands[ui8PossibleCommandsLen] = i;
                    // ePossibleCommands[ui8PossibleCommandsLen] = (teINTERFACE_COMMAND)i;
                    ui8PossibleCommandsLen++;
                }
            }

            // Possible commands found -> trigger the protocol
            if (ui8PossibleCommandsLen > 0)
            {
                // sInterface.sReceiveParameter.eCmd = eCommandArr[i];
                sInterface.sReceiveParameter.ui16MsgByteLen = 0;
                sInterface.sResults.ui8ParNum = 0;
                // sInterface.sReceiveParameter.sRObject.cObj[0] = ui8Data;
                sInterface.sReceiveParameter.sRObject.ui16ObjLen = 0;
                sInterface.sReceiveParameter.eInterfaceState = eINTERFACE_RECEIVE_STATE_COMMAND;
            }
        }
        break;

        case eINTERFACE_RECEIVE_STATE_COMMAND:
        {
            uint8_t i = 0;

            // Check the second letter of every possible command
            for (; i < ui8PossibleCommandsLen; i++)
            {
                if (ui8Data == sInterface.pcCommands[ui8PossibleCommands[i]][1])
                    break;
            }

            // if we meet one command, proceed to receive parameters
            if (i < ui8PossibleCommandsLen)
            {
                sInterface.sResults.ui8CmdNum = ui8PossibleCommands[i];
                sInterface.sReceiveParameter.eInterfaceState = eINTERFACE_RECEIVE_STATE_WAIT;
            }
            else
                sInterface.sReceiveParameter.eInterfaceState = eINTERFACE_RECEIVE_STATE_IDLE;
        }
        break;

        // Wait for the next character that is no space
        case eINTERFACE_RECEIVE_STATE_WAIT:
            if (sInterface.sReceiveParameter.bConclude)
            {
                sInterface.sReceiveParameter.eInterfaceState = eINTERFACE_RECEIVE_STATE_READY;
                sInterface.sReceiveParameter.bValid = true;
            }
            // Only transition to next state if it is no ' '
            else if (ui8Data != ' ')
            {
                if (sInterface.sResults.ui8ParNum >= INTERFACE_MAX_NUMBER_OF_PARAMETERS)
                {
                    sInterface.sReceiveParameter.bValid = false;
                    sInterface.sReceiveParameter.eInterfaceState = eINTERFACE_RECEIVE_STATE_WAIT_FOR_MESSAGE_END;
                }
                else 
                {
                    if (ui8Data == '-')
                    {
                        sInterface.sReceiveParameter.eInterfaceState = eINTERFACE_RECEIVE_STATE_MINUS_RECEIVED;
                    }
                    else
                    {
                        if(ui8Data < '0' || ui8Data > '9')
                        {
                            sInterface.sReceiveParameter.bValid = false;
                            sInterface.sReceiveParameter.eInterfaceState = eINTERFACE_RECEIVE_STATE_WAIT_FOR_MESSAGE_END;
                        }
                        else
                        {
                            sInterface.sReceiveParameter.eInterfaceState = eINTERFACE_RECEIVE_STATE_GET_PAR;
                        }
                    }
                    sInterface.sReceiveParameter.sRObject.cObj[0] = ui8Data;
                    sInterface.sReceiveParameter.sRObject.ui16ObjLen = 1;
                }

            }

                // sInterface.sReceiveParameter.eInterfaceState = eINTERFACE_RECEIVE_STATE_GET_PAR_ID;
                // No break here, we want to go on with the next state immediately
            break;
        
        case eINTERFACE_RECEIVE_STATE_MINUS_RECEIVED:

            // Received object could also be a negative number
            if ((ui8Data >= '0') && (ui8Data <= '9'))
            {
                sInterface.sReceiveParameter.eInterfaceState = eINTERFACE_RECEIVE_STATE_GET_PAR;
                sInterface.sReceiveParameter.sRObject.cObj[1] = ui8Data;
                sInterface.sReceiveParameter.sRObject.ui16ObjLen++;
            }
            else if (ui8Data != ' ')
            {
                // If the message is concluded here, it is always invalid
                if (sInterface.sReceiveParameter.bConclude)
                {
                    sInterface.sReceiveParameter.bValid = false;
                    sInterface.sReceiveParameter.eInterfaceState = eINTERFACE_RECEIVE_STATE_READY;
                }
                else
                {
                    uint8_t i = 0;
                    // uint8_t ui8NumOfIDs = sizeof(cValidParIDs)/sizeof(char*);

                    sInterface.sReceiveParameter.sRObject.cObj[1] = ui8Data;
                    sInterface.sReceiveParameter.sRObject.cObj[2] = '\0';

                    for (; i < sInterface.ui8IDLen; i++)
                    {
                        if (!strcmp(sInterface.pcIDs[i], sInterface.sReceiveParameter.sRObject.cObj))
                            break;
                    }

                    // Parameter ID found 
                    if (i < sInterface.ui8IDLen)
                    {
                        sInterface.sResults.ui8ParID[sInterface.sResults.ui8ParNum] = i;
                        sInterface.sResults.fParVal[sInterface.sResults.ui8ParNum] = 0.0f;
                        sInterface.sResults.ui8ParNum++;
                        sInterface.sReceiveParameter.sRObject.ui16ObjLen = 0;
                        sInterface.sReceiveParameter.eInterfaceState = eINTERFACE_RECEIVE_STATE_WAIT;
                    }
                    // Not a valid command
                    else
                    {
                        sInterface.sReceiveParameter.bValid = false;
                        sInterface.sReceiveParameter.eInterfaceState = eINTERFACE_RECEIVE_STATE_WAIT_FOR_MESSAGE_END;
                    }

                }
            }
            break;

        case eINTERFACE_RECEIVE_STATE_GET_PAR:

            // Parameter string has been received
            if ((sInterface.sReceiveParameter.sRObject.ui16ObjLen > 0) && (ui8Data == ' ') || sInterface.sReceiveParameter.bConclude)
            {
                sInterface.sReceiveParameter.sRObject.cObj[sInterface.sReceiveParameter.sRObject.ui16ObjLen] = '\0';
                // Number of parameters are already checked in the former state
                sInterface.sResults.fParVal[sInterface.sResults.ui8ParNum - 1] = (float)atof(sInterface.sReceiveParameter.sRObject.cObj);
                sInterface.sReceiveParameter.sRObject.ui16ObjLen = 0;

                // Decide if we want to wait for another parameter or if we want to conclude the message
                if (!sInterface.sReceiveParameter.bConclude)
                    sInterface.sReceiveParameter.eInterfaceState = eINTERFACE_RECEIVE_STATE_WAIT;
                else
                {
                    sInterface.sReceiveParameter.eInterfaceState = eINTERFACE_RECEIVE_STATE_READY;
                    sInterface.sReceiveParameter.bValid = true;
                }
            }
            else if (ui8Data == ' ')
            {
                ; // Do nothing in this case
            }
            else
            {
                sInterface.sReceiveParameter.sRObject.ui16ObjLen++;

                // Make sure that the data format is sufficient
                if (sInterface.sReceiveParameter.sRObject.ui16ObjLen > MAXIMUM_RECEIVE_OBJECT_BYTE_LENGTH ||
                    ((ui8Data < '0' || ui8Data > '9') && ui8Data != '.'))
                {
                    sInterface.sReceiveParameter.bValid = false;
                    sInterface.sReceiveParameter.eInterfaceState = eINTERFACE_RECEIVE_STATE_WAIT_FOR_MESSAGE_END;
                }
                else
                {
                    sInterface.sReceiveParameter.sRObject.cObj[sInterface.sReceiveParameter.sRObject.ui16ObjLen - 1] = ui8Data;
                }
            }

            break;

        case eINTERFACE_RECEIVE_STATE_WAIT_FOR_MESSAGE_END:

            if (sInterface.sReceiveParameter.bConclude)
                sInterface.sReceiveParameter.eInterfaceState = eINTERFACE_RECEIVE_STATE_READY;

            break;

        default:
            break;
    }

    /// @todo Pack this into a separate function
    if (sInterface.sReceiveParameter.eInterfaceState == eINTERFACE_RECEIVE_STATE_READY)
    {
        _InterfaceMsgEval();
        sInterface.sReceiveParameter.eInterfaceState = eINTERFACE_RECEIVE_STATE_IDLE;
        sInterface.sReceiveParameter.bValid = false;
        sInterface.sReceiveParameter.bConclude = false;
    }

    if (sInterface.sReceiveParameter.eInterfaceState != eINTERFACE_RECEIVE_STATE_IDLE)
        sInterface.sReceiveParameter.ui16MsgByteLen++;
}

//=============================================================================
void _InterfaceMsgEval (void)
{
    uint8_t ui8ReturnStringLen;
    char* cMsg;
    uint16_t ui16MsgLen;
    bool bSuccess;

    if (sInterface.sReceiveParameter.bValid)
    {
        // Process the command
        bSuccess = sInterface.cbProcessCommand(&sInterface.sResults, cReturnString, MAXIMUM_RETURN_STRING_BYTE_LENGTH, &ui8ReturnStringLen);
    }
    else
    {
        ui8ReturnStringLen = snprintf(cReturnString, MAXIMUM_RETURN_STRING_BYTE_LENGTH, "Invalid Command String");
        bSuccess = false;
    }

    cMsg = (char*)malloc(ui8ReturnStringLen + 11);

    if (bSuccess)
        ui16MsgLen = sprintf(cMsg, "SUCCESS: %s\n", cReturnString);
    else
        ui16MsgLen = sprintf(cMsg, "ERROR: %s\n", cReturnString);

    _InterfaceTransmit(cMsg, ui16MsgLen);

    free(cMsg);
}

//=============================================================================
void _InterfaceTransmit(char* cMsg, uint16_t ui16MsgLen)
{
    // Send the return message to the interface where it came from
    if (sInterface.sTransmitParameter.cbTransmit[sInterface.sTransmitParameter.ui8IfIdx] != NULL)
        sInterface.sTransmitParameter.cbTransmit[sInterface.sTransmitParameter.ui8IfIdx](cMsg, ui16MsgLen);
}