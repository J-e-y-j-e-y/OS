#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
    pid_t pid;
    int n;
    char* message;

    pid = fork();

    switch(pid){
        case -1:
            perror("Failed to fork a process");
            exit(1);
        case 0:
            n = 5;
            message = "Child's process!!!";
            break;
        default:
            n = 3;
            message = "Parent's process!!!";
    }

    for(; n > 0; n--){
        puts(message);
        sleep(1);
    }
    exit(0);
}