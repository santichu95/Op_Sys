/**
 * Santiago Andaluz Ruiz 
 * Program 0
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main( int argc, char* argv[] ) {
    //Checking for right number of agruments
    if ( argc != 3 ) {
        fprintf(stderr, "Expected %s <file name> <number of processes>\n", argv[0]);
        exit(1);
    } 

    FILE* input;
    int numProc;
    int x, y;
    int start, end;

    //Pipe
    int fd[2];

    //Check if the files exists
    input = fopen(argv[1], "r");

    if( input == NULL ) {
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
    int numberPoints;
    
    fscanf(input, "%d", &numberPoints);

    /* fprintf(stdout, "%d\n", numberPoints); */

    //int points[][] = (int *) malloc(sizeof(int) * numberPoints);
   
    int points[numberPoints][2];

    for ( int i = 0; i < numberPoints; i++ ) {
        fscanf(input, "%d %d", &x, &y);

        points[i][0] = x;
        points[i][1] = y;
    }

    /*
    for ( int i = 0; i < numberPoints; i++ ) {
        x = points[i][0];
        y = points[i][1];

        fprintf(stdout, "%d %d\n", x, y);
    }
    */

    //Create Pipe
    
    pipe(fd);
   
    //Fork and thread
    
    //Parent counts as one of the proc
    numProc--;
    
    for( int i = 0; i < numProc; i++; ) {

    }

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
