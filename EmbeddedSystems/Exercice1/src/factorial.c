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


long double performsFactorial(int maxNumber, int minNumber)
{   
    long double factorial = 1;
    for (maxNumber; maxNumber >= minNumber; maxNumber--) 
    {   
        usleep(1000);
        factorial = factorial * maxNumber;
    }
    return factorial;
}
