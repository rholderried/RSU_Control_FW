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
#include "Common.h"
#include "Configuration.h"

/******************************************************************************
 * Defines
 *****************************************************************************/
#define MAXIMUM_RECEIVE_OBJECT_BYTE_LENGTH  20
#define MAXIMUM_RETURN_STRING_BYTE_LENGTH   128
/******************************************************************************
 * Type definitions
 *****************************************************************************/
typedef enum
{
    eINTERFACE_COMMAND_NONE             = -1,
    eINTERFACE_COMMAND_GO_TO_SLOT       = 0,
    eINTERFACE_COMMAND_GO_TO_NEXT_FREE  = 1
}teINTERFACE_COMMAND;

typedef enum
{
    eINTERFACE_PARAMETER_NONE           = -1,
    eINTERFACE_PARAMETER_SLOT           = 0,
    eINTERFACE_PARAMETER_ACCELERATION   = 1,
    eINTERFACE_PARAMETER_VELOCITY       = 2,
    eINTERFACE_PARAMETER_POSITION       = 3
}teINTERFACE_PARAMETER;

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

typedef void (*tTRANSMIT_CB)(char* cMsg, uint16_t ui16Len);
typedef bool (*tPROCESS_COMMAND_CB)(tsCOMMAND_INFO sCmdInfo, char* cReturnString, uint8_t ui8ReturnStringMaxLen, uint8_t *pui8ReturnStrLen);

typedef struct
{
    struct
    {
        bool                        bValid;
        bool                        bConclude;
        teINTERFACE_RECEIVE_STATE   eInterfaceState;
        teINTERFACE_COMMAND         eCmd;
        teINTERFACE_PARAMETER       eParID[INTERFACE_MAX_NUMBER_OF_PARAMETERS];
        float                       fParVal[INTERFACE_MAX_NUMBER_OF_PARAMETERS];
        uint16_t                    ui16MsgByteLen;
        uint8_t                     ui8ParNum;

        struct
        {
            uint16_t                ui16ObjLen;
            char                    cObj[MAXIMUM_RECEIVE_OBJECT_BYTE_LENGTH + 1];
        }sRObject;
    }sReceiveParameter;

    struct
    {
        uint8_t ui8IfIdx;
        tTRANSMIT_CB cbTransmit[2];
    }sTransmitParameter;

    tPROCESS_COMMAND_CB cbProcessCommand;

    // tsCOMMAND_INFO sCommand;
}tsINTERFACE;

#define tsINTERFACE_DEFAULTS {{false, false, eINTERFACE_RECEIVE_STATE_IDLE, eINTERFACE_COMMAND_NONE, {eINTERFACE_PARAMETER_NONE}, {0.0f}, 0, 0, {0, 0}},\
                              {0, {NULL}},\
                              NULL}
/******************************************************************************
 * Function declarations
 *****************************************************************************/
void InitInterfaces (tPROCESS_COMMAND_CB cbProcessCommand);
bool InterfaceAddTransmitCallback(uint8_t ui8IfIdx, tTRANSMIT_CB txFn);
void InterfaceReceiveString (uint8_t ui8Data, uint8_t ui8IfIdx);
void _InterfaceMsgEval (void);
void _InterfaceTransmit(char* cMsg, u_int16_t ui16MsgLen);
#endif //_INTERFACE_H_