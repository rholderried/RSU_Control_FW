/**************************************************************************//**
 * \file MotorController.cpp
 * \author Roman Holderried
 *
 * \brief Motorcontroller related functionality (SCI Variables and -Commands).
 * 
 * <b> History </b>
 * 	- 2022-11-28 - File creation -
 *****************************************************************************/

/******************************************************************************
 * Includes
 *****************************************************************************/
#include <cstdint>
#include <SCIMaster.h>
#include "MotorController.h"

/******************************************************************************
 * Defines
 *****************************************************************************/

/******************************************************************************
 * Private Globals
 *****************************************************************************/
static tsMOTORCONTROLLER sMotCtrl = tsMOTORCONTROLLER_DEFAULTS;

/******************************************************************************
 * Exported Globals
 *****************************************************************************/

tsMOTCONTROLVARS sMotCtrlVars = tsMOTCONTROLVARS_DEFAULTS;

tsSCI_VAR sSciVarTable[NUMBER_OF_MOTORCONTROLLER_VARS] =
{   
    {  
        .i16Num = MOTCTRL_VAR_NUM_MOTION_CONTROLLER_SCALE,
        .pVar   = &sMotCtrlVars.fMotionControllerScale,
        .eDtype = eDTYPE_F32
    },
    {  
        .i16Num = MOTCTRL_VAR_NUM_ACTUAL_POSITION_INCREMENTS,
        .pVar   = &sMotCtrlVars.i32ActualPositionIncrements,
        .eDtype = eDTYPE_INT32
    }
};


/******************************************************************************
 * Function definitions
 *****************************************************************************/
bool MotCtrlGetVar (int16_t i16Num, void (*TransferRdyCb)(void))
{
    if (SCIGetProtocolState() == ePROTOCOL_IDLE)
    {
        SCIRequestGetVar(i16Num);
        sMotCtrl.sTransfer.TransferRdyCb = TransferRdyCb;

        // Flag that transfer has been started
        return true;
    }
    else
        return false;
}

//=============================================================================
bool MotCtrlStartMovement(float fTargetAcceleration, float fTargetVelocity, float fTargetPosition, bool bReferenceMovement, void (*TransferRdyCb)(void))
{
    if (SCIGetProtocolState() == ePROTOCOL_IDLE)
    {
        // Fill in the payload
        tuREQUESTVALUE uVals[5] =   {{.ui32_hex = 0},
                                    {.f_float = fTargetAcceleration},
                                    {.f_float = fTargetVelocity},
                                    {.f_float = fTargetPosition},
                                    {.ui32_hex = bReferenceMovement}};

        SCIRequestCommand(MOTCTRL_CMD_NUM_MOTIONCONTROLLER_START, uVals, 5);

        sMotCtrl.sTransfer.TransferRdyCb = TransferRdyCb;

        // Flag that transfer has been started
        return true;
    }
    else
        return false;
}

//=============================================================================
bool MotCtrlStopMovement(void (*TransferRdyCb)(void))
{
    if (SCIGetProtocolState() == ePROTOCOL_IDLE)
    {
        // Fill in the payload
        tuREQUESTVALUE uVals[1] =   {{.ui32_hex = 0}};

        SCIRequestCommand(MOTCTRL_CMD_NUM_MOTIONCONTROLLER_FINISH_MVMNT, uVals, 1);

        sMotCtrl.sTransfer.TransferRdyCb = TransferRdyCb;

        // Flag that transfer has been started
        return true;
    }
    else
        return false;
}

//=============================================================================
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

        // Call the Transfer callback
        if (sMotCtrl.sTransfer.TransferRdyCb)
            sMotCtrl.sTransfer.TransferRdyCb();

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
            case MOTCTRL_CMD_NUM_MOTIONCONTROLLER_START:
                break;
            case MOTCTRL_CMD_NUM_MOTIONCONTROLLER_FINISH_MVMNT:
                break;
        }

        if (sMotCtrl.sTransfer.TransferRdyCb)
            sMotCtrl.sTransfer.TransferRdyCb();
            
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