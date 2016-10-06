/**
 * Santiago Andaluz Ruiz 
 * Program 0
 * Pythagorian theorum might be a better option?
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <math.h>

/*A structure to keep track of the x and y coordinates of each point
 *
 */
struct Point {
    int x;
    int y;
};

int checkTriangle(struct Point, struct Point, struct Point );

int main( int argc, char* argv[] ) {

    //Checking for right number of agruments
    if ( argc != 3 ) {
        fprintf(stderr, 
                "Expected %s <file name> <number of processes>\n",
                argv[0]);
        exit(-1);
    } 

    FILE* input;
    int numProc;
    int x, y;

    //Check if the files exists
    input = fopen(argv[1], "r");

    if( input == NULL ) {
        perror("File not found");
        fclose(input);
        exit(-1);
    }

    //Check if the number of programs is valid
    // 0 < x < 101
    numProc = atoi(argv[2]);
    
    if( numProc < 0 || numProc > 101 ) {
        fprintf( stderr, 
            "Number of processes was not in the acceptable bounds\n");
    }

    //Read in the points
    int numberPoints;
    
    fscanf(input, "%d", &numberPoints);

    //Create an array for all the points
    struct Point points[numberPoints];

    int returnValue;

    for ( int i = 0; i < numberPoints; i++ ) {
        returnValue = fscanf(input, "%d %d", &x, &y);
        if ( returnValue == EOF ) {
            fprintf(stderr, "Data file did not contain enough points\n");
            fclose(input);
            exit(-1);
        }
        points[i].x = x;
        points[i].y = y;
    }

    //Close the file
    fclose(input);

    //Create pipe
    int fd[2];
    
    pipe(fd);
    pid_t pid;
   
    int slice, remainder;
    slice = numberPoints/numProc;
    remainder = numberPoints % numProc;

    int start = 0;
    int end = 0;

    //Fork
    for( int i = 0; i < numProc; i++ ) {
        //Set the start and stop of the points 
        //that the child process needs to examine
        start = end;
        end += slice;

        if ( remainder > 0 ) {
            remainder--;
            end++;
        }

        pid = fork();

        if ( pid == 0 ) {
           break;
        }
    }

    int numberRight = 0;
    int temp, counter = 0;

    //Child process
    if ( pid == 0 ) {
        //Close reading end of pipe
        close(fd[0]);
        
        //Checking points
        for ( int i = start; i < end; i++ ) {
            for ( int j = i + 1; j < numberPoints - 1; j++ ) {
                for ( int k = j + 1; k < numberPoints; k++ ) {
                    numberRight += checkTriangle(points[i], points[j], points[k]);
                }
            }
        }

        //Send answer to parent
        write(fd[1], &numberRight, sizeof(int) );

        //Close writing end of pipe
        close(fd[1]);
        
        exit(0);
    } else { //Parent
        //Close writing end of pipe
        close(fd[1]);

        //Read the answer from each of the children
        for ( int i = 0; i < numProc; i++) {
            read(fd[0], &temp, sizeof(int) );
            counter += temp;
        }

        //Close pipe
        close(fd[0]);
    }

    //Output answer
    fprintf(stdout, "%d\n", counter);

    return 0;
}

/* checkTriangle()
 *
 * Checks if a set of three points creates a right triangle 
 * around any of the three points.
 *
 * /param first the first point to be examined
 * /param second the second point to be examined
 * /param third the third point to be examined
 * /return 1 if a right triangle is found
 *         0 if a right triangle is not found
 */
int checkTriangle(struct Point first, struct Point second, struct Point third) {
    double a, b, c, temp;

    double x1, x2, y1, y2;

    double xComp, yComp;


    //Side from first to second
    x1 = first.x;
    x2 = second.x;

    y1 = first.y;
    y2 = second.y;

    xComp = x2 - x1;
    xComp *= xComp;

    yComp = y2 - y1;
    yComp *= yComp;

    a = xComp + yComp;

    a = sqrt(a);

    //Side from first to third
    x1 = first.x;
    x2 = third.x;

    y1 = first.y;
    y2 = third.y;

    xComp = x2 - x1;
    xComp *= xComp;

    yComp = y2 - y1;
    yComp *= yComp;

    b = xComp + yComp;

    b = sqrt(b);

    //Side from second to third
    x1 = second.x;
    x2 = third.x;

    y1 = second.y;
    y2 = third.y;

    xComp = x2 - x1;
    xComp *= xComp;

    yComp = y2 - y1;
    yComp *= yComp;

    c = xComp + yComp;

    c = sqrt(c);

    //Makes sure the greatest number is in c
    if ( a > b && a > c ) {
        temp = a;
        a = c;
        c = temp;
    } else if ( b > a && b > c ) { 
        temp = b;
        b = c;
        c = temp;
    }

    //Finding a^2, b^2, c^2
    a *= a;
    b *= b;
    c *= c;
    
    return ( fabs ( c - ( a + b ) ) < 0.0000001 );
}
