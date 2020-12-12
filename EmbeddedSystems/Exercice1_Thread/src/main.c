#include "../inc/main.h"



/** ALL FUNCTION DOCUMENTATIONS CAN BE SEEN IN THE "../inc/main.h" FILE **/

/****
 * 
 * Use this script to test how thread works.
 * The main goals is to communicate between threads.
 * The communication works by passing an scruct 
 * to every thread
 *
 ****/

// variables to get arguments from command line
static uint8_t argvNumberThreads;
static uint16_t argvFat;


int main(int argc, char *argv[])
{
    uint8_t threadCounter = 0;
    long double result = 1;

    // Checks if user entered with valid inputs
    if (validUserInput(argc, argv) == False)
    {
        exit(EXIT_FAILURE);
    }

    threadingStruct *thread = organizeStructForEveryThread(argvNumberThreads, argvFat);
    handleThreads(thread, argvNumberThreads, argvFat);

    while(threadCounter < argvNumberThreads)
    {
        result = result * thread[threadCounter].result;
        threadCounter++;
    }
    printf("\nResultado do fatorial = %LF\n", result);
	return 0;
}


boolean validUserInput(int argc, char *argv[])
{

    // if number of CLI inputs is different from 3
    if (argc != 3)
    {
        fprintf(stderr, "Arguments are missing to run the application\n\n");
        return False;
    }

    argvNumberThreads = (uint8_t)atoi(argv[2]);
    argvFat = atoi(argv[1]);

    // All CLI arguments must be grater than 0
    if ((argvFat <= 0) || (argvNumberThreads <= 0) || (argvNumberThreads > MAX_NUMBER_THREAD))
    {
        fprintf(stderr, "Arguments must be greater than 0\n\n");
        return False;
    }
    else
    {
        return True;
    }
}


threadingStruct *organizeStructForEveryThread(int numberThreads, int factorialNumber)
{
    int initEndNumber = 0, rest = 0, division = 0;

    // Allocate memory for all child structs
    threadingStruct * thread = (threadingStruct *)malloc(numberThreads * sizeof(threadingStruct));

    // If the rest of the division is 1 a process will calculate one more number 
    if (factorialNumber % numberThreads)
    {
        rest = 1;
    }

    division = (int)factorialNumber/numberThreads;
    /*
    if (division == factorialNumber)
    {
        printf("retornando thread")
        return thread;
    }
    */
    for (uint8_t counter = 0; counter < numberThreads; counter++)
    {
        // init number is calculated using the number of processes passed by the user, 
        // the division between the number to be calculated, the factorial and the 
        // number of processes.
        // Counter -->  is the variable used to make the looping interval. It is added to one 
        // in the formula because it starts at 0.
        // Division --> is the result of dividing the number to be calculated and the number 
        // of processes. It is used to know how many multiplications each process will do.
        // +1 -->  in the formula is used because the starting number is always 1 more than the
        // previous ending number. Example: 
        //                        start number 120; end number 81; start number 80; end number 41
        // Rest is the remainder of the division of the number to be calculated with the number of 
        // processes. If the rest is 1, one of the processes will multiply 1 more number
        //      
        
        initEndNumber = ((numberThreads - (counter+1))*division) +1;
        if (counter == 0)
        {
            initEndNumber = initEndNumber - rest;
        }
        
        thread[counter].startNumber = factorialNumber;
        thread[counter].endNumber = initEndNumber;
        factorialNumber = initEndNumber - 1;
    }
    return thread;
}