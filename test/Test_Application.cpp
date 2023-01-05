/**************************************************************************//**
 * \file Test_Application.cpp
 * \author Roman Holderried
 *
 * \brief Test functions for the RSU Application.
 * 
 * <b> History </b>
 * 	- 2022-01-05 - File creation
 *****************************************************************************/

/******************************************************************************
 * Includes
 *****************************************************************************/
#include <cstdint>
#include <cstdbool>
#include <time.h>
#include <windows.h>
#include "Unittests.h"
#include "Timer32Bit.h"

/******************************************************************************
 * Defines
 *****************************************************************************/

/******************************************************************************
 * Imported Globals
 *****************************************************************************/
extern tTIMER_CTL_32BIT sTimer32Bit;

/******************************************************************************
 * Private Globals
 *****************************************************************************/

/******************************************************************************
 * Function definitions
 *****************************************************************************/
void Test_Module (void (*Fn)(void), uint32_t ui32LoopCnt)
{
    uint32_t ui32Loop = 0;
    clock_t start = clock();
    double dMsec = 0;

    setup();

    while (ui32Loop < ui32LoopCnt)
    {
        loop();
        Fn();

        ui32Loop++;

        dMsec = (double)(clock() - start) * 1000 / CLOCKS_PER_SEC;

        // Execute the timer
        if (dMsec > 1)
        {
            Timer32BitExecute();
            start = clock();
        }
        Sleep(1);
    }
}

void Test_Startup (void)
{

}