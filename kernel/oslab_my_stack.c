#define MAX = 300
static int arr[MAX] = {0};
static int top = -1;

SYSCALL_DEFINE1(os2023_push, int, a){

    if(top == MAX-1){
        printf("ERROR: STACK OVERFLOW");
    }
    else{
        top++;
        arr[top] = a;
        printf("Push : %3d\n", a);

        printk("Stack Top --------------------\n");
        for(int i = 0; i <= top; ++i){
            printk("%3d\n", arr[i]);
        }
        printk("Stack Bottom -----------------\n");
    }
}

SYSCALL_DEFINE2(os2023_pop){
    if(top == -1){
        printf("ERROR: STACK UNDERFLOW");
    }
    else{
        printf("Pop : %3d", arr[top]);
        top--;

        printk("Stack Top --------------------\n");
        for(int i = 0; i <= top; ++i){
            printk("%3d\n", arr[i]);
        }
        printk("Stack Bottom -----------------\n");
    }
}