#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "headMenu.h"

long double factorial (long double numberMax, long double numberMin, long double result);
void factorialPipe(long double numberMax, long double numberMin, int *fd);

const int READ = 0;
const int WRITE = 1;
//calculate factorial
long double factorial (long double numberMax, long double numberMin, long double result){
    if(numberMax == (numberMin - 1)){
        return result;
    }else if(numberMax){
        return numberMax * factorial(numberMax - 1, numberMin, result);
    }else{
        return result;
    }
}

//calculate factorial using multi process
void factorialPipe(long double numberMax, long double numberMin, int *fd){
    long double result = factorial(numberMax, numberMin, 1);
    write(fd[WRITE], &result, sizeof(result));
    printf("[son] pid %d [parent] pid %d\n", getpid(), getppid());
    wait(NULL);
}

int main(int argc, char *argv[]){

    menu();

    // Input user
    int numberProcess, numberFactorial;
    printf("Digite o numero do fatorial: ");
    scanf("%d", &numberFactorial);
    printf("Digite a quantidade de Processos: ");
    scanf("%d", &numberProcess);

    if((numberFactorial < 0) || (numberProcess < 0)){
        printf("Input Error");
        exit(0);
    }

    //organize array with process
    if(numberProcess > numberFactorial) numberProcess = numberFactorial / 2;
    

    long double numberFactorialMax[numberProcess];
    long double numberFactorialMin[numberProcess];

    if(numberProcess == 1){
        numberFactorialMax[0] = numberFactorial;
        numberFactorialMin[0] = 0;
    }else{
        numberFactorialMax[0] = (numberFactorial / numberProcess) - 1;
        numberFactorialMin[0] = 0;
        for(int i = 1; i < numberProcess; i++){
            numberFactorialMin[i] = numberFactorialMax[i-1]+1;
            if(i == numberProcess - 1){
                numberFactorialMax[i] = numberFactorial;
            }else{
                numberFactorialMax[i] = numberFactorialMin[i] + (numberFactorial/numberProcess)-1;
            }
        }
    }
    
    //children process
    pid_t pid;
    int fd[numberProcess][2];
    for(int i = 0; i < numberProcess; i++){
        if(pipe(fd[i]) == -1){
            printf("Pipe Error\n"); 
        }
        pid = fork();
        if(pid == -1){
            printf("Fork Error\n");
        }else if(pid == 0){
            close(fd[i][READ]);
            factorialPipe(numberFactorialMax[i], numberFactorialMin[i], fd[i]);
            close(fd[i][WRITE]);
            exit(0);    
        }
    }

    //parent process
    if(pid != 0){
        long double resultChildren;
        long double result = 1;
        for(int i = 0; i < numberProcess; i++){
            close(fd[i][WRITE]);
            wait(NULL);
            read(fd[i][READ], &resultChildren, sizeof(resultChildren));
            result = result * resultChildren;
            close(fd[i][READ]);
        }
        printf("Resultado: %Lf\n", result);
    }
    
    return 0;
}