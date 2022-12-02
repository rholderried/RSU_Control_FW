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






/******************************************************************************
 * Type definitions
 *****************************************************************************/

/******************************************************************************
 * Function declarations
 *****************************************************************************/

#endif //_HWCONFIG_H_