#ifndef MAIN_HEADER_GUARD
#define MAIN_HEADER_GUARD


#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <pthread.h>
#include "threading.h"


#define MAX_NUMBER_THREAD 80
#define MIN_THREADS 1
#define ERROR -1


/*****
 * 
 * Struct used to emulate boolean variables
 * 
 ****/

typedef enum { 
    False,
    True, 
} boolean;


/***
 * 
 * Function prototypes
 * 
 ***/

/******
 * 
 * function: *allocarteMemory
 * :param size: size of memory to be allocated
 * :return pointer that can be casted to any type
 * 
 ******/
void* allocateMemory(size_t size);


/******
 * 
 * function: validUserInput
 * :param argc: number of arguments passed  command line
 * :param char *argv[]: pointer to list which contains all
 *                      parameters passed in CLI
 * :return boolean variable representing if arguments was passed 
 *         correctly or not.
 * 
 ******/
boolean validUserInput(int argc, char *argv[]);


/******
 * 
 * function: organizeArrayForEveryThread
 * :param int numberProcess: number of thread to be used in the 
 *                          calculation
 * :param int factorialNumber: number used to calculate the factorial
 * :return pointer representing array of structs.
 *      The size of this array depending depends on how many threads 
 *      are going to be used.
 * 
 ******/
threadingStruct *organizeStructForEveryThread(int numberThreads, int factorialNumber);
#endif