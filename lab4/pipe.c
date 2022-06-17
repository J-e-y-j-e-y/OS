#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <malloc.h>

#define SIZE 1024
#define MAXGAMMASIZE 2048
#define SIXTEEN 16
#define DEST "out.txt"

void xor_func(char* source, char* gamma, char* dest);

int main(int argc, char *argv[]){
    int source_pipe[2];
    int gamma_pipe[2];
    pid_t fork_res;
    pid_t fork_res_2;

    if(pipe(source_pipe) == 0){
        fork_res = fork();
        if(fork_res == -1){
            fprintf(stderr, "Fork failure");
            exit(EXIT_FAILURE);
        }
        if(fork_res == 0){
            close(1);
            dup(source_pipe[1]);
            close(source_pipe[0]);
            close(source_pipe[1]);
            printf("\ncalling %s", argv[1]);
            execlp(argv[1], argv[1], (char*)0);
            exit(EXIT_FAILURE);
        }
        else{
            close(source_pipe[1]);
            
            if(pipe(gamma_pipe) == 0){
                fork_res_2 = fork();
                if(fork_res_2 == -1){
                    fprintf(stderr, "Fork failure");
                    exit(EXIT_FAILURE);
                }
                if(fork_res_2 == 0){
                    close(1);
                    dup(gamma_pipe[1]);
                    close(gamma_pipe[0]);
                    close(gamma_pipe[1]);
                    printf("\ncalling %s", argv[2]);
                    execlp(argv[2], argv[2], (char*)0);
                    exit(EXIT_FAILURE);
                }else{
                    close(gamma_pipe[1]);
                    int c;
                    
                    char gamma_key[MAXGAMMASIZE];
                    int BLOCK_SIZE = read(gamma_pipe[0], gamma_key, MAXGAMMASIZE);
                    printf("\n%.*s  block = %d", (int)strlen(gamma_key), gamma_key, BLOCK_SIZE);
                    

                    int out = open(DEST, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
                    if(out == -1){
                        printf("\nCan't open file to write");
                    }else{
                        printf("\nSuccessfully opened file!");
                    }

                    char sourcebuf[SIZE];
                    sourcebuf[SIZE-1] = '\0';
                    int offset = 0;
                    char dest[SIZE];
                        
                    while((c = read(source_pipe[0], sourcebuf, SIZE-1)) > 0){
                        
                        printf("\n--------------------------------");
                        printf("\ntrying to write %s", sourcebuf);
                        for(int i = 0; i < c; i++){    
                            dest[i] = sourcebuf[i] ^ gamma_key[offset];
                            offset++;
                            if(offset >= BLOCK_SIZE)
                                offset = 0;
                        }
                        printf("\n--------------------------------");
                        printf("\n[%.*s] size = %d", SIZE, dest, SIZE);
                        
                        write(out, dest, c);
                    } 


                }
            }    
        }
    }

    exit(EXIT_SUCCESS);
}

void xor_func(char* source, char* gamma, char* dest){
    printf("\n--------------------------------");
    printf("\ntrying to write %s", source);

    printf("\nmodification %s", dest);
    printf("\n--------------------------------");
}