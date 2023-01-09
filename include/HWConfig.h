/**************************************************************************//**
 * \file HWConfig.h
 * \author Roman Holderried
 *
 * \brief Hardware configuration.
 *
 * <b> History </b>
 * 	- 2022-11-28 - File creation
 *****************************************************************************/

#ifndef _HWCONFIG_H_
#define _HWCONFIG_H_
/******************************************************************************
 * Includes
 *****************************************************************************/
#include <stdint.h>
#include <stdbool.h>

/******************************************************************************
 * Defines
 *****************************************************************************/
// SCI configuration
/** SCI UART baud rate [bit/s]*/
#define SCI_BAUD    115200

/** SCI receive pin number */
#define SCI_PIN_RX  14

/** SCI transmit pin number */
#define SCI_PIN_TX  27

/** SCI UART peripheral number */
#define SCI_UART_NUM  1

// 32Bit ms Timer config
/** Number (0-3) of the hardware timer */
#define HW_TIMER_CFG_NUM    0

/** \brief Divider of the hardware timer 
 * 
 * The base frequency of the ESP32 timer module is 80 MHz, which
 * can be prescaled by HW_TIMER_CFG_DIV from 2 to 65535.
*/
#define HW_TIMER_CFG_DIV    2

// Reference Sensor Hardware configuration
/** Reference Sensor GPIO number */
#define REF_SENSOR_PIN      13

// Serial interface config
/** Serial interface baud rate */
#define SERIAL_BAUD 115200

/** Serial interface index */
#define SERIAL_INTERFACE_INDEX 0

// Slot config
#define SLOT_1_ID_PIN 36
#define SLOT_2_ID_PIN 39
#define SLOT_3_ID_PIN 34
#define SLOT_4_ID_PIN 35
#define SLOT_5_ID_PIN 32
#define SLOT_6_ID_PIN 33
#define SLOT_7_ID_PIN 25
#define SLOT_8_ID_PIN 26

/** Gear Ratio of the RSU (1:x)*/
#define RSU_GEAR_RATIO 6

/** Motor gear ratio (1:x) */
#define MOTOR_GEAR_RATIO 33

/** Motor number of pole pairs */
#define MOTOR_NUMBER_OF_POLE_PAIRS 2





/******************************************************************************
 * Type definitions
 *****************************************************************************/

/******************************************************************************
 * Function declarations
 *****************************************************************************/

#endif //_HWCONFIG_H_