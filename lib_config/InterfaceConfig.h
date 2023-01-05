/**************************************************************************//**
 * \file InterfaceConfig.h
 * \author Roman Holderried
 *
 * \brief Interface configuration template.
 *
 * <b> History </b>
 * 	- 2022-01-04 - File creation
 *****************************************************************************/

#ifndef _INTERFACECONFIG_H_
#define _INTERFACECONFIG_H_
/******************************************************************************
 * Includes
 *****************************************************************************/


/******************************************************************************
 * Defines
 *****************************************************************************/
/** Maximum number of chars that a single parameter object can be represented by */
#define MAXIMUM_RECEIVE_OBJECT_BYTE_LENGTH  20

/** Maximum string length that is returned by the tPROCESS_COMMAND_CB callback */
#define MAXIMUM_RETURN_STRING_BYTE_LENGTH   128

/** Maximum number of parameters that can be submitted by the interface */
#define INTERFACE_MAX_NUMBER_OF_PARAMETERS 5

/** Maximum number of Commands */
#define INTERFACE_MAX_NUMBER_OF_COMMANDS 10

/** Number of Interfaces */
#define NUMBER_OF_INTERFACES 2

#endif //_INTERFACECONFIG_H_