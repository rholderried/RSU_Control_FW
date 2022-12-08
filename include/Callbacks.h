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
 * Function declarations
 *****************************************************************************/

uint8_t SCISerialWriteNonBlocking(uint8_t *pui8Buf, uint8_t ui8Len);
bool SCISerialGetBusyState(void);
void serialTransmit (char* cMsg, uint16_t ui16Len);

#endif //_CALLBACKS_H_