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
#include <cstdint>
#include <cstdbool>
#include "HWConfig.h"
#include "Arduino.h"
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
void InitRefSensor (tsREFERENCE_SENSOR_CALLBACKS sCallbacks)
{
    // Assign the GPIO
    sRefSens.sCallbacks = sCallbacks;

    pinMode(REF_SENSOR_PIN, INPUT_PULLUP);

    sRefSens.bLaststate = (bool)digitalRead(REF_SENSOR_PIN);
}

//=============================================================================
bool GetRefSensorState (void)
{
    sRefSens.bLaststate = (bool)digitalRead(REF_SENSOR_PIN);

    return sRefSens.bLaststate;
}

//=============================================================================
void RefSensorOnChangeISR (void)
{
    bool bNewState = (bool)digitalRead(REF_SENSOR_PIN);
    

    if (sRefSens.bLaststate != bNewState)
    {
        if (bNewState == REFERENCE_SENSOR_STATE_ON)
            sRefSens.sCallbacks.OnEdgeChangeCB(REFERENCE_SENSOR_EDGE_POS);
        else
            sRefSens.sCallbacks.OnEdgeChangeCB(REFERENCE_SENSOR_EDGE_NEG);
    }

    sRefSens.bLaststate = bNewState;
}