#include <iostream>
#include "buffet.h"
#include "pthread.h"
#include <iterator>
#include <vector>

using namespace std;

void *serverFunction( void * );

void *patronFunction( void * );

int main() {
    Buffet *buf = new Buffet();
    buf->AddPizza(5, Cheese);
    buf->close();
    delete buf;

    return 0;
}


void *serverFunction( void * param ) {
    SliceType pizza[] = {Meat, Veggie, Works, Cheese};
    int type,
        count,
        max = (*(int *)param);
    Buffet *buff = ((Buffet *)param) + 4;
    while( true ) {
        type = random();
        count = max;
        //rand the type
        //rand the number 1 - 2*size
        //add pizza to buff
        if ( !(buff->AddPizza( count, pizza[type] )) ) {
            break;
        }
    }

    return nullptr;
}

void *patronFunction( void * param ) {
    int type,
        count,
        max = (*(int *)param);
    Buffet *buff = ((Buffet *)param) + 4;
    while ( true ) {
        type = random();
        count = max;
        //random veg or meat
        //random number of slices 1 - size/3 
        if ( type == 0 ) {
            buff->TakeAny(count);
        }
    }

    return nullptr;
}

