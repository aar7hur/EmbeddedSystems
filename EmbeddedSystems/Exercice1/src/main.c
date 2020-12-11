#include "../inc/main.h"


/** ALL FUNCTION DOCUMENTATIONS CAN BE SEEN IN THE "../inc/main.h" FILE **/

/****
 * 
 * Use this script to test how pip works.
 * The main goals is to communicate between proccess.
 * The communication works by passing an writing an scruct 
 * into file file descriptor on child process and reading it
 *
 ****/

// variables to get arguments from command line
static uint8_t argvNumberProcess;
static uint16_t argvFat;

int main(int argc, char *argv[])
{

    // Checks if user entered with valid inputs
    if (validUserInput(argc, argv) == False)
    {
        exit(EXIT_FAILURE);
    }
   
    // Creates file descriptor to communicate between
    // process using PIPE
    int fileDescriptor[2], nbytes;
    
    // Allocate memory to create an array to separate
    // the numbers that the processes will calculate
    childProcessStruct *childs = organizeStructForEveryProcess(argvNumberProcess, argvFat);

    // Allocate memory to direct access struct
    pipeStruct *data = allocateMemory(sizeof(*data));
    data->numberProcess = argvNumberProcess;
    data->factorialNumber = argvFat;
    data->result = MIN_PROCESS;

    // Handles with all process
    handleProcess(childs, data, argvNumberProcess);

    printf("\nFactorial Result is: %Lf\n", data->result);
    free(data);
    free(childs);
}

boolean validUserInput(int argc, char *argv[])
{

    // if number of CLI inputs is different from 3
    if (argc != 3)
    {
        fprintf(stderr, "Arguments are missing to run the application\n\n");
        return False;
    }

    argvNumberProcess = (uint8_t)atoi(argv[2]);
    argvFat = atoi(argv[1]);

    // All CLI arguments must be grater than 0
    if ((argvFat <= 0) || (argvNumberProcess <= 0) || (argvNumberProcess > MAX_NUMBER_PROCESS))
    {
        fprintf(stderr, "Arguments must be greater than 0\n\n");
        return False;
    }
    else
    {
        return True;
    }
    
}

void *allocateMemory(size_t size)
{   
    // allocates memory dynamically
    void *p = malloc(size);

    // if some error occurs in allocating memory
    if(!p)
    {
        perror("Cannot allocate memory\n");
        exit(EXIT_FAILURE);
    }
    return p;
}


childProcessStruct *organizeStructForEveryProcess(int numberProcess, int factorialNumber)
{
    int initEndNumber = 0, rest = 0, division = 0;

    // Allocate memory for all child structs
    childProcessStruct * childs = (childProcessStruct *)malloc(numberProcess * sizeof(childProcessStruct));


    // If the rest of the division is 1 a process will calculate one more number 
    if (factorialNumber % numberProcess)
    {
        rest = 1;
    }

    division = (int)factorialNumber/numberProcess;

    // If division is equal to the number to calculate factorial,
    // only one process will be used, so return without continue.
    if (division == factorialNumber)
    {
        return childs;
    }

    for (uint8_t counter = 0; counter < numberProcess; counter++)
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
        
        initEndNumber = ((numberProcess - (counter+1))*division) +1;
        if (counter == 0)
        {
            initEndNumber = initEndNumber - rest;
        }
        
        childs[counter].startNumber = factorialNumber;
        childs[counter].endNumber = initEndNumber;
        factorialNumber = initEndNumber - 1;
    }

    return childs;
}