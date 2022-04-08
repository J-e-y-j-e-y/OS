#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(){
    printf("\nCalling ps with execlp\n");
    execlp("ps", "ps", "ax", 0);
    printf("\nDone!\n");
    exit(0);
}