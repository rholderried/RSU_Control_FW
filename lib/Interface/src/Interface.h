/**************************************************************************//**
 * \file Interface.h
 * \author Roman Holderried
 *
 * \brief Interface functions.
 *
 * <b> History </b>
 * 	- 2022-12-06 - File creation
 *****************************************************************************/

#ifndef _INTERFACE_H_
#define _INTERFACE_H_
/******************************************************************************
 * Includes
 *****************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "InterfaceConfig.h"

/******************************************************************************
 * Defines
 *****************************************************************************/

/******************************************************************************
 * Type definitions
 *****************************************************************************/
typedef enum
{
    eINTERFACE_RECEIVE_STATE_IDLE,
    eINTERFACE_RECEIVE_STATE_COMMAND,
    eINTERFACE_RECEIVE_STATE_WAIT,
    eINTERFACE_RECEIVE_STATE_MINUS_RECEIVED,
    eINTERFACE_RECEIVE_STATE_GET_PAR,
    eINTERFACE_RECEIVE_STATE_WAIT_FOR_MESSAGE_END,
    eINTERFACE_RECEIVE_STATE_READY
}teINTERFACE_RECEIVE_STATE;

typedef enum
{
    eINTERFACE_ERROR_NONE           = 0,
    eINTERFACE_ERROR_INVALID_COMMAND= 1,
    eINTERFACE_ERROR_RSU_BUSY       = 2
}teINTERFACE_ERRORS;

typedef struct
{
    uint8_t                     ui8CmdNum;
    uint8_t                     ui8ParNum;
    uint8_t                     ui8ParID[INTERFACE_MAX_NUMBER_OF_PARAMETERS];
    float                       fParVal[INTERFACE_MAX_NUMBER_OF_PARAMETERS];
}tsINTERFACE_RECEIVE_RESULTS;

#define tsINTERFACE_RECEIVE_RESULTS_DEFAULTS {0, 0, {'\0'}, {0.0f}}


typedef void (*tTRANSMIT_CB)(char* cMsg, uint16_t ui16Len);
typedef bool (*tPROCESS_COMMAND_CB)(tsINTERFACE_RECEIVE_RESULTS *sResults, char* cReturnString, uint8_t ui8ReturnStringMaxLen, uint8_t *pui8ReturnStrLen);

typedef struct
{
    struct
    {
        bool                        bValid;
        bool                        bConclude;
        teINTERFACE_RECEIVE_STATE   eInterfaceState;
        uint16_t                    ui16MsgByteLen;

        struct
        {
            uint16_t                ui16ObjLen;
            char                    cObj[MAXIMUM_RECEIVE_OBJECT_BYTE_LENGTH + 1];
        }sRObject;
    }sReceiveParameter;

    struct
    {
        uint8_t ui8IfIdx;
        tTRANSMIT_CB cbTransmit[NUMBER_OF_INTERFACES];
    }sTransmitParameter;

    tsINTERFACE_RECEIVE_RESULTS sResults;

    const char ** pcCommands;
    uint8_t ui8CommandLen;
    const char** pcIDs;
    uint8_t ui8IDLen;

    tPROCESS_COMMAND_CB cbProcessCommand;

    // tsCOMMAND_INFO sCommand;
}tsINTERFACE;

#define tsINTERFACE_DEFAULTS {{false, false, eINTERFACE_RECEIVE_STATE_IDLE, 0, 0, {0, 0}},\
                              {0, {NULL}},\
                              tsINTERFACE_RECEIVE_RESULTS_DEFAULTS,\
                              NULL,\
                              0,\
                              NULL,\
                              0,\
                              NULL}
/******************************************************************************
 * Function declarations
 *****************************************************************************/
void InitInterfaces (tPROCESS_COMMAND_CB cbProcessCommand, const char ** pcCommands, uint8_t ui8CommandLen, const char ** pcIDs, uint8_t ui8IDLen);
bool InterfaceAddTransmitCallback(uint8_t ui8IfIdx, tTRANSMIT_CB txFn);
void InterfaceReceiveString (uint8_t ui8Data, uint8_t ui8IfIdx);
void _InterfaceMsgEval (void);
void _InterfaceTransmit(char* cMsg, uint16_t ui16MsgLen);
#endif //_INTERFACE_H_