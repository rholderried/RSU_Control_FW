#include <Arduino.h>
#include <HardwareSerial.h>
#include "Callbacks.h"
#include "HWConfig.h"
#include <SCIMaster.h>


// SCI serial connection
HardwareSerial SCISerial(SCI_UART_NUM);

void setup() 
{
  #if TX_PACKET_LENGTH > 126
  SCISerial.setTxBufferSize(TX_PACKET_LENGTH + 2);
  #endif

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
  // put your main code here, to run repeatedly:
}