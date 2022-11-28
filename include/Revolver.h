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

/******************************************************************************
 * defines
 *****************************************************************************/
/** Default offset of slot 1 relative to the reference position */
#define SLOT1_DEFAULT_OFFSET 0.0f

/******************************************************************************
 * Type definitions
 *****************************************************************************/
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

    tsSLOT sSlots[MAX_NUMBER_OF_SLOTS];         /*!< Slot info array (Index identifies slot number => index 0 -> slot 1) */
}tsREVOLVER;

#define tsREVOLVER_DEFAULTS {   {false, 0.0, {0.0}, {SLOT1_DEFAULT_OFFSET}}, \
                                {tsSLOT_DEFAULTS} \
}
/******************************************************************************
 * Function declarations
 *****************************************************************************/

#endif //_REVOLVER_H_