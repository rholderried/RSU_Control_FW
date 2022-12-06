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
#include "ReferenceSensor.h"

/******************************************************************************
 * defines
 *****************************************************************************/
#define INIT_TO_PREOP_TIMEOUT_MS 2000
#define PREOP_TO_POSITION_REFERENCE_TIMEOUT_MS 100
#define POSITION_REFERENCE_TO_OPERATIONAL_IDLE_TIMEOUT_MS 2000

/******************************************************************************
 * Type definitions
 *****************************************************************************/
typedef enum
{
    eRSU_STATE_INIT                 = 0,
    eRSU_STATE_PREOP,
    eRSU_STATE_POSITION_REFERENCE,
    eRSU_STATE_OPERATIONAL_IDLE,
    eRSU_STATE_OPERATIONAL_MOVING,
    eRSU_STATE_FAULT
}teRSU_STATE;

typedef enum
{
    eREF_STATE_INIT = 0,
    eREF_STATE_START_MOVEMENT,
    eREF_STATE_WAIT_FOR_POS_EDGE,
    eREF_STATE_WAIT_FOR_NEG_EDGE,
    eREF_STATE_STOP_MOVEMENT,
    eREF_STATE_READY
}teREFERENCE_STATE;

/** \brief RSU main variable struct*/
typedef struct
{
    bool bInitialized;
    tsCOMMAND_INFO sNextCmd;

    struct
    {
        teRSU_STATE eRsuState;                  /*!< Current State */
        teRSU_STATE eNextState;                 /*!< Next State */
        int8_t      i8StateTransitionTimerIdx;
    }sStateControl;

    struct
    {
        int32_t i32RefIncs[2];
        teREFERENCE_STATE eRefState;
    }sPosRefence;

    tsREVOLVER sRevolver;                   /*!< Revolver variables */
    tsREFERENCE_SENSOR sReferenceSensor;    /*!< Reference Sensor variables */
}tsRSU;

#define tsRSU_DEFAULTS {false,\
                        tsCOMMAND_INFO_DEFAULTS,\
                        {eRSU_STATE_INIT, eRSU_STATE_INIT,  -1},\
                        {eREF_STATE_INIT},\
                        tsREVOLVER_DEFAULTS,\
                        tsREFERENCE_SENSOR_DEFAULTS\
                        }

/******************************************************************************
 * Function declarations
 *****************************************************************************/

void RSUInit(void);
void RSUStateMachine (void);
void SetStateTransitionTimeout(uint32_t ui32Timeout_ms);
void ReferenceSensorEdgeISR(void);
bool RegisterCommand (tsCOMMAND_INFO);
void _RSUTransitionToState (void * pUserData);
void _RSUChangeState (teRSU_STATE eState, uint32_t ui32Timeout_ms);
void _RSUPosRefChangeState(teREFERENCE_STATE eNewState);
// void RSUStartMovement();

#endif //_RSU_H_