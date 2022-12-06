/**************************************************************************//**
 * \file Revolver.h
 * \author Roman Holderried
 *
 * \brief Revolver function declarations, type definitions, etc.
 *
 * <b> History </b>
 * 	- 2022-11-28 - File creation
 *****************************************************************************/

#ifndef _REVOLVER_H_
#define _REVOLVER_H_
/******************************************************************************
 * Includes
 *****************************************************************************/
#include <stdint.h>
#include <stdbool.h>

#include "Common.h"
#include "Configuration.h"

/******************************************************************************
 * defines
 *****************************************************************************/

#define MOTIONCONTROLLER_ACTIVE_POLLING_RATE_MS 200

/******************************************************************************
 * Type definitions
 *****************************************************************************/
typedef enum
{
    eREVOLVER_MOTION_STATE_IDLE = 0,
    eREVOLVER_MOTION_STATE_CHECK_MOTIONCONTROLLER,
    eREVOLVER_MOTION_STATE_START_MOTIONCONTROLLER,
    eREVOLVER_MOTION_STATE_FINISH_MOVEMENT
}teREVOLVER_MOTION_STATES;

/** \brief Sample cylinder slot data */
typedef struct
{
    bool    bInserted;  /*!< Flags true if cylinder is inserted */
    uint8_t ui8Type;    /*!< Cylinder type that is inserted */
}tsSLOT;

#define tsSLOT_DEFAULTS {false, 0}

typedef struct
{
    struct
    {
        bool    bValid;                         /*!< Flags true if revolver has already been referenced */
        float   fActual;                        /*!< Position of the revolver on last request */
        float   fSlots[MAX_NUMBER_OF_SLOTS];    /*!< Slot positions */
        struct
        {
            float fSlot1Offset;                 /*!< Offset of slot 1 relative to the reference position */
        }sPar;
    }sPosition;

    struct
    {
        bool bAbort;
        bool bLock;
        bool bMotionFinished;
        int8_t i8TimerIdx;
        teREVOLVER_MOTION_STATES eMotionStates;
        tsMOTION_INFO sMotionInfo;
    }sMotion;

    tsSLOT sSlots[MAX_NUMBER_OF_SLOTS];         /*!< Slot info array (Index identifies slot number => index 0 -> slot 1) */
}tsREVOLVER;

#define tsREVOLVER_DEFAULTS {   {false, 0.0, {0.0}, {SLOT1_DEFAULT_OFFSET_FROM_REFERENCE}}, \
                                {false, false, false, -1, eREVOLVER_MOTION_STATE_IDLE, tsMOTION_INFO_DEFAULTS}, \
                                {tsSLOT_DEFAULTS} \
}
/******************************************************************************
 * Function declarations
 *****************************************************************************/
bool InitRevolver (tsREVOLVER *psRevolver);

bool RevolverStartMovementToSlot(tsREVOLVER *psRevolver, tsMOTION_INFO sMotionInfo);

void RevolverMotionStateMachine(tsREVOLVER *psRevolver);

void RevolverInitSlots (tsREVOLVER *psRevolver, float fRefPos);

void _RevolverSetMotionState(tsREVOLVER *psRevolver, teREVOLVER_MOTION_STATES eNewState);

#endif //_REVOLVER_H_