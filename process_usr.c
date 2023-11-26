#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MY_CPU 337  // ku_cpu(virtual CPU)  : #337 in the syscall_64.tbl

int main(int argc, char **argv){
    int job;
    int delay;
    char name[4];
    int wait = 0;

    if(argc < 4){
        printf("\nError: Insufficient Arguments\n");
        return 1;
    }

    job = atoi(argv[1]);            // Arg #1: TIme required for job (second)
    delay = atoi(argv[2]);          // Arg #2: Delay time before exe. (second)
    strcpy(name, argv[3]);          // Arg #3: Name of process

    // Delay before execution
    sleep(delay);                  
    
    printf("\nProcess: %s | Job time: %d(s)\n", name, job);
    job *= 10;                      // Execute syscall in every 0.1(s)

    // Execution
    while(job){
        // Accepted request
        if(!syscall(MY_CPU, name, job)) job--;
        // Rejected request
        else{
            wait++;
        }
        usleep(100000);             // Delay 0.1 second
    }

    syscall(MY_CPU, name, 0);
    printf("\n[Finish] Process: %s | Total wait time %d\n", name, (wait+5)/10);
    return 0;
}