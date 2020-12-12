#include "../inc/main.h"



/************
 * 
 * File used to describe all functions used to perform
 * factorial calculations
 * 
 * 
 * ALL FUNCTIONS DOCUMENTATIONS CAN BE SEEN IN "../inc/factorial.h"
 * 
 * 
 ***********/


void *performsFactorial(void * structThread)
{   
    // Creating a copy of structure received in function and 
    // do a pointer to this struct
    threadingStruct *specificThread = (threadingStruct*)structThread;

    long double factorial = 1;
    int minNumber = specificThread->endNumber;
    int maxNumber = specificThread->startNumber;

    for (maxNumber; maxNumber >= minNumber; maxNumber--) 
    {   
        usleep(1000);
        factorial = factorial * maxNumber;
    }
    specificThread->result = factorial;
}