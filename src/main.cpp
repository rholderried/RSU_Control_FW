/**************************************************************************//**
 * \file main.cpp
 * \author Roman Holderried
 *
 * \brief Application Setup / Main loop.
 * 
 * <b> History </b>
 * 	- 2022-11-28 - File creation
 *****************************************************************************/

/******************************************************************************
 * Includes
 *****************************************************************************/
#ifndef DEBUG_NATIVE
#include <Arduino.h>
#include <HardwareSerial.h>
#endif
#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>
#include "Debug.h"
#include "RSU.h"
#include "Callbacks.h"
#include "HWConfig.h"
#include "ReferenceSensor.h"
#include "MotorController.h"
#include "Interface.h"
#include <SCIMaster.h>
#include <Timer32Bit.h>

/******************************************************************************
 * Defines
 *****************************************************************************/

/******************************************************************************
 * Exported globals
 *****************************************************************************/
// SCI serial connection
#ifndef DEBUG_NATIVE
HardwareSerial SCISerial(SCI_UART_NUM);
hw_timer_t *HwTimer = NULL;
#endif
tTIMER_CTL_32BIT sTimer32Bit = tTIMER_32BIT_CTL_DEFAULTS;
uint8_t ui8DbgLvl = DBG_OUTPUT_LVL_DEFAULT;



/******************************************************************************
 * Imported globals
 *****************************************************************************/
extern const char* cInterfaceCommands[];
extern const char* cInterfaceIDs[];

/******************************************************************************
 * Function definitions
 *****************************************************************************/

void DebugOutput(uint8_t ui8Lvl, const char *format, ...)
{
    va_list args;
    char cDbgMsg[DBG_OUTPUT_MAX_SIZE];
    uint16_t ui16StrLen;

    // Print debug message only if it fits the current defined debug level
    if (ui8Lvl <= ui8DbgLvl && ui8Lvl > DBG_OUTPUT_LVL_NONE)
    {
      va_start(args, format);
      ui16StrLen = vsnprintf(cDbgMsg, DBG_OUTPUT_MAX_SIZE - 1, format, args);
      Serial.write(cDbgMsg, ui16StrLen);
      va_end(args);
    }
}

//=============================================================================
void setup() 
{
  #ifndef DEBUG_NATIVE
  // The size of the TX buffer must be exactly known because the callback
  // which determines the TX busy state relies on this information.
  
  #if TX_PACKET_LENGTH > 126
  SCISerial.setTxBufferSize(TX_PACKET_LENGTH + 2);
  #endif

  Serial.begin(115200, SERIAL_8N1);

  DebugOutput(DBG_OUTPUT_LVL_HIGH, "Begin application setup...\n");

  /****************************************************************************
   * Initialize timer (Timer base frequency == 80 MHz, minimum divider == 2 -> Timer frequency == 40 MHz)
   ***************************************************************************/
  HwTimer = timerBegin(HW_TIMER_CFG_NUM, HW_TIMER_CFG_DIV, true);
  timerAttachInterrupt(HwTimer, Timer32BitExecute, false);
  // Set the timer period value for a timeout every 1 ms
  timerAlarmWrite(HwTimer, 80000000/(HW_TIMER_CFG_DIV * 1000), true);
  timerStart(HwTimer);
  #endif
  /***************************************************************************/

  /****************************************************************************
   * Initialize interfaces
   ***************************************************************************/
  InitInterfaces (RSUProcessCommands, cInterfaceCommands, 3, cInterfaceIDs, 4);

  #ifndef DEBUG_NATIVE
  // SCI serial interface
  SCISerial.begin(SCI_BAUD, SERIAL_8N1, SCI_PIN_RX, SCI_PIN_TX);
  // Serial interface to the RSU controller
  Serial.begin(SERIAL_BAUD, SERIAL_8N1);
  #endif

  InterfaceAddTransmitCallback(SERIAL_INTERFACE_INDEX, serialTransmit);

  // Initialize the SCI Master
  SCIMasterInit((tsSCI_MASTER_CALLBACKS){.SetVarExternalCB = ProcessSetVarTransfer,
                                         .GetVarExternalCB = ProcessGetVarTransfer,
                                         .CommandExternalCB = ProcessCommandTransfer,
                                         .UpstreamExternalCB = ProcessUpstreamTransfer,
                                         .BlockingTxExternalCB = NULL,
                                         .NonBlockingTxExternalCB = SCISerialWriteNonBlocking,
                                         .GetTxBusyStateExternalCB = SCISerialGetBusyState});
  /***************************************************************************/

  // Initialize the RSU
  RSUInit();

  DebugOutput(DBG_OUTPUT_LVL_HIGH, "Application setup complete.\n");
  DebugOutput(DBG_OUTPUT_LVL_HIGH, "Starting application loop...\n");
}

//=============================================================================
void loop() 
{
  RSUStateMachine();

  SCIMasterSM();
}