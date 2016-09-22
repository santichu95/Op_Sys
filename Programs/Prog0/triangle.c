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

struct Point {
    int x;
    int y;
};

int checkTriangle(struct Point, struct Point, struct Point );

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
        perror("File not found");
        fclose(input);
        exit(-1);
    }

    //Check if the number of programs is valid
    //i.e > 0
    numProc = atoi(argv[2]);
    
    if( numProc < 0 || numProc > 101 ) {
        fprintf( stderr, 
            "Number of processes was not in the acceptable bounds\n");
    }

    //Read in the points
    int numberPoints;
    
    fscanf(input, "%d", &numberPoints);

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

    fclose(input);

    //Create Pipe
    
    pipe(fd);
    pid_t pid;
   
    int slice, remainder;
    slice = numberPoints/numProc;
    remainder = numberPoints % numProc;

    int start = 0;
    int end = 0;

    //Fork and thread
    
    for( int i = 0; i < numProc; i++ ) {
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

    if ( pid == 0 ) {
        close(fd[0]);
        
        //fprintf(stdout, "%d %d\n", start, end);
        
        for ( int i = start; i < end; i++ ) {
            for ( int j = i + 1; j < numberPoints - 1; j++ ) {
                for ( int k = j + 1; k < numberPoints; k++ ) {
                    numberRight += checkTriangle(points[i], points[j], points[k]);
                }
            }
        }

        write(fd[1], &numberRight, sizeof(int) );
        close(fd[1]);
        
        exit(0);
    } else {
        close(fd[1]);
        for ( int i = 0; i < numProc; i++) {
            read(fd[0], &temp, sizeof(int) );
            counter += temp;
        }
        close(fd[0]);

    }


    fprintf(stdout, "%d\n", counter);

    //Figure out how to solve the question.
    return 0;
}

/*
 * Checks if a set of three points creates a right triangle around any of the three
 * points.
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

    //fprintf(stdout, "%f %f %f\n", a, b, c);
    //fprintf(stdout, "%d %d %d %d %d %d a: %d b: %d c: %d\n", first.x, first.y, second.x, second.y, third.x, third.y, a, b, c);
        
    a *= a;
    b *= b;
    c *= c;
    
    if ( fabs ( c - ( a + b ) ) < 0.0000001 ) {
        return 1;
    } else {
        return 0;
    }
}
