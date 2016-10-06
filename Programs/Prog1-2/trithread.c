/**
 * Santiago Andaluz Ruiz 
 * Program 1 Trithreads
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <math.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/shm.h>


struct Point {
    int x;
    int y;
};

void *childFunction( void * );

int checkTriangle(struct Point, struct Point, struct Point );

int main( int argc, char* argv[] ) {

    //Checking for right number of agruments
    if ( argc != 3 ) {
        fprintf(stderr, "Expected %s <file name> <number of processes>\n", argv[0]);
        exit(1);
    } 

    FILE* input;
    int numProc;

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

    //Check return
    fscanf(input, "%d", &numberPoints);

    if ( numberPoints < numProc ) {
        numProc = numberPoints;
    }

    struct Point *points = malloc(sizeof(struct Point) * numberPoints);


    for ( int i = 0; i < numberPoints; i++ ) {
        if ( fscanf(input, "%d %d", &(points[i].x), &(points[i].y)) == EOF ) {
            fprintf(stderr, "Data file did not contain enough points\n");
            fclose(input);
            exit(-1);
        }
    }

    fclose(input);

    //Fork and thread
    pthread_t tid[numProc];

    int startPoint = 0;
    int size = (numberPoints / numProc) + 1;
    int remainder = numberPoints % numProc;
    int answer[numProc];
    
    //Spawning threads
    for( int i = 0; i < numProc; i++ ) {
        if ( remainder > 0 ) {
            remainder--;
        } else if ( remainder == 0 ) {
            size--;
            remainder--;
        }

        void *param = malloc((sizeof(int) * 4) + sizeof(int *) + sizeof(struct Point*));

        ((int *)param)[0] = size;
        ((int *)param)[1] = startPoint;
        ((int *)param)[2] = numberPoints;
        ((long *)param)[2] = &answer[i];
        ((long *)param)[3] = points;

        startPoint += size;

        pthread_create(&tid[i], NULL, childFunction, param);
    }

    int counter = 0;

    //Waiting for answers
    for( int i = 0; i < numProc; i++ ) {
        pthread_join(tid[i], NULL);
        counter += answer[i];
    }

    fprintf(stdout, "%d\n", counter);

    //Clean up
    return 0;
}

void *childFunction( void * void_ptr ) {
    int end = ((int *) void_ptr)[0];
    int start = ((int *) void_ptr)[1];
    int numberPoints = ((int *) void_ptr)[2];
    int *answer = ((long *) void_ptr)[2];
    end += start;

    int numberRight = 0;
    struct Point *points;
    points = ((long *)void_ptr)[3];
    

    //Checking all the points that this thread is suppose to check
    for ( int i = start; i < end; i++ ) {
        for ( int j = i + 1; j < numberPoints - 1; j++ ) {
            int countRights = 0;
            for ( int k = j + 1; k < numberPoints; k++ ) {
                if ( countRights == 2 ) {
                    break;
                }
                countRights += checkTriangle(points[i], points[j], points[k]);
            }
            numberRight += countRights;
        }
    }

    *answer = numberRight;

    free(void_ptr);

    pthread_exit(0);
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
    x2 = third.x;

    y2 = third.y;

    xComp = x2 - x1;
    xComp *= xComp;

    yComp = y2 - y1;
    yComp *= yComp;

    b = xComp + yComp;

    b = sqrt(b);

    //Side from second to third
    x1 = second.x;

    y1 = second.y;

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

    a *= a;
    b *= b;
    c *= c;
    
    return ( fabs ( c - ( a + b ) ) < 0.0000001 );
}
