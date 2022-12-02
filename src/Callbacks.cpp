/**************************************************************************//**
 * \file Callbacks.cpp
 * \author Roman Holderried
 *
 * \brief Callbacks of the RSU application.
 * 
 * <b> History </b>
 * 	- 2022-11-28 - File creation -
 *****************************************************************************/

/******************************************************************************
 * Includes
 *****************************************************************************/
#include <Arduino.h>
#include <HardwareSerial.h>
#include <cstdint>
#include "Callbacks.h"
#include <SCIMaster.h>
#include "SCIMasterConfig.h"
#include "MotorController.h"
#include "HWConfig.h"


/******************************************************************************
 * Defines
 *****************************************************************************/
#define SCI_SERIAL_EVENT_CALLBACK serialEvent ## SCI_UART_NUM 

/******************************************************************************
 * Imported globals
 *****************************************************************************/
extern HardwareSerial SCISerial;
extern tsSCI_VAR sSciVarTable[];
extern tsMOTCONTROLVARS sMotControlVars;

/******************************************************************************
 * Function definitions
 *****************************************************************************/

teTRANSFER_ACK ProcessSetVarTransfer (teREQUEST_ACKNOWLEDGE eAck, int16_t i16Num, uint16_t ui16ErrNum)
{
    if (eAck == eREQUEST_ACK_STATUS_SUCCESS)
    {
        ;
    }
    return eTRANSFER_ACK_SUCCESS;
}

//=============================================================================
teTRANSFER_ACK ProcessGetVarTransfer (teREQUEST_ACKNOWLEDGE eAck, int16_t i16Num, uint32_t ui32Data, uint16_t ui16ErrNum)
{
    uint16_t ui16Idx;

    /// TODO: Error handling?
    if (eAck != eREQUEST_ACK_STATUS_SUCCESS)
        return eTRANSFER_ACK_ABORT;

    // Search variable number in var table
    for(ui16Idx = 0; ui16Idx < NUMBER_OF_MOTORCONTROLLER_VARS; ui16Idx++)
    {
        if (sSciVarTable[ui16Idx].i16Num == i16Num)
            break;
    }

    // Copy data by using the appropriate data type
    if (ui16Idx < NUMBER_OF_MOTORCONTROLLER_VARS)
    {
        switch (sSciVarTable[ui16Idx].eDtype)
        {
            case eDTYPE_UINT8:
                *(uint8_t*)sSciVarTable[ui16Idx].pVar = (uint8_t)ui32Data;
                break;
            case eDTYPE_INT8:
                *(int8_t*)sSciVarTable[ui16Idx].pVar = *(int8_t*)&ui32Data;
                break;
            case eDTYPE_UINT16:
                *(uint16_t*)sSciVarTable[ui16Idx].pVar = (uint16_t)ui32Data;
                break;
            case eDTYPE_INT16:
                *(int16_t*)sSciVarTable[ui16Idx].pVar = *(int16_t*)&ui32Data;
                break;
            case eDTYPE_UINT32:
                *(uint32_t*)sSciVarTable[ui16Idx].pVar = (uint32_t)ui32Data;
                break;
            case eDTYPE_INT32:
                *(int32_t*)sSciVarTable[ui16Idx].pVar = *(int32_t*)&ui32Data;
                break;
            case eDTYPE_F32:
                *(float*)sSciVarTable[ui16Idx].pVar = *(float*)&ui32Data;
                break;
        }
        return eTRANSFER_ACK_SUCCESS;
    }
    else
        return eTRANSFER_ACK_ABORT;
}

//=============================================================================
teTRANSFER_ACK ProcessCommandTransfer(teREQUEST_ACKNOWLEDGE eAck, int16_t i16Num, uint32_t *pui32Data, uint8_t ui8DataCnt, uint16_t ui16ErrNum)
{
    teTRANSFER_ACK eTAck = eTRANSFER_ACK_ABORT;

    if (eAck == eREQUEST_ACK_STATUS_SUCCESS || eAck == eREQUEST_ACK_STATUS_SUCCESS_DATA)
    {
        // Only functions that return values must be handled here
        switch (i16Num)
        {
            case CMD_NUM_MOTIONCONTROLLER_START:
                break;
            case CMD_NUM_MOTIONCONTROLLER_FINISH_MVMNT:
                break;
        }
        eTAck = eTRANSFER_ACK_SUCCESS;
    }
    else
    {
        eTAck = eTRANSFER_ACK_ABORT;
    }

    return eTAck;
}

//=============================================================================
teTRANSFER_ACK ProcessUpstreamTransfer(int16_t i16Num, uint8_t *pui8Data, uint32_t ui32DataCnt)
{
    // if (eAck == eREQUEST_ACK_STATUS_SUCCESS)
    // {
    //     ;
    // }
    return eTRANSFER_ACK_SUCCESS;
}

//=============================================================================
void SCI_SERIAL_EVENT_CALLBACK(void)
{
    uint8_t ui8Data[RX_PACKET_LENGTH];
    
    SCIReceive(ui8Data, SCISerial.readBytes(ui8Data, SCISerial.available()));
}

//=============================================================================
uint8_t SCISerialWriteNonBlocking(uint8_t *pui8Buf, uint8_t ui8Len)
{
    if(SCISerial.availableForWrite() >= ui8Len)
    {
        SCISerial.write(pui8Buf, ui8Len);
        return ui8Len;
    }
    else
        return 0;
}

//=============================================================================
bool SCISerialGetBusyState(void)
{
    bool bBufferSent;

    #if TX_PACKET_LENGTH > 126
    bBufferSent = (bool)(SCISerial.availableForWrite() >= TX_PACKET_LENGTH + 2);
    #else
    bBufferSent = (bool)(SCISerial.availableForWrite() >= 128)
    #endif

    return bBufferSent;
}