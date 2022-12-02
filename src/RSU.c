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

#include "RSU.h"

/******************************************************************************
 * Globals
 *****************************************************************************/
tsRSU sRsu = tsRSU_DEFAULTS;

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