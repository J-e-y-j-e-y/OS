#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <pwd.h>
#include <linux/limits.h>

int main(){
    char* input[1024];
    pid_t pid;
    int stat;
    pid_t child_pid;
    
    while(1){
        char PathName[PATH_MAX];
        char UserName[1024];
		char* PN = getwd (PathName);
        getlogin_r(UserName, 1024);
        printf("\n%s-@@term@@: %s $ ", UserName, PathName);
        gets(input);

        if(strcmp(input, "quit") == 0)
            break;

        pid = fork();    
        switch(pid){
            case -1:
                perror("\nTerminal is not working :(");
                break;
            case 0:
                for(int i = 0; i < 10; i++){
                    printf("\nChild");
                    sleep(1);
                }
                exit(0); 
        }
        if(pid != 0){
            child_pid = wait(&stat);
            printf("Child process has finished : PID = %d\n", child_pid);
        }
    }
}