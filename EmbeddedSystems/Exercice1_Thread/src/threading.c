#include "../inc/main.h"
#include "../inc/factorial.h"
#include "../inc/threading.h"

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



void handleThreads(threadingStruct *data, int numberThreads, int factorialNumber)
{
    // If more than one process
    if (numberThreads > MIN_THREADS)
    {
        // Creates variables to handle with creating proccess
        // use in file descriptor and aux variables
        uint8_t counter = 0;
        pthread_t threadId[numberThreads];

        while (counter < numberThreads)
        {
            // If occurs some erros in creating thread
            if (pthread_create(&(threadId[counter]), NULL, performsFactorial, &data[counter]) == ERROR)
            {
                printf("Error creating thread");
                _exit(EXIT_FAILURE);
            }
            counter++;
        }

        counter = 0;
        while (counter < numberThreads)
        {
            pthread_join(threadId[counter], NULL);
            counter++;
        }
    }

    else
    {
        performsFactorial(data);
    }

}
