#include <stdlib.h>
#include <unistd.h>

int main()
{
    char buf[128];
    int nread;
    nread = read(0, buf, 128);

    if(nread == -1)
        write(2, "Error occured while reading!\n", 29);

    if(write(1, buf, nread) != nread)
        write(2, "Error occured while writing!\n", 29);
        
    exit(0);
}