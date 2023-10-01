#include "linux/syscalls.h"
#include "linux/kernel.h"
#include "linux/linkage.h"

#define MAX 300
static int arr[MAX] = {0};
static int top = -1;

SYSCALL_DEFINE1(os2023_push, int, a){
    int i;

    if(top == MAX-1){
        printk("ERROR: STACK OVERFLOW");
    }
    else{
        top++;
        arr[top] = a;

        printk("Stack Top --------------------\n");
        for(i = 0; i <= top; ++i){
            printk("%3d\n", arr[i]);
        }
        printk("Stack Bottom -----------------\n");
    }

    return a;
}

SYSCALL_DEFINE0(os2023_pop){
    int push, i;

    if(top == -1){
        printk("ERROR: STACK UNDERFLOW");
    }
    else{
        push = arr[top];
        top--;

        printk("Stack Top --------------------\n");
        for(i = 0; i <= top; ++i){
            printk("%3d\n", arr[i]);
        }
        printk("Stack Bottom -----------------\n");
    }

    return push;
}