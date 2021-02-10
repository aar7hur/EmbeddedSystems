
/** 
 * 
 * ALL FUNCTION DOCUMENTATIONS CAN BE SEEN IN THE "../inc/main.h" FILE 
 * 
 * 
 * TE355 - Sistemas operacionais embarcados
 * Author: Arthur Munhoz Amaral
 * GRR20177243
 * 
 * 
 * **/
#include "../inc/main.h"

PrintManager *printOrder;

int main(int argc, char *argv[])
{
   header();
   // allocate memory for 6 structs. 
   // 6 is because the script will have 6 threads running in parallel
   printOrder = (PrintManager *)malloc(NUMBER_OF_THREADS * sizeof(PrintManager));
   if (!printOrder)
   {
      perror("Cannot allocate memory");
   }
   // Populate every struct position with the letter and number of times that
   // this letter will be shown in the terminal
   populatePrintManager(printOrder);

   if (sem_init(&semaphore, pshared, SEMAPHORE_START_VALUE) < 0){
         perror("Failed to create semaphore\n");
         return 1;
      }

   // creates all threads
   createThreads(printOrder);

   // finish all threads
   terminateThreads();

   // destrot semnaphore
   sem_destroy(&semaphore);

   return EXIT_SUCCESS;
}

void header(void)
{
   printf("TE355 - Trabalho 2 - Semaphore\n");
   printf("Aluno: Arthur Munhoz Amaral - GRR20177243\n");
   
}

void populatePrintManager(PrintManager *printOrder)
{
   // initiate the array of structs with each thread and each
   // character to print in the terminal
   PrintManager print[NUMBER_OF_THREADS] = {
      {characterThread3, NUMBER_OF_PRINT_THREAD_3},
      {characterThread2, NUMBER_OF_PRINT_THREAD_2},
      {characterThread1, NUMBER_OF_PRINT_THREAD_1},
      {characterThread4, NUMBER_OF_PRINT_THREAD_4},
      {characterThread6, NUMBER_OF_PRINT_THREAD_6},
      {characterThread1, NUMBER_OF_FINAL_PRINT_THREAD_1}
   };

   // Copy all content already declared from print to printOrder.
   // This is done only because i want to work with memory allocated
   // dinamically
   memcpy(printOrder, print, NUMBER_OF_THREADS * sizeof(*print));
}

/**
 * Function that creates all threads based on #define NUMBER_OF_THREADS
 * 
 * To see more about documentation please go to inc/main.h
 * 
 **/
void createThreads(PrintManager *printOrder)
{
   uint8_t threadCounter = 0;
   while (threadCounter < NUMBER_OF_THREADS)
   {  
      int *threadNumber = malloc(sizeof(int));
      *threadNumber = threadCounter;
      if (pthread_create(&(threadId[threadCounter]), NULL, printThreads, threadNumber) == ERROR)
      {
         printf("Error in creating threads");
         _exit(EXIT_FAILURE);
      }
      threadCounter++;
   }
}

/**
 * Function that creates finish all threads
 * 
 * To see more about documentation please go to inc/main.h
 * 
 **/
void terminateThreads(void)
{  
   uint8_t threadNumber = 0;
   while (threadNumber < NUMBER_OF_THREADS)
   {
      if(pthread_join(threadId[threadNumber], NULL) != SUCCESS)
      {
         perror("A problem occurs when trying to terminate thread\n");
      }
      threadNumber++;
   }
}

/**
 * Function that prints in the terminal the letter referring to the thread in question
 * 
 * To see more about documentation please go to inc/main.h
 * 
 **/
void *printThreads(void *args)
{

   sem_wait(&semaphore);
   
   for (uint8_t counter =0; counter < printOrder[*(int*)args].numberOfPrints; counter++)
   {
         printf("-%s\n", printOrder[*(int*)args].character);
   }

   usleep(1000);
   sem_post(&semaphore);
   free(args);
}
