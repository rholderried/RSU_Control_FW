/**************************************************************************//**
 * \file Callbacks.c
 * \author Roman Holderried
 *
 * \brief Callbacks of the RSU application.
 * 
 * <b> History </b>
 * 	- 2022-11-28 - File creation -
 *****************************************************************************/

/******************************************************************************
 * Includes
 *****************************************************************************/
#include <Arduino.h>
#include "Callbacks.h"

/******************************************************************************
 * Function definitions
 *****************************************************************************/
teTRANSFER_ACK ProcessGetVarTransfer (teREQUEST_ACKNOWLEDGE eAck, int16_t i16Num, uint32_t ui32Data, uint16_t ui16ErrNum)
{
    if (eAck == eREQUEST_ACK_STATUS_SUCCESS)
    {

    }
}