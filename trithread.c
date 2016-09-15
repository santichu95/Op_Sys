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

struct Point *points;
int numberPoints;

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
    int x, y;


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
    
    fscanf(input, "%d", &numberPoints);

    points = malloc(sizeof(struct Point) * numberPoints);

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

    //Fork and thread

    int shmid;
    key_t key = 7421;
    int* shm;
    size_t shmSize = numberPoints * sizeof(int);
    
    //shmid = shmget(key, shmSize ,IPC_CREAT | IPC_EXCL | 0660  );
    shmid = shmget(key, shmSize , IPC_CREAT | 0660 );

    if ( shmid == -1 ) {
        perror("Could not allocate memory");
        exit(-1);
    }


    if ( (shm = (int *) shmat(shmid, NULL, 0)) == (int *) -1 ) {
        perror("Could not attach memory\n");
        exit(-1);
    }

    //Getting the shared memory ready
    for ( int i = 0; i < numberPoints; i++ ) {
        shm[i] = -1;
    }

    pthread_t tid[numProc];

    int firstPoint, secondPoint = 0;
    int slice = numberPoints / numProc;
    int remainder = numberPoints % numProc;
    
    //Spawning threads
    for( int i = 0; i < numProc; i++ ) {
        firstPoint = secondPoint;
        secondPoint += slice;
        if ( remainder > 0 ) {
            remainder--;
            secondPoint++;
        }

        void *param = malloc(sizeof(int) * 2);

        ((int *)param)[0] = firstPoint;

        ((int *)param)[1] = secondPoint;

        pthread_create(&tid[0], NULL, childFunction, param);
    }

    int counter = 0;

    //Waiting for answers
    for( int i = 0; i < numberPoints; i++ ) {
        while ( shm[i] == -1 );
        counter += shm[i];
    }


    fprintf(stdout, "%d\n", counter);

    //Clean up
    shmdt(shm);
    shmctl(shmid, IPC_RMID, NULL);
    free(points);

    return 0;
}

void *childFunction( void * void_ptr ) {

    int *int_ptr = (int *) void_ptr;

    int start = int_ptr[0];
    int end = int_ptr[1];

    /*
    fprintf(stdout, "%d %d\n", start, end);
    fprintf(stdout, "%d %d\n", points[start].x,  points[start].y);
    */

    int numberRight = 0;
    
    key_t key = 7421;
    size_t size = numberPoints * sizeof(int);
    int shmid = shmget(key, size , 0660 );
    if ( shmid == -1 ) {
        perror("Could not located shared memory");
        exit(-1);
    }

    int *answers = (int *) shmat(shmid, NULL, 0);

    if ( answers == (int *)-1) {
        perror("Could not attach memory child");
        exit(-1);
    }

    for ( int i = start; i < end; i++ ) {
        for ( int j = i + 1; j < numberPoints - 1; j++ ) {
            for ( int k = j + 1; k < numberPoints; k++ ) {
                numberRight += checkTriangle(points[i], points[j], points[k]);
            }
        }
        answers[i] = numberRight;
        numberRight = 0;
    }

    shmdt(answers);

    free(void_ptr);

    return NULL;
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
