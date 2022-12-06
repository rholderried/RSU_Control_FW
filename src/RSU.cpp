/**************************************************************************//**
 * \file RSU.c
 * \author Roman Holderried
 *
 * \brief RSU control functions
 *
 * <b> History </b>
 * 	- 2022-11-28 - File creation
 *****************************************************************************/

/******************************************************************************
 * Includes
 *****************************************************************************/
#include <Arduino.h>
#include "Callbacks.h"
#include "ReferenceSensor.h"
#include "MotorController.h"
#include "Configuration.h"
#include "HWConfig.h"
#include "Interface.h"

#include "RSU.h"

/******************************************************************************
 * Globals
 *****************************************************************************/
tsRSU sRsu = tsRSU_DEFAULTS;

extern tsMOTCONTROLVARS sMotCtrlVars;

/******************************************************************************
 * Function definitions
 *****************************************************************************/
void RSUInit(void)
{
    bool bSuccess = true;

    tTIMERVAR32 sStateTransitionTimer = tTIMERVAR32_DEFAULTS;
    sStateTransitionTimer.bOneShot = true;

    // Append the state transition timer
    sRsu.sStateControl.i8StateTransitionTimerIdx = AppendTimer32Bit(&sStateTransitionTimer);

    bSuccess &= (sRsu.sStateControl.i8StateTransitionTimerIdx >= 0);

    if (bSuccess)
    {
        Timer32AttachCallback(sRsu.sStateControl.i8StateTransitionTimerIdx, _RSUTransitionToState, NULL);
    }

    InitRefSensor(&sRsu.sReferenceSensor);

    bSuccess &= InitRevolver(&sRsu.sRevolver);

    sRsu.bInitialized = bSuccess;
}


//=============================================================================
void RSUStateMachine (void)
{
    // First: Get out if state transition is not finished
    if (sRsu.sStateControl.eRsuState != sRsu.sStateControl.eNextState)
        return;

    switch (sRsu.sStateControl.eRsuState)
    {
        case eRSU_STATE_INIT:
            // Wait a little bit so the Motor controller has time to start up

            /// @todo Check if SCI interface is up and running
            if (sRsu.bInitialized)
                _RSUChangeState(eRSU_STATE_POSITION_REFERENCE, INIT_TO_PREOP_TIMEOUT_MS);

            break;
        
        case eRSU_STATE_PREOP:
            /// @todo Make sure all variables are set the right way?

            // Switch on the motor controller
            MotCtrlSwitchOnMotorOutput([](void*){_RSUChangeState(eRSU_STATE_POSITION_REFERENCE, PREOP_TO_POSITION_REFERENCE_TIMEOUT_MS);}, NULL);


        case eRSU_STATE_POSITION_REFERENCE:
            {
                switch(sRsu.sPosRefence.eRefState)
                {
                    case eREF_STATE_INIT:
                        MotCtrlGetVar(MOTCTRL_VAR_NUM_MOTION_CONTROLLER_SCALE, [](void*){_RSUPosRefChangeState(eREF_STATE_START_MOVEMENT);}, NULL);
                        break;

                    // Submit the command for starting reference position search movement
                    case eREF_STATE_START_MOVEMENT:

                        MotCtrlStartMovement(   REFERENCE_POSITION_SEARCH_ANGULAR_ACCELERATION,
                                                REFERENCE_POSITION_SEARCH_ANGULAR_VELOCITY,
                                                0.0, true, 
                                                [](void*){_RSUPosRefChangeState(eREF_STATE_WAIT_FOR_POS_EDGE);}, NULL);
                        break;

                    case eREF_STATE_WAIT_FOR_POS_EDGE:
                        // Nothing to do, interrupt triggered
                        break;

                    case eREF_STATE_WAIT_FOR_NEG_EDGE:
                        // Nothing to do, interrupt triggered
                        break;

                    case eREF_STATE_STOP_MOVEMENT:
                        MotCtrlStopMovement([](void*){_RSUPosRefChangeState(eREF_STATE_READY);}, NULL);
                        break;

                    case eREF_STATE_READY:
                        {
                            float fPos1 = (float)sRsu.sPosRefence.i32RefIncs[0] / sMotCtrlVars.fMotionControllerScale;
                            float fPos2 = (float)sRsu.sPosRefence.i32RefIncs[1] / sMotCtrlVars.fMotionControllerScale;
                            
                            RevolverInitSlots(&sRsu.sRevolver, (fPos1 + fPos2) / 2.0f);

                            _RSUChangeState(eRSU_STATE_OPERATIONAL_IDLE, POSITION_REFERENCE_TO_OPERATIONAL_IDLE_TIMEOUT_MS);
                        }
                        break;
                    
                    default:
                        break;
                }
            }
            
            break;
        case eRSU_STATE_OPERATIONAL_IDLE:
            {
                switch (sRsu.sNextCmd.eCommandType)
                {
                    case eCOMMAND_NONE:
                        break;
                    case eCOMMAND_MOVE_TO_SLOT:

                        if(RevolverStartMovementToSlot(&sRsu.sRevolver, sRsu.sNextCmd.sMotionInfo))
                            sRsu.sStateControl.eRsuState = eRSU_STATE_OPERATIONAL_MOVING;
                        break;

                    default:
                        break;
                }
            }
            break;

        case eRSU_STATE_OPERATIONAL_MOVING:
            RevolverMotionStateMachine(&sRsu.sRevolver);

            // Return to the Operational Idle state if the movement has been finished
            if (sRsu.sRevolver.sMotion.eMotionStates == eREVOLVER_MOTION_STATE_IDLE)
            {
                _RSUChangeState (eRSU_STATE_OPERATIONAL_IDLE, 0);
                sRsu.sNextCmd.eCommandType = eCOMMAND_NONE;
            }
            break;

        case eRSU_STATE_FAULT:
            break;

        default:
            break;
    }    
}

