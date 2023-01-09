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
#ifndef DEBUG_NATIVE
#include <Arduino.h>
#include <HardwareSerial.h>
#include <cstdint>
#include "Callbacks.h"
#include <SCIMaster.h>
#include "SCIMasterConfig.h"
#include "MotorController.h"
#include "HWConfig.h"
#include "Interface.h"
#include "Debug.h"


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
void SCIReceiveByte(void)
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
        InterfaceReceiveString(ui8Data, SERIAL_INTERFACE_INDEX);
    }
}

//=============================================================================
void serialTransmit (char* cMsg, uint16_t ui16Len)
{
    Serial.write(cMsg, ui16Len);
    Serial.write('\n');
}

//=============================================================================
uint8_t SCISerialWriteNonBlocking(uint8_t *pui8Buf, uint8_t ui8Len)
{
    DebugOutput(DBG_OUTPUT_LVL_HIGH, "Send SCI message.");
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
    bool bBusy;
    #if TX_PACKET_LENGTH > 126
    bBusy = !((bool)(SCISerial.availableForWrite() >= TX_PACKET_LENGTH + 2));
    #else
    bBusy = !((bool)(SCISerial.availableForWrite() >= 128));
    #endif

    return bBusy;
}
#endif