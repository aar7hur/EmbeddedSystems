#ifndef THREADING_HEADER_GUARD
#define THREADING_HEADER_GUARD



/*****
 * 
 * Struct used to save start and finish numbers to calculate
 * 
 ****/
typedef struct {
    int startNumber;
    int endNumber;
    long double result;
} threadingStruct;


void handleThreads(threadingStruct *data, int numberThreads, int factorialNumber);

#endif