//=============================================================================
void ReferenceSensorEdgeISR(void)
{
    bool bLastState = sRsu.sReferenceSensor.bLaststate;
    bool bNewState = GetRefSensorState(&sRsu.sReferenceSensor);
    
    // Edge appeared (For convenience, should not be necessary)
    if (bLastState != bNewState)
    {

        // Positive edge
        if (bNewState == REFERENCE_SENSOR_STATE_ON && sRsu.sPosRefence.eRefState == eREF_STATE_WAIT_FOR_POS_EDGE)
            MotCtrlGetVar(MOTCTRL_VAR_NUM_ACTUAL_POSITION_INCREMENTS, [](void*){ _RSUPosRefChangeState(eREF_STATE_WAIT_FOR_NEG_EDGE);}, NULL);

        // Negative edge
        else if (bNewState == REFERENCE_SENSOR_STATE_OFF && sRsu.sPosRefence.eRefState == eREF_STATE_WAIT_FOR_NEG_EDGE)
            MotCtrlGetVar(MOTCTRL_VAR_NUM_ACTUAL_POSITION_INCREMENTS, [](void*){ _RSUPosRefChangeState(eREF_STATE_READY);}, NULL);

    }
}

//=============================================================================
bool RegisterCommand (tsCOMMAND_INFO sCmdInfo)
{
    if (sRsu.sStateControl.eRsuState == eRSU_STATE_OPERATIONAL_IDLE)
    {
        sRsu.sNextCmd = sCmdInfo;
        return true;
    }
    else
        return false;
}

//=============================================================================
void _RSUTransitionToState (void * pUserData)
{
    sRsu.sStateControl.eRsuState = sRsu.sStateControl.eNextState;
}

//=============================================================================
void _RSUChangeState (teRSU_STATE eNewState, uint32_t ui32Timeout_ms)
{
    sRsu.sStateControl.eNextState = eNewState;
    Timer32BitSetValue(sRsu.sStateControl.i8StateTransitionTimerIdx, true, ui32Timeout_ms);
}

//=============================================================================
void _RSUPosRefChangeState(teREFERENCE_STATE eNewState)
{
    switch(sRsu.sPosRefence.eRefState)
    {
        case eREF_STATE_INIT:
            // Switch on the interrupt for the reference sensor
            attachInterrupt(REF_SENSOR_PIN, ReferenceSensorEdgeISR, CHANGE);
            break;

        case eREF_STATE_START_MOVEMENT:
            break;

        case eREF_STATE_WAIT_FOR_POS_EDGE:
            sRsu.sPosRefence.i32RefIncs[0] = sMotCtrlVars.i32ActualPositionIncrements;
            break;

        case eREF_STATE_WAIT_FOR_NEG_EDGE:
            sRsu.sPosRefence.i32RefIncs[1] = sMotCtrlVars.i32ActualPositionIncrements;
            detachInterrupt(REF_SENSOR_PIN);
            break;
    }
    sRsu.sPosRefence.eRefState = eNewState;
}

