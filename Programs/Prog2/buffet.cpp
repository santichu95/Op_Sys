#include "buffet.h"
#include <fcntl.h>
#include <semaphore.h>
#include <climits>
#include <algorithm>

Buffet::Buffet( const int maxSlice ) {
    bufOpen = true;
    vegWait = 0;
    maxSlices = maxSlice;
    vector<SliceType> food = vector<SliceType>();
    sem_init(&buf, 0, 1);
    sem_init(&vegCounter, 0, 1);
}

Buffet::~Buffet() {
    //ensure the buff is closed
    close();

    //Destroy semaphore
    sem_destroy(&buf);
    sem_destroy(&vegCounter);
}

vector<SliceType> Buffet::TakeAny(const int desired) {
    int found = 0;
    int index;
    vector<SliceType> taken;
    vector<int> sliceIndex;

    //While buffet is open and there is a possibility for the right 
    //number of slices to be taken
    while ( bufOpen && found < desired 
            && taken.size() < ((unsigned int)desired) ) {
        index = 0;

        //Wait sema
        sem_wait(&buf);

        //Checks that the buff is still open
        if ( !bufOpen ) {
            return {}; 
        }

        //Checks if the buff has enough slices to take
        if (food.size() >= ((unsigned int)desired) && food.size() > 0 ) {
            //Checks that the buff is still open
            if ( !bufOpen ) {
                return {};
            }

            //Looks slice by slice for cheese or veggie
            for ( SliceType var : food ) {
                if ( found == desired ) {
                    break;
                }

                if ( !bufOpen ) {
                    return {};
                }

                //Checks for cheese or veg slice
                if ( var == Meat || var == Works ) {
                    //keeps track of slice index
                    sliceIndex.push_back(index);

                    found++;
                }

                //point to the next slice
                index++;
            }

            //If not veg waiting and there is enough total slices
            if ( vegWait == 0 ) {
                if ( food.size() >= (unsigned int) desired ) {
                //Take first n slices
                }
            }
            //If found enough non-veg pizza and veg waiting
            else if ( found == desired ) {
                int offset = food.size() - 1;
                int index;
                vector<SliceType>::iterator it = food.end();
                it--;

                while ( !sliceIndex.empty() ) {
                    index = sliceIndex.back();

                    sliceIndex.pop_back();

                    it -= ( offset - index );

                    offset = index;

                    taken.push_back(*it);

                    food.erase(it);
                }

                reverse(taken.begin(), taken.end());

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

    if ( bufOpen ) {
        return taken;
    } else { 
        return {};
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

    //While the buffet is open, the number of slices found is not the number
    //desired and while there is a possibility to grab enough slices
    while ( bufOpen && found < desired 
            && taken.size() < ((unsigned int)desired) ) {
        index = 0;

        //Wait sema
        sem_wait(&buf);

        //Checks that the buff is still open
        if ( !bufOpen ) {
            return {}; 
        }

        //Checks if the buff has enough slices to take
        if (food.size() >= ((unsigned int)desired) && food.size() > 0 ) {
            //Checks that the buff is still open
            if ( !bufOpen ) {
                return {};
            }

            //Looks slice by slice for cheese or veggie
            for ( SliceType var : food ) {
                if ( found == desired ) {
                    break;
                }

                if ( !bufOpen ) {
                    return {};
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
                it--;

                while ( !sliceIndex.empty() ) {
                    index = sliceIndex.back();

                    sliceIndex.pop_back();

                    it -= ( offset - index );

                    offset = index;

                    taken.push_back(*it);

                    food.erase(it);
                }

                reverse(taken.begin(), taken.end());

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

    if ( bufOpen ) {
        return taken;
    } else { 
        return {};
    }
}

bool Buffet::AddPizza(const int count, const SliceType stype) {
    int added = 0;

    while ( added < count ) {
        //wait on sema
        sem_wait(&buf);
        if ( !bufOpen ) {
            return false;
        }

        //Add pizza to vector

        //Checks to see if there is room for the pizza
        while ( food.size() < (unsigned int)maxSlices && added < count) {

            //Checks if the buffet is open 
            if ( !bufOpen ) {
                //buffet closed return false
                return false;
            } else { 
                //Adds the slice to the buff
                food.push_back(stype);
				added++;
            }
        }

        //Signal sema
        sem_post(&buf);
    }

    return true;
}

void Buffet::close(){
    //No more slices can be added
    maxSlices = 0;
    bufOpen = false;
    
    //Waits on buff
    sem_wait(&buf);
    
    //Clears food, holding the pizza
    food.clear();

    //posts buf
    sem_post(&buf);

    //keeps track of the value of buf
    int counter = INT_MAX;

    //While someone is using the semaphore busy wait
    //Any remaining threads should enter CS and immediately get kicked out
    //due to the maxSlice being 0 and the vector being empty.
    while ( counter != 1 ) {
        sem_getvalue(&buf, &counter);
    }
}
