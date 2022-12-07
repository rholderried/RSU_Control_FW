/**************************************************************************//**
 * \file Callbacks.cpp
 * \author Roman Holderried
 *
 * \brief Callbacks of the RSU application.
 * 
 * <b> History </b>
 * 	- 2022-11-28 - File creation -
 *****************************************************************************/

/******************************************************************************
 * Includes
 *****************************************************************************/
#include <Arduino.h>
#include <HardwareSerial.h>
#include <cstdint>
#include "Callbacks.h"
#include <SCIMaster.h>
#include "SCIMasterConfig.h"
#include "MotorController.h"
#include "HWConfig.h"
#include "Interface.h"


/******************************************************************************
 * Defines
 *****************************************************************************/
#define SCI_SERIAL_EVENT_CALLBACK serialEvent ## SCI_UART_NUM 

/******************************************************************************
 * Imported globals
 *****************************************************************************/
extern HardwareSerial SCISerial;
extern tsSCI_VAR sSciVarTable[];
extern tsMOTCONTROLVARS sMotControlVars;

/******************************************************************************
 * Function definitions
 *****************************************************************************/


//=============================================================================
void SCI_SERIAL_EVENT_CALLBACK(void)
{
    uint8_t ui8Data[RX_PACKET_LENGTH];
    
    SCIReceive(ui8Data, SCISerial.readBytes(ui8Data, SCISerial.available()));
}

//=============================================================================
void serialEvent (void)
{
    uint8_t ui8Data;

    while (Serial.available())
    {
        Serial.readBytes(&ui8Data, 1);
        InterfaceReceiveString(ui8Data);
    }
}

//=============================================================================
uint8_t SCISerialWriteNonBlocking(uint8_t *pui8Buf, uint8_t ui8Len)
{
    if(SCISerial.availableForWrite() >= ui8Len)
    {
        SCISerial.write(pui8Buf, ui8Len);
        return ui8Len;
    }
    else
        return 0;
}

//=============================================================================
bool SCISerialGetBusyState(void)
{
    bool bBufferSent;

    #if TX_PACKET_LENGTH > 126
    bBufferSent = (bool)(SCISerial.availableForWrite() >= TX_PACKET_LENGTH + 2);
    #else
    bBufferSent = (bool)(SCISerial.availableForWrite() >= 128)
    #endif

    return bBufferSent;
}