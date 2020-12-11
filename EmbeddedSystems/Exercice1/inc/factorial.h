#ifndef FACTORIAL_HEADER_GUARD
#define FACTORIAL_HEADER_GUARD


/** Prototypes functions used to performs factorial calculations **/

/******
 * 
 * function: performsFactorial
 * :param pipeStruct *pipeS: struct where will be save all results
 * :param maxNumber: number max to perform factorial calculations.
 *                   Calculations will start with the maxNumber and
 *                   finish with minNumber
 *                  
 * :return long double representing factorial calculated
 * 
 * Obs: pipeStruct *pipeS is always passed by reference and is 
 * therefore updated with every function call.
 * 
 ******/
long double performsFactorial(int maxNumber, int minNumber);



#endif