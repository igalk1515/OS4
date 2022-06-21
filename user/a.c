#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"
#include "kernel/syscall.h"
#include "kernel/memlayout.h"
#include "kernel/riscv.h"

/* Sanity test for task 1*/
void
test2(void){
    int fd;
    char data[1024];

    // fills the data array with 7s
    memset(data, 7, 1024);
    fd = open("file_test", O_CREATE | O_RDWR);
    for(int i = 0; i < 1024*10; i++){
        write(fd, data, sizeof(data));
        if (i == 10)
            printf("Finished writing 12KB (direct)\n");
        if (i == 268)
            printf("Finished writing 268KB (single direct)\n");
        if (i!=0 && i%1024 == 0)
            printf("Finished writing %dM\n",(i/1024));
        
    }
    printf("Finished writing 10MB\ntest finished\n");
    close(fd);
}

void
task3_test(void){
    int fd;
    char data[1024];

    // fills the data array with 7s
    fd = open("sanity_check_3", O_CREATE | O_RDWR);
    write(fd, "data", sizeof(data));
}


int main(int argc, char* argv[]){
    test2();
    //task3_test();

    exit(1);
}