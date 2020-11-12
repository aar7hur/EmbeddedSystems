#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <errno.h>
#include <sys/wait.h>
#include <stddef.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>

int main(int argc, char *argv[])
{
    int fileDescriptor[2], nbytes;
    pid_t childpid;
    char* readBuffer = calloc(10, sizeof(char));
    char* string = calloc(10, sizeof(char));
    strcpy(string, "teste");

    pipe(fileDescriptor);

    childpid = fork();
    if (childpid == 0)
    {
        printf("Child process");
        close(fileDescriptor[0]);
        write(fileDescriptor[1], string, 5);
    }
    else
    {
        printf("Parent process");
        close(fileDescriptor[1]);
        nbytes = read(fileDescriptor[0], readBuffer, sizeof(readBuffer));
        printf("FInalValue: %s", readBuffer);
    }
    free(readBuffer);
    free(string);
}