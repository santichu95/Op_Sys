/**
 * Santiago Andaluz Ruiz 
 * Program 0
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

int  checkPoints(int xOne, int xTwo, int xThree, int yOne, int yTwo, int yThree) {
    int  ans;

    int  vertOne = 0, vertTwo = 0;

    int  rise, run; 
    double  slopeOne, slopeTwo;

    rise = yTwo - yOne;
    run = xTwo - xOne;
    

    if( run == 0 ) {
        vertOne = 1;
    } else if (rise == 0 ) {
        slopeOne = 0;
    } else {
        slopeOne = rise/run;
    }
    
    rise = yThree - yOne;
    run = xThree - xOne;

    if( run == 0 ) {
       vertTwo = 1;
    } else if ( rise == 0 ) {
       slopeTwo = 0;
    }  else {
       slopeTwo = rise/run;
    }

    if ( vertOne && slopeTwo == 0 ) {
        ans = 0;
    }
    
    if ( vertTwo && slopeOne == 0 ) {
        ans = 0;
    }
    
    slopeTwo *= -1;
    slopeTwo /= 1;

    if ( abs(slopeOne - slopeTwo) < .0001 ) {
        ans = 1;
    }

    return ans;
}
int main( int argc, char* argv[] ) {
    //Checking for right number of agruments
    if ( argc != 3 ) {
        fprintf(stderr, "Expected %s <file name> <number of processes>\n", argv[0]);
        exit(1);
    } 

    FILE* tempIn;
    FILE* tempOut;
    FILE* input;
    int numProc;
    int x, y;
    int start = 0, end;
    int slice, remainder;
    pid_t pid;

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

    slice = numberPoints/numProc;
    remainder = numberPoints % numProc;

    //Create Pipe
    
    pipe(fd);
   
    //Fork and thread
    
    start = 0 - slice;
    end = 0;
    
    for( int i = 0; i < numProc; i++ ) {
        start = end;
        end += slice;

        if ( remainder > 0 ) {
            end++;
            remainder--;
        }

        pid = fork();
    }

    //Figure out how to solve the question.
    int counter = 0;
    int temp;;

    if ( pid == 0 ) {
        close(fd[1]);

        for ( int i = start; i < end; i++ ) {
            for ( int j = i + 1; j < numberPoints ; j++ ) {
                for ( int k = j + 1; k < numberPoints; k++) {
                    if ( 1 == checkPoints( points[i][0], points[j][0], points[k][0], 
                                        points[i][1], points[j][1], points[k][1]) ) {
                        counter++;
                    }
                }
            }
        }

        tempIn = fdopen(fd[0], "w");

        fprintf(tempIn, "%d", counter );

        fclose(tempIn);

        close(fd[0]);
    } else {
        close(fd[0]);
        tempOut = fdopen(fd[1], "r");

        for ( int i = 0; i < numProc; i ++ ) {
            fscanf(tempOut, "%d", &temp);
            counter += temp;
        }

        fprintf(stdout, "%d\n", counter);

        fclose(tempOut);
        close(fd[1]);
    }

    return 0;
}
