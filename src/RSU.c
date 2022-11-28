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
void RSUStateMachine (void)
{
    switch (sRsu.eRsuState)
    {
        case eRSU_STATE_INIT:
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