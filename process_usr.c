#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MY_CPU_FCFS 337  // ku_cpu(FCFS): #337 in the syscall_64.tbl
#define MY_CPU_SJF 338  // ku_cpu(SJF)  : #338 in the syscall_64.tbl
#define MY_CPU_SRTF 339 // ku_cpu(SRTF) : #339 in the syscall_64.tbl
#define MY_CPU_RR 340   // Ku_cpu(RR)   : #340 in the syscall_64.tbl

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
        // if(!syscall(MY_CPU_FCFS, name, job)){            // FCFS
        // if(!syscall(MY_CPU_SJF, name, job)){             // SJF
        if(!syscall(MY_CPU_SRTF, name, job)){               // SRTF
        // if(!syscall(MY_CPU_RR, name, job)){              // RR

            job--;
        }
        // Rejected request
        else{
            wait++;
        }

        usleep(100000);             // Delay 0.1 second
    }

    syscall(MY_CPU_RR, name, 0);       // Finish Process
    printf("\n[Finish] Process: %s | Total wait time %d\n", name, (wait+5)/10);
    return 0;
}