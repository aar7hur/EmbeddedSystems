#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <errno.h>
#include <sys/wait.h>
#include <stddef.h>
#include <unistd.h>
#include <stdint.h>

/*
*   Defines:
*        MAX_NUMBER_PROCESS: Represents the number...
*
*
*/
#define MAX_NUMBER_PROCESS 100
#define NELEMS(x)  (sizeof(x) / sizeof((x)[0]))

/*
*   Structs and Enumerators:
*
*
*/
typedef struct {
    int initNumber;
    int finalNumber;
    long double result;
} processStruct;

typedef enum { 
    False,
    True 
} boolean;

/*
*   Functions declarations
*
*
*/

long double factorial(int number);
uint8_t* createProccess(uint8_t* processVector);
boolean validUserInput(int argc, char *argv[]);
void* allocateMemory(size_t size);

static uint8_t argvNumberProcess;
static int argvFat;



int main(int argc, char *argv[])
{
    
    if (validUserInput(argc, argv) == False)
    {
        exit(EXIT_FAILURE);
    }
   
    uint8_t* processIds = allocateMemory(argvNumberProcess);
    processIds = createProccess(processIds);
    exit(EXIT_SUCCESS);
    for (int i = 0; NELEMS(processIds); i++)
    {
        printf("ID: %d\n", processIds[i]);
    }


    /*long double fat;
    clock_t inicial = clock();
    fat = factorial(100);
    clock_t final = clock();
    printf("O fatorial é %Lf", fat);
    printf("A aplicação durou: %f segundos", (double)(final-inicial)/CLOCKS_PER_SEC);
    test = createProccess()

  */
}

boolean validUserInput(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Faltam argumentos para rodar a aplicação");
        return False;
    }

    argvNumberProcess = (uint8_t)atoi(argv[2]);
    argvFat = atoi(argv[1]);

    if ((argvFat < 0) || (argvNumberProcess < 0) || (argvNumberProcess > MAX_NUMBER_PROCESS))
    {
        fprintf(stderr, "Os argumentos precisam ser maiores do que 0.");
        return False;
    }
    else
    {
        return True;
    }
    
}

uint8_t* createProccess(uint8_t* processVector)
{  

    for(int iterator = 0; iterator < NELEMS(processVector); iterator ++)
    {
        printf("%d", iterator);
        processVector[iterator] = fork();
        if (processVector[iterator] < 0)
        {
            fprintf(stderr, "Erro na criação do processo filho\n");
        }
    }
    return processVector;
}


long double factorial(int number)
{
    if (number >= 1)
    {
        return number*factorial(number-1);
    }
    else
    {
        return 1;
    }
    
}

void* allocateMemory(size_t size)
{
    void* pointer = calloc(size, sizeof(uint8_t));
    if(!pointer)
    {
        perror("Cannot allocate meemory");
        exit(EXIT_FAILURE);
    }
    return pointer;
}