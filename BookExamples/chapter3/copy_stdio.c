#include <stdlib.h>
#include <stdio.h>

int main()
{
    int c;
    FILE* in = fopen("file.in", "r");
    FILE* out = fopen("file.out", "w");
    while((c = fgetc(in)) != EOF) fputc(c, out);
    exit(0);
}