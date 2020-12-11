#ifndef PROCESS_HEADER_GUARD
#define PROCESS_HEADER_GUARD



#define WRITE_FD 1
#define READ_FD 0
#define FILE_DESCRIPTOR_LEN 2
#define ERROR -1
#define CHILD_PROCESS 0 
#define CHILD_FINISHED 0


/****
 * 
 * Struct used to group important process variables
 * 
 ****/
typedef struct {
    int numberProcess;
    int factorialNumber;
    long double result;

} pipeStruct;

/*****
 * 
 * Struct used to emulate boolean variables
 * 
 ****/

typedef enum { 
    False,
    True 
} boolean;

/*****
 * 
 * Struct used to save child start and finish numbers to calculate
 * 
 ****/

typedef struct {
    int startNumber;
    int endNumber;
} childProcessStruct;

/** Prototypes functions used to performs communications between process**/

/******
 * 
 * function: writeToPipe
 * 
 * The main goal of this function is to calculate factorial and save the result
 * into a file descriptor.
 * :param int *fileDescriptor: array which contains file descriptor
 *                            to communicate between process
 * :param childProcessStruct: struct which contais all parameters for 
 *                            the child process
 *                  
 * :return void
 * 
 * 
 ******/
void writeToPipe(int* fileDesciptor, childProcessStruct *data);

void handleOneProcess(pipeStruct *resultStruct);

/******
 * 
 * function: handleOneProcess
 * :param childProcess *data: struct where will be save all results.
 * 
 * This function is responsible to handle with only one process.
 *                  
 * :return void
 * 
 * Obs: pipeStruct *pipeS is always passed by reference and is 
 * therefore updated with every function call.
 * 
 ******/

void handleProcess(childProcessStruct *data, pipeStruct *result, int numberProcess);




#endif