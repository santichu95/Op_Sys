/**
 * Santiago Andaluz Ruiz 
 * Program 0
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

struct Point {
    int x;
    int y;
};

int checkTriangle(struct Point, struct Point, struct Point );

int checkPoints(struct Point, struct Point, struct Point );

int main( int argc, char* argv[] ) {

    //Checking for right number of agruments
    if ( argc != 3 ) {
        fprintf(stderr, "Expected %s <file name> <number of processes>\n", argv[0]);
        exit(1);
    } 

    FILE* input;
    int numProc;
    int x, y;

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
            "Number of processes was not in the acceptable bounds\n");
    }

    //Read in the points
    int numberPoints;
    
    fscanf(input, "%d", &numberPoints);

    /* fprintf(stdout, "%d\n", numberPoints); */

   
    //int points[numberPoints][2];

    struct Point points[numberPoints];

    for ( int i = 0; i < numberPoints; i++ ) {
        fscanf(input, "%d %d", &x, &y);
        points[i].x = x;
        points[i].y = y;
    }

    /*
    for ( int i = 0; i < numberPoints; i++ ) {
        x = points[i].x;
        y = points[i].y;

        fprintf(stdout, "%d %d\n", x, y);
    }
    */

    /*
    int slice, remainder;
    slice = numberPoints/numProc;
    remainder = numberPoints % numProc;
    */

    //Create Pipe
    
    pipe(fd);
   
    //Fork and thread
    
    /*
    for( int i = 0; i < numProc; i++ ) {
    }
    */

    int numberRight = 0;

    for ( int i = 0; i < numberPoints - 2; i++ ) {
        for ( int j = i + 1; j < numberPoints - 1; j++ ) {
            for ( int k = k + 1; k < numberPoints; k++ ) {
                numberRight += checkTriangle(points[i], points[j], points[k]);
                //fprintf(stdout, "%d\n", numberRight);
            }
        }
    }

    fprintf(stdout, "%d\n", numberRight);

    //Figure out how to solve the question.
    return 0;
}

/*
 * Checks if a set of three points creates a right triangle around any of the three
 * points.
 */
int checkTriangle(struct Point first, struct Point second, struct Point third) {
    int result = 0;

    result = checkPoints(first, second, third);

    if ( result == 0 ) {
        result += checkPoints(second, first, third);
    }
    if ( result == 0 ) { 
        result += checkPoints(third, first, second);
    }

    return result;
}

/*
 *Checks if a set of three points create a right triangle centered around Point first
 */
int checkPoints(struct Point first, struct Point second, struct Point third) {
    int  ans = 1;

    int  vertOne = 0, vertTwo = 0;

    int  rise, run; 
    double  slopeOne, slopeTwo;

    rise = second.y - first.y;
    run = second.x - first.x;

    if( run == 0 ) {
        vertOne = 1;
    } else if (rise == 0 ) {
        slopeOne = 0;
    } else {
        slopeOne = rise/run;
    }
    
    rise = third.y - first.y;
    run = third.x - first.x;

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

/*
int checkPoints(int xOne, int xTwo, int xThree, int yOne, int yTwo, int yThree) {
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
*/
