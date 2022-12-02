/**************************************************************************//**
 * \file MotorController.cpp
 * \author Roman Holderried
 *
 * \brief Motorcontroller related functionality (SCI Variables and -Commands).
 * 
 * <b> History </b>
 * 	- 2022-11-28 - File creation -
 *****************************************************************************/

/******************************************************************************
 * Includes
 *****************************************************************************/
#include <cstdint>
#include <SCIMaster.h>
#include "MotorController.h"

/******************************************************************************
 * Defines
 *****************************************************************************/

/******************************************************************************
 * Exported globals
 *****************************************************************************/

tsMOTCONTROLVARS sMotControlVars = tsMOTCONTROLVARS_DEFAULTS;

tsSCI_VAR sSciVarTable[NUMBER_OF_MOTORCONTROLLER_VARS] =
{   
    {  
        .i16Num = SCI_VAR_NUM_MOTION_CONTROLLER_SCALE,
        .pVar   = &sMotControlVars.fMotionControllerScale,
        .eDtype = eDTYPE_F32
    },
    {  
        .i16Num = SCI_VAR_NUM_ACTUAL_POSITION_INCREMENTS,
        .pVar   = &sMotControlVars.i32ActualPositionIncrements,
        .eDtype = eDTYPE_INT32
    }
};


/******************************************************************************
 * Function definitions
 *****************************************************************************/

