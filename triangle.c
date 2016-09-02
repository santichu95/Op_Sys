/**
 * Santiago Andaluz Ruiz 
 * Program 0
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main( int argc, char* argv[] ) {
    if ( argc != 3 ) {
        write(1, "Wrong number of arguements\n", 
                strlen("Wrong number of arguements\n"));
        exit(1);
    } 

    FILE* input;
    int numProc;

    //Check if the files exists
    if( !( fopen(argv[1], "r")) ) {
        perror("FIle not found");
    }

    //Check if the number of programs is valid
    //i.e > 0
    numProc = atoi(argv[2]);
    
    if( numProc < 0 || numProc > 100 ) {
        fprintf( stderr, 
            "Number of processes was not in the acceptable bounds\n", 
            strlen("Number of processes was not in the acceptable bouds\n"));
    }

    
    //Read in the points

    //Create Pipe
   
    //Fork and thread

    //Figure out how to solve the question.

    //Have a selected point
    //Check if any two other points have negative reciprocal slopes
    
    //for each point {
    //Double for loop
    //for i = 0 ... numPoints - 1
    //  for j = 1 + 1 ... numPoints
    //  }
    
    return 0;
}
