#ifndef MAIN_HEADER_GUARD
#define MAIN_HEADER_GUARD

#include <stdio.h>
#include <semaphore.h>
#include "pthread.h"
#include <sys/wait.h>
#include <stdlib.h>
#include "stdint.h"
#include <string.h>
#include <unistd.h>


#define ERROR -1
#define SHARE_THREAD 0
#define SEMAPHORE_START_VALUE 1
#define SUCCESS 0

/*

Enumerator which will define how many prints each thread
 will make in the terminal.

*/
typedef enum
{ 
    NUMBER_OF_PRINT_THREAD_1 =  20,
    NUMBER_OF_FINAL_PRINT_THREAD_1 = 15,
    NUMBER_OF_PRINT_THREAD_2  = 7,
    NUMBER_OF_PRINT_THREAD_3  = 4,
    NUMBER_OF_PRINT_THREAD_4  = 4,
    NUMBER_OF_PRINT_THREAD_5  = 0,
    NUMBER_OF_PRINT_THREAD_6  = 13,
    NUMBER_OF_THREADS = 6
} NumberPrintPerThread;

/*

Struct used to define the number of prints for each thread
and what letter the thread will print.
Example
typedef struct 
{
    "b",
    7
}
the letter b will be displayed on the terminal 7 times by the second thread

*/
typedef struct PrintManager
{
    const char * character;
    int numberOfPrints;
} PrintManager;

/*

Variables that represents all characters that each thread will print

*/
const char *characterThread1 = "a";
const char *characterThread2 = "b";
const char *characterThread3 = "c";
const char *characterThread4 = "d";
const char *characterThread5 = "e";
const char *characterThread6 = "f";

/*
If pshared has the value 0, then the semaphore is shared between
the threads of a process, and should be located at some address
that is visible to all threads (e.g., a global variable, or a
variable allocated dynamically on the heap).
*/
int pshared = SHARE_THREAD;
uint8_t threadCounter = 0;
pthread_t threadId[NUMBER_OF_THREADS];
sem_t semaphore;



void createThreads(PrintManager *printOrder);
void terminateThreads(void);
void *printThreads(void *printOrder);
void populatePrintManager(PrintManager *printOrder);
void header(void);


#endif