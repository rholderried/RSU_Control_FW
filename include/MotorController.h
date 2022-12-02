/**************************************************************************//**
 * \file MotorController.h
 * \author Roman Holderried
 *
 * \brief Motorcontroller related functionality (SCI Variables and -Commands).
 *
 * <b> History </b>
 * 	- 2022-12-01 - File creation
 *****************************************************************************/

#ifndef _MOTORCONTROLLER_H_
#define _MOTORCONTROLLER_H_
/******************************************************************************
 * Includes
 *****************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include <cstddef>

/******************************************************************************
 * SCI Variables
 *****************************************************************************/

#define NUMBER_OF_MOTORCONTROLLER_VARS 2

// Motorcontroller external variables
#define SCI_VAR_NUM_MOTION_CONTROLLER_SCALE     59
#define SCI_VAR_NUM_ACTUAL_POSITION_INCREMENTS  12

// Motorcontroller external functions
#define CMD_NUM_MOTIONCONTROLLER_START          15
#define CMD_NUM_MOTIONCONTROLLER_FINISH_MVMNT   16

/******************************************************************************
 * Type definitions
 *****************************************************************************/

/** \brief Data type enumerator.*/
typedef enum
{
    eDTYPE_UINT8  = 0,
    eDTYPE_INT8   = 1,
    eDTYPE_UINT16 = 2,
    eDTYPE_INT16  = 3,
    eDTYPE_UINT32 = 4,
    eDTYPE_INT32  = 5,
    eDTYPE_F32    = 6
}teDTYPE;

typedef struct 
{
    int16_t     i16Num;
    void        *pVar;
    teDTYPE     eDtype;
}tsSCI_VAR;

#define tsSCI_VAR_DEFAULTS {-1, NULL, eDTYPE_UINT8}

typedef struct
{
    float   fMotionControllerScale;
    int32_t i32ActualPositionIncrements;
}tsMOTCONTROLVARS;

#define tsMOTCONTROLVARS_DEFAULTS {0};

/******************************************************************************
 * Function declarations
 *****************************************************************************/
// void GetVarCallback (tsSCI_VAR *SciVarTable);

#endif //_MOTORCONTROLLER_H_