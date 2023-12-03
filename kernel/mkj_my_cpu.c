#include <linux/syscalls.h>
#include <linux/kernel.h>
#include <linux/linkage.h>
#include <linux/sched.h>

#define MAX 3000                 // size of queue
#define IDLE -1                 // define 'IDLE' as int -1 (PCB: 3~5 integer value)

static int head = -1;            // points head of Q
static int tail = -1;            // points tail of Q
static int len = 0;
// static int timer = 0;
// static char* queue_name[MAX];

typedef struct
{
    pid_t pid;
    int job;
}job_t;

// typedef struct
// {
//     pid_t pid;
//     char* name;
// }name_dictionary;


job_t queue[MAX];               // Queue dtype: job_t, size: MAX, initialized by 0, job: Execution time
job_t now = {IDLE, 0};                      // Variable to keep track of process recently occupying CPU
job_t new;
// static job_t sj_processss;              // Variable which indicating Shortest Job Process

int is_ready_queue_empty(void);
pid_t ready_queue_pop_pid(void);
job_t ready_queue_pop(void);
pid_t ready_queue_pop_shortest(void);
int is_process_new(pid_t);
void ready_queue_push(job_t);
void ready_queue_push_sort(job_t);
job_t shortest_process(void);
// char* find_name(pid_t);

void print_ready_queue(void);       // for debugging

SYSCALL_DEFINE2(os2023_ku_cpu1, char*, name, int, job){
    /*
        System Call Function : os2023_ku_cpu1 (FCFS)
    */
    
    // Store current PID
    job_t new_job = {current->pid, job};

    // Case 1: CPU idle
    if (now.pid == IDLE){
        now.pid = new_job.pid;

        // For debugging
        // printk("now.pid: %d", now.pid);
        // print_ready_queue();

        return 0;       // Accepted
    }
    // Case 2: current process == requested process
    else if (now.pid == new_job.pid){
        // If the job has finished
        if (job == 0){
            printk("\033[32m[FINISH]  \033[0m Process: %s\n", name);

            // Check whether the ready Q is empty
            if(is_ready_queue_empty())
                now.pid = IDLE;
            else
                now.pid = ready_queue_pop_pid();       // pop head(pid)
        }
        else{
            printk("\033[33m[WORKING] \033[0m Process: %s\n", name);
        }

        // For debugging
        // printk("now.pid: %d", now.pid);
        // print_ready_queue();
        
        return 0;      // Accepted
    } else{
        // Case 3: current process != requested process
        // If the process is not already in Waiting Q
        if(is_process_new(new_job.pid))
            ready_queue_push(new_job);
            
        printk("\033[31m[DENIED]  \033[0m Process: %s\n", name);

        // For debugging
        // printk("now.pid: %d", now.pid);
        // print_ready_queue();

        return 1;   // Rejected
    }

}


SYSCALL_DEFINE2(os2023_ku_cpu_sjf, char*, name, int, job){
    /*
        System Call Function : os2023_ku_sjf (SJF)
    */
    
    
    // Store current PID
    job_t new_job = {current->pid, job};

    // Case 1: CPU idle
    if (now.pid == IDLE){
        now = new_job;

        // For debugging
        // printk("now: %d", now);
        // print_ready_queue();

        return 0;       // Accepted
    }
    // Case 2: current process == requested process
    else if (now.pid == new_job.pid){
        // If the job has finished
        if (job == 0){
            printk("\033[32m[FINISH]  \033[0m Process: %s\n", name);

            // Check whether the ready Q is empty
            if(is_ready_queue_empty())
                now.pid = IDLE;
            else
                now = ready_queue_pop();       // Pop head(job_t type)
        }
        else{
            printk("\033[33m[WORKING] \033[0m Process: %s\n", name);
        }

        // For debugging
        // printk("now.pid: %d", now.pid);
        // print_ready_queue();
        
        return 0;      // Accepted
    } else{
        // Case 3: current process != requested process
        // If the process is not already in Waiting Q
        if(is_process_new(new_job.pid))
            ready_queue_push_sort(new_job);     // Enqueue process and sort ready queue
            
        printk("\033[31m[DENIED]  \033[0m Process: %s\n", name);

        // For debugging
        // printk("now.pid: %d", now.pid);
        // print_ready_queue();

        return 1;   // Rejected
    }

}

