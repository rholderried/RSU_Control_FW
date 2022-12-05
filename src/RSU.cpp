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
    sStateTransitionTimer.pfnTimer_cb = _RSUTransitionToState;

    // Append the state transition timer
    sRsu.sStateControl.i8StateTransitionTimerIdx = AppendTimer32Bit(&sStateTransitionTimer);

    bSuccess &= (sRsu.sStateControl.i8StateTransitionTimerIdx >= 0);

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
                _RSUChangeState(eRSU_STATE_POSITION_REFERENCE, INIT_TO_POSITION_REFERENCE_TIMEOUT_MS);

            break;

        case eRSU_STATE_POSITION_REFERENCE:
            {
                switch(sRsu.sPosRefence.eRefState)
                {
                    case eREF_STATE_INIT:
                        MotCtrlGetVar(MOTCTRL_VAR_NUM_MOTION_CONTROLLER_SCALE, [](){_RSUPosRefChangeState(eREF_STATE_START_MOVEMENT);});
                        break;

                    // Submit the command for starting reference position search movement
                    case eREF_STATE_START_MOVEMENT:

                        MotCtrlStartMovement(   REFERENCE_POSITION_SEARCH_ANGULAR_ACCELERATION,
                                                REFERENCE_POSITION_SEARCH_ANGULAR_VELOCITY,
                                                0.0, true, 
                                                [](){_RSUPosRefChangeState(eREF_STATE_WAIT_FOR_POS_EDGE);});
                        break;

                    case eREF_STATE_WAIT_FOR_POS_EDGE:
                        break;
                    case eREF_STATE_WAIT_FOR_NEG_EDGE:
                        break;

                    case eREF_STATE_STOP_MOVEMENT:
                        MotCtrlStopMovement([](){_RSUPosRefChangeState(eREF_STATE_READY);});
                        break;

                    case eREF_STATE_READY:
                        {
                            float fPos1 = (float)sRsu.sPosRefence.i32RefIncs[0] / sMotCtrlVars.fMotionControllerScale;
                            float fPos2 = (float)sRsu.sPosRefence.i32RefIncs[1] / sMotCtrlVars.fMotionControllerScale;
                            
                            ///@todo Calculate Revolver position

                            _RSUChangeState(eRSU_STATE_OPERATIONAL_IDLE, POSITION_REFERENCE_TO_OPERATIONAL_IDLE_TIMEOUT_MS);
                        }
                        break;
                    
                    default:
                        break;
                }
            }
            
            break;
        case eRSU_STATE_OPERATIONAL_IDLE:
            break;
        case eRSU_STATE_OPERATIONAL_MOVING:
            break;
        case eRSU_STATE_FAULT:
            break;
        default:
            break;
    }    
}

//=============================================================================
void _RSUTransitionToState (void)
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
            attachInterrupt(REF_SENSOR_PIN, RefSensorOnChangeISR, CHANGE);
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

//=============================================================================
void ReferenceSensorEdgeCb(bool bEdge)
{
    if (bEdge == REFERENCE_SENSOR_EDGE_POS && sRsu.sPosRefence.eRefState == eREF_STATE_WAIT_FOR_POS_EDGE)
    {
        MotCtrlGetVar(MOTCTRL_VAR_NUM_ACTUAL_POSITION_INCREMENTS, [](){ _RSUPosRefChangeState(eREF_STATE_WAIT_FOR_NEG_EDGE);});
    }
    else if (bEdge == REFERENCE_SENSOR_EDGE_NEG && sRsu.sPosRefence.eRefState == eREF_STATE_WAIT_FOR_NEG_EDGE)
    {
        MotCtrlGetVar(MOTCTRL_VAR_NUM_ACTUAL_POSITION_INCREMENTS, [](){ _RSUPosRefChangeState(eREF_STATE_READY);});
    }
}