/*
 * Santiago Andaluz Ruiz
 * Writer.c
 * CSI 4337
 */


#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    //Opening the file
    int output;
    output = open("/dev/null", O_RDONLY);

    //What is being written to the file
    char junk[] = "1234567890";

    //Writing the char array 100,000,000 times
    for ( int i = 0; i < 100000000; i++ ) { 
        write(output, junk, 10);
    }

    close(output);

    return 0;
}
