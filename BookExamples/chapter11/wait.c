#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
    pid_t pid;
    int n;
    char* message;
    int exit_code;

    pid = fork();

    switch(pid){
        case -1:
            perror("Failed to fork a process");
            exit(1);
        case 0:
            n = 5;
            message = "Child's process!!!";
            exit_code = 37;
            break;
        default:
            n = 3;
            message = "Parent's process!!!";
            exit_code = 0;
    }

    for(; n > 0; n--){
        puts(message);
        sleep(1);
    }

    if(pid != 0){
        int stat;
        pid_t child_pid;
        child_pid = wait(&stat);
        printf("Child process has finished : PID = %d\n", child_pid);

        if(WIFEXITED(stat)){
            printf("Child process has finished with status = %d\n", WEXITSTATUS(stat));
        }else{
            printf("Child process has finished abnormally\n");
        }
    }
    exit(exit_code);
}