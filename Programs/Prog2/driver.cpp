#include <iostream>
//#include "buffet.h"
//#include "pthread.h"
#include <iterator>
#include <vector>

using namespace std;

/*
void *serverFunction( void * );

void *patronFunction( void * );
*/

int main() {
    //Buffet buf(10);
    vector<int> food(0);
    vector<int> sliceIndex(0);

    for ( int i = 0; i < 10; i++ ) {
        food.push_back(i);
    }

    sliceIndex.push_back(8);

    cout << "BREAK\n";
    cout << food.size();
    for ( int var : food ) {
        cout << var << " ";
    }

    int offset = food.size() - 1;
    int index;
    vector<int>::iterator it;

    while ( !sliceIndex.empty() ) {
        index = sliceIndex.back();

        sliceIndex.pop_back();

        it -= ( offset - index );

        offset = index;
        food.erase(it);
    }

    for ( int var : food ) {
        cout << var << " ";
    }

    return 0;
}
