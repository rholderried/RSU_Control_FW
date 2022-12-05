/**************************************************************************//**
 * \file MotorController.h
 * \author Roman Holderried
 *
 * \brief Motorcontroller related functionality (SCI Variables and -Commands).
 *
 * <b> History </b>
 * 	- 2022-12-01 - File creation
 *****************************************************************************/

#ifndef _MOTORCONTROLLER_H_
#define _MOTORCONTROLLER_H_
/******************************************************************************
 * Includes
 *****************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/******************************************************************************
 * SCI Variables
 *****************************************************************************/

#define NUMBER_OF_MOTORCONTROLLER_VARS 2

// Motorcontroller external variables
#define MOTCTRL_VAR_NUM_MOTION_CONTROLLER_SCALE     59
#define MOTCTRL_VAR_NUM_ACTUAL_POSITION_INCREMENTS  12

// Motorcontroller external functions
#define MOTCTRL_CMD_NUM_MOTIONCONTROLLER_START          15
#define MOTCTRL_CMD_NUM_MOTIONCONTROLLER_FINISH_MVMNT   16

/******************************************************************************
 * Type definitions
 *****************************************************************************/
typedef enum
{
    eTRANSFER_STATE_IDLE,
    eTRANSFER_STATE_ISSUED,
    eTRANSFER_STATE_BUSY,
    eTRANSFER_STATE_READY
}teTRANSFER_STATE;

/** \brief Data type enumerator.*/
typedef enum
{
    eDTYPE_UINT8  = 0,
    eDTYPE_INT8   = 1,
    eDTYPE_UINT16 = 2,
    eDTYPE_INT16  = 3,
    eDTYPE_UINT32 = 4,
    eDTYPE_INT32  = 5,
    eDTYPE_F32    = 6
}teDTYPE;

typedef struct 
{
    int16_t     i16Num;
    void        *pVar;
    teDTYPE     eDtype;
}tsSCI_VAR;

#define tsSCI_VAR_DEFAULTS {-1, NULL, eDTYPE_UINT8}

typedef struct
{
    float   fMotionControllerScale;
    int32_t i32ActualPositionIncrements;
}tsMOTCONTROLVARS;

#define tsMOTCONTROLVARS_DEFAULTS {0};

typedef struct
{
    struct
    {
        teTRANSFER_STATE eTransferState;
        void (*TransferRdyCb)(void);
    }sTransfer;
}tsMOTORCONTROLLER;

#define tsMOTORCONTROLLER_DEFAULTS {{eTRANSFER_STATE_IDLE, NULL}}

/******************************************************************************
 * Function declarations
 *****************************************************************************/
bool MotCtrlGetVar (int16_t i16Num, void (*TransferRdyCb)(void));
bool MotCtrlStartMovement(float fTargetAcceleration, float fTargetVelocity, float fTargetPosition, bool bReferenceMovement, void (*TransferRdyCb)(void));
bool MotCtrlStopMovement(void (*TransferRdyCb)(void));


teTRANSFER_ACK ProcessSetVarTransfer (teREQUEST_ACKNOWLEDGE eAck, int16_t i16Num, uint16_t ui16ErrNum);
teTRANSFER_ACK ProcessGetVarTransfer (teREQUEST_ACKNOWLEDGE eAck, int16_t i16Num, uint32_t ui32Data, uint16_t ui16ErrNum);
teTRANSFER_ACK ProcessCommandTransfer(teREQUEST_ACKNOWLEDGE eAck, int16_t i16Num, uint32_t *pui32Data, uint8_t ui8DataCnt, uint16_t ui16ErrNum);
teTRANSFER_ACK ProcessUpstreamTransfer(int16_t i16Num, uint8_t *pui8Data, uint32_t ui32DataCnt);

// void GetVarCallback (tsSCI_VAR *SciVarTable);

#endif //_MOTORCONTROLLER_H_