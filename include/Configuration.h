/**************************************************************************//**
 * \file Configuration.h
 * \author Roman Holderried
 *
 * \brief Configuration for the RSU control unit.
 *
 * <b> History </b>
 * 	- 2022-11-28 - File creation
 *****************************************************************************/

#ifndef _CONFIGURATION_H_
#define _CONFIGURATION_H_
/******************************************************************************
 * Includes
 *****************************************************************************/
#include <stdint.h>
#include <stdbool.h>

#include "Common.h"

/******************************************************************************
 * Revolver parameter
 *****************************************************************************/
/** Number of Sample cylinder slots */
#define NUMBER_OF_SLOTS 6

// Reference position search parameters
/** Angular velocity of the revolver on reference position search [rad/s] */
#define REFERENCE_POSITION_SEARCH_ANGULAR_VELOCITY      3.14f

/** Angular acceleration of the revolver on reference position search [rad/s²] */
#define REFERENCE_POSITION_SEARCH_ANGULAR_ACCELERATION  3.14f

// Movement velocity
/** Angular velocity of the revolver movement in nominal operation [rad/s] */
#define MOVEMENT_ANGULAR_VELOCITY   6.28f

/** Angular acceleration of the revolver movement in nominal operation [rad/s] */
#define MOVEMENT_ANGULAR_ACCELERATION 3.14f

/** Index of the motion controller implemented on the motor controller */
// #define MOTION_CONTROLLER_INSTANCE 0

/** Default offset of slot 1 relative to the reference position */
#define     SLOT1_DEFAULT_OFFSET_FROM_REFERENCE 0.0f

/** Maximum number of parameters that can be submitted by the interface */
#define INTERFACE_MAX_NUMBER_OF_PARAMETERS 5








/******************************************************************************
 * Type definitions
 *****************************************************************************/

/******************************************************************************
 * Function declarations
 *****************************************************************************/

#endif //_CONFIGURATION_H_