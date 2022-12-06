/**************************************************************************//**
 * \file Revolver.cpp
 * \author Roman Holderried
 *
 * \brief Functions controlling the movements / Sample tubes.
 * 
 * <b> History </b>
 * 	- 2022-12-05 - File creation
 *****************************************************************************/

/******************************************************************************
 * Includes
 *****************************************************************************/
#include <cstdint>
#include <cstdbool>
#include <Timer32Bit.h>
#include "HWConfig.h"
#include "Configuration.h"
#include "MotorController.h"
#include "Revolver.h"

/******************************************************************************
 * Defines
 *****************************************************************************/

/******************************************************************************
 * Imported Globals
 *****************************************************************************/
extern tsMOTCONTROLVARS sMotCtrlVars;

/******************************************************************************
 * Function definitions
 *****************************************************************************/
bool InitRevolver (tsREVOLVER *psRevolver)
{
    bool bSuccess = true;
    // Assign the GPIO
    // psRefSens->sCallbacks = sCallbacks;

    // psRefSens->bLaststate = (bool)digitalRead(REF_SENSOR_PIN);

    // Initialize the Motion timer
    {
        tTIMERVAR32 sTemp = tTIMERVAR32_DEFAULTS;
        sTemp.bOneShot = true;
        psRevolver->sMotion.i8TimerIdx = AppendTimer32Bit(&sTemp);
    }

    bSuccess &= (psRevolver->sMotion.i8TimerIdx >= 0);

    return bSuccess;
}

//=============================================================================
void RevolverInitSlots (tsREVOLVER *psRevolver, float fRefPos)
{
    float fPosIncrement = 2.0f * 3.14159265f / NUMBER_OF_SLOTS;

    for (uint8_t i = 0; i < NUMBER_OF_SLOTS; i++)
    {
        psRevolver->sPosition.fSlots[i] = i * fPosIncrement + psRevolver->sPosition.sPar.fSlot1Offset;
    }
}

//=============================================================================
bool RevolverStartMovementToSlot(tsREVOLVER *psRevolver, tsMOTION_INFO sMotionInfo)
{
    if (sMotionInfo.ui8TargetSlot > NUMBER_OF_SLOTS ||
        psRevolver->sMotion.eMotionStates != eREVOLVER_MOTION_STATE_IDLE)
        return false;

    sMotionInfo.fTargetPosition = psRevolver->sPosition.fSlots[sMotionInfo.ui8TargetSlot - 1];

    if (sMotionInfo.fTargetAcceleration == 0.0)
        sMotionInfo.fTargetAcceleration = MOVEMENT_ANGULAR_ACCELERATION;
    else
        sMotionInfo.fTargetAcceleration = sMotionInfo.fTargetAcceleration;

    if (sMotionInfo.fTargetVelocity == 0.0)
        sMotionInfo.fTargetVelocity = MOVEMENT_ANGULAR_VELOCITY;
    else
        sMotionInfo.fTargetVelocity = sMotionInfo.fTargetVelocity;

    psRevolver->sMotion.sMotionInfo = sMotionInfo;

    psRevolver->sMotion.eMotionStates = eREVOLVER_MOTION_STATE_START_MOTIONCONTROLLER;
    
    return true;
}

//=============================================================================
void RevolverMotionStateMachine(tsREVOLVER *psRevolver)
{
    if (psRevolver->sMotion.bLock)
        return;

    switch (psRevolver->sMotion.eMotionStates)
    {
        case eREVOLVER_MOTION_STATE_IDLE:
            break;

        case eREVOLVER_MOTION_STATE_START_MOTIONCONTROLLER:
            // Automatic retry if starting the motion did not work (because the callback will never get called)
            if (!psRevolver->sMotion.bAbort)
            {
                MotCtrlStartMovement(   psRevolver->sMotion.sMotionInfo.fTargetAcceleration,
                                        psRevolver->sMotion.sMotionInfo.fTargetVelocity,
                                        psRevolver->sMotion.sMotionInfo.fTargetPosition,
                                        false, [](void* psRevolver){_RevolverSetMotionState((tsREVOLVER*)psRevolver, eREVOLVER_MOTION_STATE_FINISH_MOVEMENT);},
                                        psRevolver);
            }
            else
            {
                _RevolverSetMotionState(psRevolver, eREVOLVER_MOTION_STATE_IDLE);
            }
            break;
        
        case eREVOLVER_MOTION_STATE_FINISH_MOVEMENT:
            {
                if (!psRevolver->sMotion.bAbort)
                {
                    // Poll the Motioncontroller active flag
                    MotCtrlGetVar(  MOTCTRL_VAR_NUM_MOTIONCONTROLLER_ACTIVE_FLAG, 
                                    [](void* pDat){*(bool*)pDat = sMotCtrlVars.bMotionControllerActive;},
                                    &psRevolver->sMotion.bMotionFinished);
                }
                else
                {
                    /// @todo Abort the movement (Motioncontroller soft stop function must be implemented)
                }
                
                // Wait a bit if the motion is not finished
                if(!psRevolver->sMotion.bMotionFinished)
                {
                    // Poll every MOTIONCONTROLLER_ACTIVE_POLLING_RATE_MS milliseconds if movement has finished
                    Timer32AttachCallback(  psRevolver->sMotion.i8TimerIdx, 
                                            [](void* pUserData){*(bool*)pUserData = false;},
                                            &psRevolver->sMotion.bLock);

                    Timer32BitSetValue(psRevolver->sMotion.i8TimerIdx, true, MOTIONCONTROLLER_ACTIVE_POLLING_RATE_MS);

                    psRevolver->sMotion.bLock = true;
                }
                // Motion finished
                else
                {
                    _RevolverSetMotionState(psRevolver, eREVOLVER_MOTION_STATE_IDLE);
                }
                
            }
            break;

        default:
            break;
    }
}

//=============================================================================
void _RevolverSetMotionState(tsREVOLVER *psRevolver, teREVOLVER_MOTION_STATES eNewState)
{
    switch (eNewState)
    {
        case eREVOLVER_MOTION_STATE_IDLE:
            {
                psRevolver->sMotion.eMotionStates = eREVOLVER_MOTION_STATE_IDLE;
                psRevolver->sMotion.bMotionFinished = false;
                psRevolver->sMotion.bAbort = false;
                psRevolver->sMotion.bLock = false;
                psRevolver->sMotion.sMotionInfo = (tsMOTION_INFO)tsMOTION_INFO_DEFAULTS;
            }
            break;

        case eREVOLVER_MOTION_STATE_START_MOTIONCONTROLLER:
            psRevolver->sMotion.eMotionStates = eREVOLVER_MOTION_STATE_START_MOTIONCONTROLLER;
            break;
        
        case eREVOLVER_MOTION_STATE_FINISH_MOVEMENT:
            psRevolver->sMotion.eMotionStates = eREVOLVER_MOTION_STATE_FINISH_MOVEMENT;
            break;

        default:
            break;
    }
}