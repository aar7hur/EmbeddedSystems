#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <errno.h>
#include <sys/wait.h>
#include <stddef.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>

/****
 * 
 * Use this scripts to test how pip works.
 * The main goals is to communicate between two proccess.
 * The communication works by passing an writing an scruct 
 * into file file descriptor on child process and reading 
 *
 * */


/****
 * 
 * Struct used to write/read from all process
 * 
 ****/
typedef struct {
    int initValue;
    int finalValue;
    int result;
} pipeStruct;

void* allocateMemory(size_t size);

int main(int argc, char *argv[])
{
    int fileDescriptor[2], nbytes;
    pid_t childpid;
    pipeStruct *data = allocateMemory(sizeof(*data));
    pipe(fileDescriptor);

    childpid = fork();
    if (childpid == 0)
    {
        printf("Child process");
        close(fileDescriptor[0]);
        data->result = 90;
        write(fileDescriptor[1], data, sizeof(pipeStruct));
    }
    else
    {
        printf("Parent process");
        close(fileDescriptor[1]);
        nbytes = read(fileDescriptor[0], data, sizeof(pipeStruct));
        printf("FInalValue: %d", data->result);
    }
    free(data);
}
/******
 * 
 * function: *allocarteMemory
 * :param size: size of memory to be allocated
 * :return pointer that can be casted to any type
 * 
 */
void *allocateMemory(size_t size)
{
    void *p = malloc(size);
    if(!p)
    {
        perror("Cannot allocate memory");
        exit(EXIT_FAILURE);
    }
    return p;
}

