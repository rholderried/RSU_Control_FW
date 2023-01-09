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
#define VALUE_THRESHOLD_NOT_INSERTED    4000
#define VALUE_THRESHOLD_TUBE_TYPE_8     3500
#define VALUE_THRESHOLD_TUBE_TYPE_7     3000
#define VALUE_THRESHOLD_TUBE_TYPE_6     2500
#define VALUE_THRESHOLD_TUBE_TYPE_5     2000
#define VALUE_THRESHOLD_TUBE_TYPE_4     1500
#define VALUE_THRESHOLD_TUBE_TYPE_3     1000
#define VALUE_THRESHOLD_TUBE_TYPE_2     500
#define VALUE_THRESHOLD_TUBE_TYPE_1     0

#define VALUE_THRESHOLDS {  VALUE_THRESHOLD_TUBE_TYPE_1,\
                            VALUE_THRESHOLD_TUBE_TYPE_2,\
                            VALUE_THRESHOLD_TUBE_TYPE_3,\
                            VALUE_THRESHOLD_TUBE_TYPE_4,\
                            VALUE_THRESHOLD_TUBE_TYPE_5,\
                            VALUE_THRESHOLD_TUBE_TYPE_6,\
                            VALUE_THRESHOLD_TUBE_TYPE_7,\
                            VALUE_THRESHOLD_TUBE_TYPE_8,\
                            VALUE_THRESHOLD_NOT_INSERTED}

#define ADC_PINS {  SLOT_1_ID_PIN,\
                    SLOT_2_ID_PIN,\
                    SLOT_3_ID_PIN,\
                    SLOT_4_ID_PIN,\
                    SLOT_5_ID_PIN,\
                    SLOT_6_ID_PIN,\
                    SLOT_7_ID_PIN,\
                    SLOT_8_ID_PIN}

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
    int8_t  i8Type;    /*!< Cylinder type that is inserted */
}tsSLOT;

#define tsSLOT_DEFAULTS {false, -1}

typedef uint16_t (*tGET_ADC_VALUES_CB)(uint8_t ui8Pin);

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

    uint8_t ui8CurrentSlot;
    tsSLOT  sSlots[MAX_NUMBER_OF_SLOTS];         /*!< Slot info array (Index identifies slot number => index 0 -> slot 1) */
    tGET_ADC_VALUES_CB cbGetADCValues;          /*!< Function to call to get the Slot ADC values */
}tsREVOLVER;

#define tsREVOLVER_DEFAULTS {   {false, 0.0, {0.0}, {SLOT1_DEFAULT_OFFSET_FROM_REFERENCE}}, \
                                {false, false, false, -1, eREVOLVER_MOTION_STATE_IDLE, tsMOTION_INFO_DEFAULTS}, \
                                0,\
                                {tsSLOT_DEFAULTS}, \
                                NULL,\
}
/******************************************************************************
 * Function declarations
 *****************************************************************************/
bool InitRevolver (tsREVOLVER *psRevolver, tGET_ADC_VALUES_CB cbGetADCValues);

bool RevolverStartMovementToSlot(tsREVOLVER *psRevolver, tsMOTION_INFO sMotionInfo);

void RevolverMotionStateMachine(tsREVOLVER *psRevolver);

void RevolverInitSlots (tsREVOLVER *psRevolver, float fRefPos);

void RevolverPollSlots (tsREVOLVER *psRevolver);

void _RevolverSetMotionState(tsREVOLVER *psRevolver, teREVOLVER_MOTION_STATES eNewState);

#endif //_REVOLVER_H_