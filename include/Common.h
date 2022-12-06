/**************************************************************************//**
 * \file Common.h
 * \author Roman Holderried
 *
 * \brief Common defines and type definitions.
 *
 * <b> History </b>
 * 	- 2022-11-28 - File creation
 *****************************************************************************/

#ifndef _COMMON_H_
#define _COMMON_H_
/******************************************************************************
 * Includes
 *****************************************************************************/
#include <stdint.h>
#include <stdbool.h>

/******************************************************************************
 * defines
 *****************************************************************************/

#define REVOLVER_VELOCITY_POSITIVE(vel) (vel < 0 ? vel * -1 : vel)
#define REVOLVER_VELOCITY_NEGATIVE(vel) (vel < 0 ? vel : vel * -1)

#define MAX_NUMBER_OF_SLOTS 8


/******************************************************************************
 * Type definitions
 *****************************************************************************/
typedef enum
{
    eCOMMAND_NONE = 0,
    eCOMMAND_MOVE_TO_SLOT,
    eCOMMAND_MOVE_TO_NEXT_FREE,
    eCOMMAND_POSITION_REFERENCE
}teCOMMAND_TYPE;

typedef struct
{
    uint8_t ui8TargetSlot;
    float   fTargetAcceleration;
    float   fTargetVelocity;
    float   fTargetPosition;
}tsMOTION_INFO;

#define tsMOTION_INFO_DEFAULTS {0, 0.0, 0.0, 0.0}

typedef struct
{
    teCOMMAND_TYPE eCommandType;
    tsMOTION_INFO sMotionInfo;
}tsCOMMAND_INFO;

#define tsCOMMAND_INFO_DEFAULTS {eCOMMAND_NONE, tsMOTION_INFO_DEFAULTS}

/******************************************************************************
 * Function declarations
 *****************************************************************************/

#endif //_COMMON_H_