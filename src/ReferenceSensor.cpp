/**************************************************************************//**
 * \file ReferenceSensor.cpp
 * \author Roman Holderried
 *
 * \brief Functions related to the reference sensor.
 * 
 * <b> History </b>
 * 	- 2022-12-05 - File creation
 *****************************************************************************/

/******************************************************************************
 * Includes
 *****************************************************************************/
#ifndef DEBUG_NATIVE
#include <Arduino.h>
#include <cstdint>
#include <cstdbool>
#include "HWConfig.h"
#include "ReferenceSensor.h"

/******************************************************************************
 * Defines
 *****************************************************************************/

/******************************************************************************
 * Private Globals
 *****************************************************************************/
static tsREFERENCE_SENSOR sRefSens = tsREFERENCE_SENSOR_DEFAULTS;

/******************************************************************************
 * Function definitions
 *****************************************************************************/
void InitRefSensor (tsREFERENCE_SENSOR *psRefSens)//, tsREFERENCE_SENSOR_CALLBACKS sCallbacks)
{
    // Assign the GPIO
    // psRefSens->sCallbacks = sCallbacks;

    pinMode(REF_SENSOR_PIN, INPUT);

    psRefSens->bLaststate = (bool)digitalRead(REF_SENSOR_PIN);
}

//=============================================================================
bool GetRefSensorState (tsREFERENCE_SENSOR *psRefSens)
{
    psRefSens->bLaststate = (bool)digitalRead(REF_SENSOR_PIN);

    return psRefSens->bLaststate;
}
#endif