#include <linux/syscalls.h>
#include <linux/kernel.h>
#include <linux/linkage.h>

#define MAX 300
static int arr[MAX] = {0};
static int top = -1;

SYSCALL_DEFINE1(os2023_push, int, a){
    int i = 0;

    printk("\033[0;33m[System call] os2023_push\033[0m :\n");
    
    // Check Overflow.
    if(top == MAX-1){
        printk("\033[0;31mERROR: STACK OVERFLOW\033[0m\n");
        return -1;
    }
    else{
        // Check whether same value already exists in.
        for(i = 0; i <= top; ++i){
            if(a == arr[i]){
                printk("\033[0;31mERROR: Same value already exists in the stack.\033[0m");
                return -1;
            }
        }

        // Push
        top++;
        arr[top] = a;
        
        // Print values of the stack.
        printk("Stack Top --------------------\n");
        for(i = top; i >= 0; --i){
            printk("%3d\n", arr[i]);
        }
        printk("Stack Bottom -----------------\n");

        return a;
    }
}

SYSCALL_DEFINE0(os2023_pop){
    int pop = 0;
    int i = 0;
    int j = 0;
    int tmp = 0;

    printk("\033[0;33m[System call] os2023_pop\033[0m :\n");
    
    // Check Underflow
    if(top == -1){
        printk("\033[0;31mERROR: STACK UNDERFLOW\033[0m\n");
        return -1;
    }
    else{
        
        // Sort stack in ascending order. (Insertion Sort)
        for(i = 0; i <= top; ++i){
            for(j = 0; j < i; ++j){
                if(arr[j] > arr[i]){
                    tmp = arr[j];
                    arr[j] = arr[i];
                    arr[i] = tmp;
                }
            }
        }

        // Pop the maximum number in the stack.
        pop = arr[top];
        top--;

        // Print values of the stack.
        printk("Stack Top --------------------\n");
        for(i = top; i >= 0; --i){
            printk("%3d\n", arr[i]);
        }
        printk("Stack Bottom -----------------\n");

        return pop;
    }
}