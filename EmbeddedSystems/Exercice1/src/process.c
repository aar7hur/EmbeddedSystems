#include "../inc/process.h"
#include "../inc/factorial.h"
#include "../inc/main.h"


/************
 * 
 * File used to describe all functions used to perform
 * communication between process.
 * 
 * 
 * ALL FUNCTIONS DOCUMENTATIONS CAN BE SEEN IN "../inc/process.h"
 * 
 * 
 ***********/


void writeToPipe(int* fileDesciptor, childProcessStruct *data)
{

    uint16_t maxNumber = data->startNumber;
    uint16_t minNumber = data->endNumber;
    long double result = performsFactorial(maxNumber, minNumber);
    write(fileDesciptor[WRITE_FD], &result, sizeof(result));
    wait(NULL);
}



void handleProcess(childProcessStruct *data, pipeStruct *result, int numberProcess)
{
    // If more than one process
    if (numberProcess > MIN_PROCESS)
    {
        // Creates variables to handle with creating proccess
        // use in file descriptor and aux variables
        pid_t process;
        int fileDescriptor[numberProcess][FILE_DESCRIPTOR_LEN];
        uint8_t counter = 0;

        while (counter < numberProcess)
        {
            if  (pipe(fileDescriptor[counter]))
            {
                perror("Pipe Error\n");
                _exit(EXIT_FAILURE);
            }
            process = fork();

            // If occurs some erros in creating fork
            if (process == ERROR)
            {
                printf("Error creating child process");
                _exit(EXIT_FAILURE);
            }

            // if the process is a child of the parent
            else if(process == CHILD_PROCESS)
            {
                // close read file descriptor because child w'ont use it
                close(fileDescriptor[counter][READ_FD]);

                // performs factorial and writes to file descriptor
                writeToPipe(fileDescriptor[counter], &data[counter]);
                // close write file descriptor because child w'ont use it
                close(fileDescriptor[counter][WRITE_FD]);
                exit(CHILD_FINISHED);
            }

            counter++;
        }

        // If the process is parent
        if (process > 0)
        {
            counter = 0;

            while (counter < numberProcess)
            {   
                long double parcialResult = 0;

                // close write file descriptor because child w'ont use it
                close(fileDescriptor[counter][WRITE_FD]);

                // waits until any child process finish
                wait(NULL);

                // read from file descriptor and saves into parcialResult variable
                read(fileDescriptor[counter][READ_FD], &parcialResult, sizeof(parcialResult));
                
                // final result is is multiplied and incremented at each loop
                result->result = parcialResult * result->result;

                // close read file descriptor because child w'ont use it
                close(fileDescriptor[counter][READ_FD]);

                counter++;
            }
        }
    }

    else
    {
        handleOneProcess(result);
    }
}

void handleOneProcess(pipeStruct *resultStruct)
{
    int maxNumber = resultStruct->factorialNumber;
    int minNumber = MIN_PROCESS;
    long double resultFact = performsFactorial(maxNumber, minNumber);
    resultStruct->result = resultFact;
}