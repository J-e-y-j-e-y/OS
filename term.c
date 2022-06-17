#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <pwd.h>
#include <linux/limits.h>
#include <signal.h>

const char *SEPARATOR = " ";

void ouch(int sig){
    printf("OUCH! I've got signal - %d\n", sig);
    (void) signal(SIGINT, SIG_DFL);
}

int main(){
    char input[1024];
    char command[100];
    pid_t pid;
    int stat;
    pid_t child_pid;

    (void) signal(SIGINT, ouch);

    while(1){
        char PathName[PATH_MAX];
        char UserName[1024];
		char* PN = getwd (PathName);
        getlogin_r(UserName, 1024);
        printf("\n%s-@@term@@: %s $ ", UserName, PathName);
        gets(input);

        char* ptr = input;
        int size;
        int space_counter = 0;
        char com = 0;
        for(size = 0; size < strlen(input); size++){
          //  printf("%s\n", ptr);
            if(input[size] == ' '){
                com = 1;
                space_counter++;
            }
            if(!com)
                command[size] = input[size];
        }
        command[size] = '\0';
        
        char* ps_argv[space_counter + 2];

        char *token = strtok(input,SEPARATOR);
        int toks = 0;
        while (token != NULL)
        {
            printf("\ntoken = %s", token);
            ps_argv[toks] = token;
            toks++;
            token = strtok(NULL, SEPARATOR);
        }
        ps_argv[toks] = 0;

        if(strcmp(input, "quit") == 0)
            break;

        pid = fork();    
        switch(pid){
            case -1:
                perror("\nTerminal is not working :(");
                break;
            case 0:
                printf("%s -> '%s'", command, input);
                execvp(command, ps_argv);
                //exit(0); 
        }


        if(pid != 0){
            //printf("toks[last-1] = %s", ps_argv[toks-1]);
            if(strcmp(ps_argv[toks-1], "&") != 0){
                child_pid = wait(&stat);
                printf("\nChild process has finished : PID = %d\n", child_pid);
            }
        }
    }
}