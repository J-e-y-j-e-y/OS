#include <stdio.h>
#include <stdlib.h>

char* gamma_file = "gamma.txt";

int main(){
    int c;
    FILE* in = fopen(gamma_file, "r");
    while((c = fgetc(in)) != EOF) fputc(c, stdout);
}