/**************************************************************************//**
 * \file Debug.h
 * \author Roman Holderried
 *
 * \brief Main function declarations.
 *
 * <b> History </b>
 * 	- 2022-01-05 - File creation
 *****************************************************************************/

#ifndef _DEBUG_H_
#define _DEBUG_H_
/******************************************************************************
 * Includes
 *****************************************************************************/
#include <stdint.h>
#include <stdbool.h>

/******************************************************************************
 * Defines
 *****************************************************************************/
/** Debug Output Levels */
#define DBG_OUTPUT_LVL_NONE     0
#define DBG_OUTPUT_LVL_LOW      1 
#define DBG_OUTPUT_LVL_MEDIUM   2
#define DBG_OUTPUT_LVL_HIGH     3

/** Maximum Size of a debug message */
#define DBG_OUTPUT_MAX_SIZE     255

/******************************************************************************
 * Type definitions
 *****************************************************************************/

/******************************************************************************
 * Function declarations
 *****************************************************************************/
void DebugOutput(uint8_t ui8Lvl, const char *format, ...);

#endif //_DEBUG_H_