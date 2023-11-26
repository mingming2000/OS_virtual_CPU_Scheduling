#include <linux/syscalls.h>
#include <linux/kernel.h>
#include <linux/linkage.h>
#include <linux/sched.h>

#define MAX 300                 // size of queue
#define IDLE -1                 // define 'IDLE' as int -1 (PCB: 3~5 integer value)

static pid_t queue[MAX];      // Queue dtype: int array, size: 300, initialized by 0 
static int head = -1;            // points head of Q
static int tail = -1;            // points tail of Q
static int now = IDLE;

typedef struct
{
    pid_t pid;
    int job;
}job_t;

int is_ready_queue_empty(void);
pid_t ready_queue_pop(void);
int is_process_new(int);
void ready_queue_push(job_t);


SYSCALL_DEFINE2(os2023_ku_cpu1, char*, name, int, job){
    /*
        System Call Function : os2023_ku_cpu1 (FCFS)
    */
    
    // Store current PID
    job_t new_job = {current->pid, job};

    // Case 1: CPU idle
    if (now == IDLE){
        now = new_job.pid;
        return 0;       // Accepted
    }
    // Case 2: current process == requested process
    else if (now == new_job.pid){
        // If the job has finished
        if (job == 0){
            printk("\033[32m[FINISH]  \033[0m Process: %s\n", name);

            // Check whether the ready Q is empty
            if(is_ready_queue_empty())
                now = IDLE;
            else
                now = ready_queue_pop();
        }
        else{
            printk("\033[33m[WORKING] \033[0m Process: %s\n", name);
        }
        return 0;      // Accepted
    } else{
        // Case 3: current process != requested process
        // If the process is not already in Waiting Q
        if(is_process_new(new_job.pid))
            ready_queue_push(new_job);
            
            printk("\033[31m[DENIED]  \033[0m Process: %s\n", name);
        return 1;   // Rejected
    }
}

int is_ready_queue_empty(){
    // If ready queue is empty
    if(head == tail)
        return 1;
    // If ready queue is not empty
    else
        return 0;
}

pid_t ready_queue_pop(){
    // Return head of Q
    int out;
    
    out = head;
    head += 1;
    return queue[out];
}

int is_process_new(int new_process){
    int i = 0;

    for(i = head; i <= tail; i++){
        printk("%d", queue[i]);
        if(queue[i] == new_process)
            return 0;
    }
    return 1;
}

void ready_queue_push(job_t new_process){
    tail += 1;
    queue[tail] = new_process.pid;
}