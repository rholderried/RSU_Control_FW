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
#include <Arduino.h>
#include <HardwareSerial.h>
// #include <
#include "RSU.h"
#include "Callbacks.h"
#include "HWConfig.h"
#include "ReferenceSensor.h"
#include "MotorController.h"
#include <SCIMaster.h>
#include <Timer32Bit.h>

/******************************************************************************
 * Defines
 *****************************************************************************/

/******************************************************************************
 * Exported globals
 *****************************************************************************/
// SCI serial connection
HardwareSerial SCISerial(SCI_UART_NUM);
hw_timer_t *HwTimer = NULL;
tTIMER_CTL_32BIT sTimer32Bit = tTIMER_32BIT_CTL_DEFAULTS;

/******************************************************************************
 * Function definitions
 *****************************************************************************/
void setup() 
{
  // The size of the TX buffer must be exactly known because the callback
  // which determines the TX busy state relies on this information.
  #if TX_PACKET_LENGTH > 126
  SCISerial.setTxBufferSize(TX_PACKET_LENGTH + 2);
  #endif

  /****************************************************************************
   * Initialize timer (Timer base frequency == 80 MHz, minimum divider == 2 -> Timer frequency == 40 MHz)
   ***************************************************************************/
  HwTimer = timerBegin(HW_TIMER_CFG_NUM, HW_TIMER_CFG_DIV, true);
  timerAttachInterrupt(HwTimer, Timer32BitExecute, false);
  // Set the timer period value for a timeout every 1 ms
  timerAlarmWrite(HwTimer, 80000000/(HW_TIMER_CFG_DIV * 1000), true);
  timerStart(HwTimer);
  /***************************************************************************/

  /****************************************************************************
   * Initialize the reference sensor
   ***************************************************************************/
  // InitRefSensor()
  /***************************************************************************/

  SCISerial.begin(SCI_BAUD, SERIAL_8N1, SCI_PIN_RX, SCI_PIN_TX);

  // Initialize the SCI Master
  SCIMasterInit((tsSCI_MASTER_CALLBACKS){.SetVarExternalCB = ProcessSetVarTransfer,
                                         .GetVarExternalCB = ProcessGetVarTransfer,
                                         .CommandExternalCB = ProcessCommandTransfer,
                                         .UpstreamExternalCB = ProcessUpstreamTransfer,
                                         .BlockingTxExternalCB = NULL,
                                         .NonBlockingTxExternalCB = SCISerialWriteNonBlocking,
                                         .GetTxBusyStateExternalCB = SCISerialGetBusyState});
}

void loop() {
  
  RSUStateMachine();

  SCIMasterSM();
  // put your main code here, to run repeatedly:
}