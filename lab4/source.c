#include <stdio.h>
#include <stdlib.h>

char* source_file = "out.txt";

int main(){
    int c;
    FILE* in = fopen(source_file, "r");
    while((c = fgetc(in)) != EOF) fputc(c, stdout);
}