#ifndef MAIN_HEADER_GUARD
#define MAIN_HEADER_GUARD

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include "process.h"

#define MAX_NUMBER_PROCESS 50
#define MIN_PROCESS 1
#define WRITE_FD 1
#define READ_FD 0
#define FILE_DESCRIPTOR_LEN 2
#define ERROR -1
#define CHILD_PROCESS 0 


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
 * function: organizeArrayForEveryProcess
 * :param int numberProcess: number of processes to be used in the 
 *                          calculation
 * :param int factorialNumber: number used to calculate the factorial
 * :return pointer representing array of structs.
 *      The size of this array depending depends on how many processes 
 *      are going to be used.
 * 
 ******/
childProcessStruct *organizeStructForEveryProcess(int numberProcess, int factorialNumber);

#endif 