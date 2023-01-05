/**************************************************************************//**
 * \file TestCallbacks.cpp
 * \author Roman Holderried
 *
 * \brief Simulation of the hardware interface for test purposes.
 * 
 * <b> History </b>
 * 	- 2022-01-05 - File creation
 *****************************************************************************/

/******************************************************************************
 * Includes
 *****************************************************************************/
#include <cstdint>
#include <cstdbool>
#include <stdio.h>
#include "ReferenceSensor.h"
#include "Callbacks.h"
/******************************************************************************
 * Defines
 *****************************************************************************/

/******************************************************************************
 * Imported Globals
 *****************************************************************************/

/******************************************************************************
 * Private Globals
 *****************************************************************************/

/******************************************************************************
 * Function definitions
 *****************************************************************************/
uint8_t SCISerialWriteNonBlocking(uint8_t *pui8Buf, uint8_t ui8Len)
{
    printf("%.*s", ui8Len, (char*)pui8Buf);

    if (*pui8Buf == 0x03)
        printf("\n");
        
    return ui8Len;
}

bool SCISerialGetBusyState(void)
{
    return false;
}

void serialTransmit (char* cMsg, uint16_t ui16Len)
{
    printf("%.*s", ui16Len, cMsg);
}

void InitRefSensor (tsREFERENCE_SENSOR *psRefSens)
{

}

bool GetRefSensorState (tsREFERENCE_SENSOR *psRefSens)
{
    return true;
}