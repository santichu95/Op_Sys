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
    srand(time(NULL));

	pthread_t meat;
	pthread_t veg;
	pthread_t server;

	pthread_create(&meat, NULL, patronFunction, (void *)buf);
	pthread_create(&server, NULL, serverFunction, (void *)buf);
	pthread_create(&veg, NULL, patronFunction, (void *)buf);

	buf->close();
    delete buf;
    return 0;
}


void *serverFunction( void * param ) {
    SliceType pizza[] = {Meat, Veggie, Works, Cheese};
    int type,
        count;
    Buffet *buff = ((Buffet *)param);
    while( true ) {
        type = rand() % 4 + 1;
        count = rand() % 10 + 1;
        if ( !(buff->AddPizza( count, pizza[type] )) ) {
            break;
        }
    }

    pthread_exit(nullptr);
}

void *patronFunction( void * param ) {
    int type,
        count;
    Buffet *buff = ((Buffet *)param);
    while ( true ) {
        type = rand() % 2 + 1;
        count = rand()% 10 + 1;
        if ( type == 1 ) {
            buff->TakeAny(count);
        } else if ( type == 2 ) {
            buff->TakeAny(count);
        }
    }

    pthread_exit(nullptr);
}

