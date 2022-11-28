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

#include "Revolver.h"

/******************************************************************************
 * defines
 *****************************************************************************/

/******************************************************************************
 * Type definitions
 *****************************************************************************/
typedef enum
{
    eRSU_STATE_INIT = 0,
    eRSU_STATE_POSITION_REFERENCE,
    eRSU_STATE_OPERATIONAL_IDLE,
    eRSU_STATE_OPERATIONAL_MOVING,
    eRSU_STATE_FAULT
}teRSU_STATE;

/** \brief RSU main variable struct*/
typedef struct
{
    teRSU_STATE eRsuState;  /*!< Current State */
    tsREVOLVER sRevolver;   /*!< Revolver variables */
}tsRSU;

#define tsRSU_DEFAULTS {eRSU_STATE_INIT, tsREVOLVER_DEFAULTS}

/******************************************************************************
 * Function declarations
 *****************************************************************************/

void RSUStateMachine (void);

#endif //_RSU_H_