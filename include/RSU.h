/**************************************************************************//**
 * \file RSU.h
 * \author Roman Holderried
 *
 * \brief RSU control function declarations, type definitions,....
 *
 * <b> History </b>
 * 	- 2022-11-28 - File creation
 *****************************************************************************/

#ifndef _RSU_H_
#define _RSU_H_
/******************************************************************************
 * Includes
 *****************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include <Timer32Bit.h>

#include "Revolver.h"

/******************************************************************************
 * defines
 *****************************************************************************/
#define INIT_TO_POSITION_REFERENCE_TIMEOUT_MS 2000

/******************************************************************************
 * Type definitions
 *****************************************************************************/
typedef enum
{
    eRSU_STATE_INIT                 = 0,
    eRSU_STATE_POSITION_REFERENCE,
    eRSU_STATE_OPERATIONAL_IDLE,
    eRSU_STATE_OPERATIONAL_MOVING,
    eRSU_STATE_FAULT
}teRSU_STATE;

/** \brief RSU main variable struct*/
typedef struct
{
    bool bInitialized;

    struct
    {
        teRSU_STATE eRsuState;                  /*!< Current State */
        teRSU_STATE eNextState;                 /*!< Next State */
        int8_t      i8StateTransitionTimerIdx;
    }sStateControl;

    tsREVOLVER sRevolver;   /*!< Revolver variables */
}tsRSU;

#define tsRSU_DEFAULTS {{eRSU_STATE_INIT, eRSU_STATE_INIT,  -1}, tsREVOLVER_DEFAULTS}

/******************************************************************************
 * Function declarations
 *****************************************************************************/

void RSUInit(void);
void RSUStateMachine (void);
void SetStateTransitionTimeout(uint32_t ui32Timeout_ms);
void _RSUTransitionToState (void);
void _RSUChangeState (teRSU_STATE eState, uint32_t ui32Timeout_ms);

#endif //_RSU_H_