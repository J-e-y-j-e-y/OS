#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define _GNU_SOURCE
#include <getopt.h>

int main(int argc, char* argv[]){

 int opt;
 struct option longopts[4] = {
    {"initialize", 0, NULL, 'i'}, 
    {"file", 1, NULL, 'f'},
    {"list", 0, NULL, 'l'},
    {0, 0, 0, 0}
 };

    while((opt = getopt_long(argc, argv, ":if:lr", longopts, NULL)) != -1){
        switch(opt){
            case 'i':
            case 'l':
            case 'r':
                printf("option: %c\n", opt);
                break;
            case 'f':
                printf("filename: %s\n", optarg);
                break;
            case ':':
                printf("option needs a value\n");
                break;
            case '?':
                printf("unknown option: %c\n", optopt);
                break;
        }
    }

    for (; optind < argc; optind++)
        printf("argument: %s\n", argv[optind]);
        

 exit(0);
}