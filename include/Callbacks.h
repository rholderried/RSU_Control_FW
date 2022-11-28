/**************************************************************************//**
 * \file Callbacks.h
 * \author Roman Holderried
 *
 * \brief Callback declarations for the RSU firmware.
 *
 * <b> History </b>
 * 	- 2022-11-17 - File copied from SCI
 *****************************************************************************/

#ifndef _CALLBACKS_H_
#define _CALLBACKS_H_
/******************************************************************************
 * Includes
 *****************************************************************************/
#include <stdint.h>
#include <stdbool.h>

#include <SCIMaster.h>
/******************************************************************************
 * defines
 *****************************************************************************/

/******************************************************************************
 * Type definitions
 *****************************************************************************/

/******************************************************************************
 * Function declarations
 *****************************************************************************/

teTRANSFER_ACK ProcessSetVarTransfer (teREQUEST_ACKNOWLEDGE eAck, int16_t i16Num, uint16_t ui16ErrNum);
teTRANSFER_ACK ProcessGetVarTransfer (teREQUEST_ACKNOWLEDGE eAck, int16_t i16Num, uint32_t ui32Data, uint16_t ui16ErrNum);
teTRANSFER_ACK ProcessCommandTransfer(teREQUEST_ACKNOWLEDGE eAck, int16_t i16Num, uint32_t *pui32Data, uint8_t ui8DataCnt, uint16_t ui16ErrNum);
teTRANSFER_ACK ProcessUpstreamTransfer(int16_t i16Num, uint8_t *pui8Data, uint32_t ui32DataCnt);


#endif //_CALLBACKS_H_