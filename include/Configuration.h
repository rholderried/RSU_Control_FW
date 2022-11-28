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








/******************************************************************************
 * Type definitions
 *****************************************************************************/

/******************************************************************************
 * Function declarations
 *****************************************************************************/

#endif //_CONFIGURATION_H_