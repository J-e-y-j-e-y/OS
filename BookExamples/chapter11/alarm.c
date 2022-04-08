#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

static int alarm_fired = 0;

void ding(int sig){
    alarm_fired = 1;
}

int main(){
    pid_t pid;
    printf("Start process forking\n");
    pid = fork();

    switch(pid){
        case -1:
            perror("Something was wrong!\n");
            exit(1);
        case 0:
            sleep(5);
            kill(getppid(), SIGALRM);
            exit(0);
    }

    printf("waiting for alarm to go off\n");
    (void) signal(SIGALRM, ding);
    pause();
    if(alarm_fired) printf("DING!\n");
    printf("Done\n");
    exit(0);
}