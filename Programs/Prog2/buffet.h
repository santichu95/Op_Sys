#ifndef BUFFET_H
#define BUFFET_H

#include <vector>
#include "semaphore.h"
using namespace std;

/**
 * Type of pizza slice
 */
enum SliceType {
  Veggie,
  Cheese,
  Meat,
  Works,
};

/**
 * Class to manage pizza buffet
 * \version 1.0
 */
class Buffet
{
  private:
      int maxSlices;
      vector<SliceType> food;
      bool bufOpen;
      int vegWait;
      sem_t buf;
      sem_t vegCounter;
	  
  public:
    /**
     * Construct buffet for maximum given pizza slides
     * \param maxSlices max slices on buffet (non-negative)
     */
    Buffet(const int maxSlices = 20);

    /**
     * Free any buffet resources
     */
    ~Buffet();
  
    /**
     * Called by non-vegetarian patrons to request a desired number of
     * slides and return a list of slices, oldest slice first. This
     * method will block callers until they can have all the
     * desired slices; it won't take any slices from the buffet until
     * they can get everything they want. If a vegetarian is waiting
     * for slices, this will only take non-vegetarian slices (slices
     * other than cheese and veggie). Otherwise, this method will
     * give them any type of slice. If the buffet is closed, this
     * method will return null.
     * \param desired number of desired slices
     * \return list of slices or null if buffet closed
     */
    vector<SliceType> TakeAny(const int desired);

    /**
     * This method is like takeAny(), but it returns the oldest
     * vegetarian slices in the slice list. As with takeAny(), 
     * it will block the caller until they can have all
     * the slices they want.  Once the buffet is closed, this method
     * will return null immediately.
     * \param desired number of desired slices
     * \return list of slices or null if buffet closed
     */
    vector<SliceType> TakeVeg(const int desired);

    /**
     * This method adds count slices, all of the given slice type,
     * stype, to the buffet. The buffet only has a finite capacity of
     * slices (given to constructor). If there isn't enough room for
     * all the slices, it will add as many as it can to the buffet and
     * then block the calling thread until there's room for more.
     * (Note that this is different from the behavior of takeAny() an
     * takeVeg(), where you don't take any slices until you can have
     * all of them.) This method immediately returns false when the
     * buffet is closed.
     * \param count number of slices to add to buffet
     * \param stype type of slices to add to buffet
     * \return true if slices were added or false if buffet closed
     */
    bool AddPizza(const int count, const SliceType stype);

    /**
     * This function marks the buffet as closed, causing threads
     * blocked in takeAny(), takeVeg() and addPizza() to immediately
     * return false (and threads subsequently calling these function
     * to
     * immediately return false).
     */
    void close();
};
 
#endif
