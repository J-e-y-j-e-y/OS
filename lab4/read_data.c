#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
    int c;
    FILE* in = fopen(argv[1], "r");
    while((c = fgetc(in)) != EOF) fputc(c, stdout);
}