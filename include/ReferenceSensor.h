/**************************************************************************//**
 * \file ReferenceSensor.h
 * \author Roman Holderried
 *
 * \brief Reference sensor type definitions, fcn declarations, etc.
 *
 * <b> History </b>
 * 	- 2022-12-05 - File creation
 *****************************************************************************/

#ifndef _REFERENCESENSOR_H_
#define _REFERENCESENSOR_H_
/******************************************************************************
 * Includes
 *****************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/******************************************************************************
 * Defines
 *****************************************************************************/
#define REFERENCE_SENSOR_STATE_ON   true
#define REFERENCE_SENSOR_STATE_OFF  false
#define REFERENCE_SENSOR_EDGE_POS   true
#define REFERENCE_SENSOR_EDGE_NEG   false

/******************************************************************************
 * Type definitions
 *****************************************************************************/
typedef struct
{
    void (*OnEdgeChangeCB)(bool bEdge);
}tsREFERENCE_SENSOR_CALLBACKS;

#define tsREFERENCE_SENSOR_CALLBACKS_DEFAULTS {NULL}

typedef struct
{
    bool bLaststate;
    tsREFERENCE_SENSOR_CALLBACKS sCallbacks;
}tsREFERENCE_SENSOR;

#define tsREFERENCE_SENSOR_DEFAULTS {REFERENCE_SENSOR_STATE_OFF, tsREFERENCE_SENSOR_CALLBACKS_DEFAULTS}


/******************************************************************************
 * Function declarations
 *****************************************************************************/
/********************************************************************************//**
 * \brief Initializes the reference sensor.
 ***********************************************************************************/
void InitRefSensor (tsREFERENCE_SENSOR_CALLBACKS sCallbacks);

/********************************************************************************//**
 * \brief Get the current reference sensor state.
 * 
 * @returns Current reference sensor state.
 ***********************************************************************************/
bool GetRefSensorState (void);

/********************************************************************************//**
 * \brief Reference sensor interrupt service routine.
 ***********************************************************************************/
void RefSensorOnChangeISR (void);

#endif //_REFERENCESENSOR_H_