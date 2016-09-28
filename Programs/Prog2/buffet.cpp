#include "buffet.h"
#include <fnctl.h>
#include <semaphore.h>
#include <climits>

Buffet::Buffet( const int maxSlice ) {
    open = true;
    vegWait = 0;
    maxSlices = maxSlice;
    vector<SliceType> food = vector<SliceType>();
    sem_init(&buf, 0, 1);
    sem_init(&vegCounter, 0, 1);
}

Buffet::~Buffet() {
    sem_destroy(&buf);
}

vector<SliceType> Buffet::TakeAny(const int desired) {
    int found = 0;
    vector<SliceType> taken;
    vector<int> locPrio;

    while ( open && taken.size() < desired ) {

        //Wait sema
        sem_wait(&buf);

        //Checks that the buff is still open
        if ( !open ) {
            return null; 
        }

        //Checks if the buff has enough slices to take
        if ( (unsigned int)food.size >= desired && food.size() > 0 ) {
            //Checks that the buff is still open
            it = food.begin();

            //Looks slice by slice for meat or works pizza
            for ( SliceType var : food ) {
                if ( !open ) {
                    return null;
                }
                //If the slice is meats or works remove it from the buf
                //add it to the taken 
                if ( var == Meat || var == Works ) {
                    //Removes pizza
                    it = food.erase(it);

                    //dec it in order for it to point to the element before
                    //the one that was removed
                    it--;

                    //Adds the slices into the vector containing the taken slice
                    taken.push_back(var);
                }

                //point to the next slice
                it++;
            }

            it = food.begin();
            while ( food.size() > 0 && ( unsigned int ) taken.size() < desired ) {
                //Checks that the buff is still open
                if ( !open ) {
                    return null;
                }

                //Add to taken slices
                taken.push_back(*it);

                //removes the first slice,
                //it will point to the following slice
                it = food.erase(it);
            }
        }
        //Signals that it is done with the buffer
        sem_post(&buf);
    }
    if ( open ) {
        return taken;
    } else { 
        return null;
    }
}

vector<SliceType> Buffet::TakeVeg(const int desired) {
    int found = 0;
    int index;
    vector<SliceType> taken;
    vector<int> sliceIndex;

    //Notifies all the thread that a vegitarian is waiting
    sem_wait(&vegCounter);
    vegWait++;
    sem_post(&vegCounter);

    while ( open && taken.size() < desired ) {
        index = 0;

        //Wait sema
        sem_wait(&buf);

        //Checks that the buff is still open
        if ( !open ) {
            return null; 
        }

        //Checks if the buff has enough slices to take
        if ( (unsigned int)food.size >= desired && food.size() > 0 ) {
            //Checks that the buff is still open
            if ( !open ) {
                return null;
            }

            //Looks slice by slice for cheese or veggie
            for ( SliceType var : food ) {
                if ( found == desired ) {
                    break;
                }

                if ( !open ) {
                    return null;
                }

                //Checks for cheese or veg slice
                if ( var == Cheese || var == Veggie ) {
                    //keeps track of slice index
                    sliceIndex.push_back(index);

                    found++;
                }

                //point to the next slice
                index++;
            }

            if ( found == desired ) {
                int offset = food.size() - 1;
                int index;

                vector<SliceType>::iterator it = food.end();

                while ( !sliceIndex.empty() ) {
                    index = sliceIndex.back();

                    sliceIndex.pop_back();

                    it -= ( offset - index );

                    offset = index;
                    food.erase(it);
                }

            }

        }

        //Signals that it is done with the buffer
        sem_post(&buf);
        
        //Did not grab all the slices we wanted

        /*
        //give up control 
        if ( taken.size() != desired ) {
            this_thread::yield();
        }
        */
    }

    if ( open ) {
        return taken;
    } else { 
        return null;
    }
}

bool AddPizza(const int count, const SliceType stype) {
    int added = 0;
    //wait on sema
    sem_wait(&buf);

    //Add pizza to vector
    while ( food.size() < maxSlices && added < count ) {

        //Checks if the buffet is open 
        if ( !open ) {
            return false;
        } else { 
            //Adds the slice to the buff
            food.push_back(stype);
        }
    }

    //Signal sema
    sem_post(&buf);

    if ( added == count ) {
        return true;
    }

    return false;
}

void close(){
    //No more slices can be added
    maxSlices = 0;
    open = false;
    
    //Waits on buff
    sem_wait(&buf);
    
    //Clears food, holding the pizza
    food.clear();

    //posts buf
    sem_post(&buf);

    //keeps track of the value of buf
    int counter = INT_MAX;

    //While someone is using the semaphore busy wait
    //Any remaining threads should enter CS and imeddiatly get kicked out
    //due to the maxSlice being 0 and the vector being empty.
    while ( counter != 1 ) {
        sem_getvalue(&buf, &counter);
    }
}