SYSCALL_DEFINE2(os2023_ku_cpu_srtf, char*, name, int, job){
    /*
        System Call Function : os2023_ku_srtf (SRTF)
    */
    
    
    // Store current PID
    job_t new_job = {current->pid, job};

    //For debugging
    // printk("\n\nnow.pid: %d", now.pid);
    // print_ready_queue();

    // Case 1: CPU idle
    if (now.pid == IDLE){
        now = new_job;


        return 0;       // Accepted
    }


    // Case 2: current process != requested process
    // If the process is not already in Waiting Q
    if(is_process_new(new_job.pid)){

        if(new_job.job < now.job){
            // If job of new process is shorter than job of running process, change process to new one.

            ready_queue_push_sort(now);
            // printk("[Push] now pid: %d job: %d \n", now.pid, now.job);   // For debugging
            printk("\033[33m[WORKING] \033[0m Process: %s\n", name);
            now = new_job;
            return 0;           // Accepted
        }
        else{
            // Push new process 
            ready_queue_push_sort(new_job);
            printk("\033[31m[DENIED]  \033[0m Process: %s\n", name);
            return 1;   // Rejected
        }
    }
    else{
        // Case 3: current process == requested process
        if (now.pid == new_job.pid){

            // If the job has finished
            if (job == 0){
                printk("\033[32m[FINISH]  \033[0m Process: %s\n", name);

                // Check whether the ready Q is empty
                if(is_ready_queue_empty())
                    now.pid = IDLE;
                else
                    now = ready_queue_pop();       // pop the shortest process in the ready queue
            }
            else{
                printk("\033[33m[WORKING] \033[0m Process: %s\n", name);
            }
            
            return 0;      // Accepted
        }
        else{
            printk("\033[31m[DENIED]  \033[0m Process: %s\n", name);
            return 1;   // Rejected
        }
    }
}

/*
SYSCALL_DEFINE2(os2023_ku_cpu_rr, char*, name, int, job){
    
        System Call Function : os2023_ku_rr (RR)
    
    
    
    // Store current PID
    job_t new_job = {current->pid, job};

    // Case 1: CPU idle
    if (now.pid == IDLE){
        new_job.job--;
        now = new_job;
        printk("\033[33m[WORKING] \033[0m Process: %s\n", name);

        // For debugging
        // printk("now: %d", now);
        // print_ready_queue();

        return 0;       // Accepted
    }

    // Case 2: Enqueue new process
    if(is_process_new(new_job.pid)){
        ready_queue_push(new_job);              // Enqueue process to ready_queue
    }

    // Case 3-2: (Process finish)
    if(now.job == 0){
        now = ready_queue_pop();
        
        return 0;
    }

    // Case 3-1: Execute next process (timeout)
    if(timer == 9){
        now.job--;
        timer = 0;
        ready_queue_push(now);
        printk("\033[33m[WORKING] \033[0m Process: %s\n", find_name(now.pid));
        now = ready_queue_pop();

        return 0;
    }
    else{
        printk("\033[33m[WORKING] \033[0m Process: %s\n", find_name(now.pid));

        return 0;
    }



}
*/

int is_ready_queue_empty(){
    // If ready queue is empty
    if(len == 0)
        return 1;
    // If ready queue is not empty
    else
        return 0;
}

pid_t ready_queue_pop_pid(){
    // Return head of Q
    int out;
    
    out = head;
    if(len != 1)
        head += 1;

    len--;
    return queue[out].pid;
}

job_t ready_queue_pop(){
    // Return head of Q
    int out;
    
    out = head;
    if(len == 0){
        printk("\033[31m[Underflow]\033[0m No element to pop");
    }
    else if(len > 1){
        head += 1;
    }

    len--;
    return queue[out];
}


pid_t ready_queue_pop_shortest(){
    // Return head of Q
    int out;
    int i;
    int shortest_job;
    int shortest_idx = head;
    job_t tmp_queue;

    shortest_job = queue[head].job;
    for(i = head+1; i <= tail; i++){
        if(shortest_job > queue[i].job){
            shortest_idx =  i;
        }
    }

    tmp_queue = queue[shortest_idx];
    queue[shortest_idx] = queue[head];
    queue[head] = tmp_queue;

    out = head;
    if(len != 1)
        head += 1;

    len--;
    return queue[out].pid;
}


int is_process_new(pid_t new_process_pid){
    int i = 0;

    if(new_process_pid == now.pid){
        return 0;
    }
    
    for(i = head; i <= tail; i++){
        // printk("%d", queue[i]);          // For debugging
        if(queue[i].pid == new_process_pid)
            return 0;
    }
    return 1;
}

void ready_queue_push(job_t new_process){
    if(len == 0){
        head++;
        tail++;
    }
    else{
        tail += 1;
    }
    len++;
    queue[tail] = new_process;
}

void ready_queue_push_sort(job_t new_process){
    
    int i;
    int j;

    if(len == 0){
        head++;
        tail++;
        queue[head] = new_process;
    }
    else{
        for(i = head; i <= tail; i++){
            if(new_process.job < queue[i].job){
                for(j = tail; j >= i; j--){
                    queue[j+1] = queue[j];
                }
                queue[i] = new_process;
            }
        }
        tail += 1;
    }
    len++;
}

job_t shortest_process(){
    if(now.job > queue[head].job){
        new = ready_queue_pop();    // Pop shortest process
        ready_queue_push(now);      // Push working process

        return new;
    }
    else{
        return now;
    }
}

// char* find_name(pid_t _pid){
//     int i = 0;

//     for(i=0; i<MAX; i++){
//         if(_pid == name_dictionary[i].pid){
//             break;
//         }
//     }
//     return name_dictionary[i].name;
// }


// for debugging
void print_ready_queue(void){
    int i;
    printk("****head #%d", head);
    for(i = head; i <= tail; ++i)   printk("Ready Queue: #%d pid: %d job: %d",i, queue[i].pid, queue[i].job);
    printk("****tail #%d", tail);
}       