import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.Semaphore;

/**
 * @author andaluzruiz
 *
 */
public class BuffetSemaphore implements Buffet {
	private int maxSlices;
	private List<SliceType> food;
	private boolean bufOpen;
	private int vegWait;
	private final Semaphore buf;
	private final Semaphore vegCounter;
	  
	public BuffetSemaphore(final int maxSlice) {
	    bufOpen = true;
	    vegWait = 0;
	    maxSlices = maxSlice;
	    food = new ArrayList<SliceType>();
	    buf = new Semaphore(1, true);
	    vegCounter = new Semaphore(1, true);
	}
	
	@Override
	public List<SliceType> TakeAny(int desired) {
	    int found;
	    int index;
	    List<SliceType> taken = new ArrayList<SliceType>();
	    List<Integer> sliceIndex = new ArrayList<Integer>();
	    
	    if ( desired < 0 || desired > maxSlices ) {
	    	throw new IllegalArgumentException("Desired was outside of bounds, expected 0 < desired < maxSlices");
	    }
	    
	    if ( desired == 0 ) {
	    	return taken;
	    }

	    //While buffet is open and there is a possibility for the right 
	    //number of slices to be taken
	    while ( bufOpen ) {
	        index = 0;
	        found = 0;
	        sliceIndex.clear();

	        //Wait sema
	        try {
				buf.acquire();
//				System.out.println("Acquire TakeAny");
			} catch (InterruptedException e) {
				return null;
			}
	        
	        //Checks that the buff is still open
	        if ( !bufOpen ) {
		        return null;
	        }

	        //Checks if the buff has enough slices to take
	        if (food.size() >= desired && food.size() > 0 ) {
	            //Checks that the buff is still open
	            if ( !bufOpen ) {
	    	        return null;
	            }
	            
	            //If a veg is not waiting check if there is enough pizza to take
	            if ( vegWait < 1 ) {
	                if ( food.size() >= desired ) {
	                	for ( int i = 0; i < desired; i++ ) {
	                		taken.add(food.get(0));
		            		//remove the slice from the buffet
			            	food.remove(0);
	                	}
	                }
	            } else { //else looks slice by slice for meat or works
		            for ( SliceType var : food ) {
		                if ( found == desired ) {
		                    break;
		                }
	
		                if ( !bufOpen ) {
		        	        return null;
		                }
	
		                //Checks for cheese or veg slice
		                if ( !var.isVeg() ) {
		                    //keeps track of slice index
		                    sliceIndex.add(index);
	
		                    found++;
		                }
	
		                //point to the next slice
		                index++;
		            }
	
		            //If found enough non-veg pizza and veg waiting
		            if ( found == desired ) {
		            	
		            	while( !sliceIndex.isEmpty() ) {
		            		//add the slice to taken
		            		taken.add(food.get(sliceIndex.get(0)));
		            		
		            		//remove the slice from the buffet
			            	food.remove(sliceIndex.get(0));
			            	
			            	//remove the slice just taken from the slices to take
			            	sliceIndex.remove(0);
		            	}
		            }
	            }

	        }

//	        System.out.println("Release TakeAny");
	        //Signals that it is done with the buffer
	        buf.release();
	    }

	    if ( bufOpen ) {
	        return taken;
	    } else { 
	        return null;
	    }

	}

	@Override
	public List<SliceType> TakeVeg(int desired) {  
		
		if ( desired < 0 || desired > maxSlices ) {
	    	throw new IllegalArgumentException("Desired was outside of bounds, expected 0 < desired < maxSlices");
	    }

	    List<SliceType> taken = new ArrayList<SliceType>();
	    
	    if ( desired == 0 ) {
	    	return taken;
	    }
		
		int found = 0;
	    int index;
	    List<Integer> sliceIndex = new ArrayList<Integer>();
	
	    //Notifies all the thread that one more vegetarian is waiting
	    try {
			vegCounter.acquire();
		} catch (InterruptedException e) {
			return null;
		}
	    vegWait++;
	    vegCounter.release();
	
	    //While the buffet is open, the number of slices found is not the number
	    //desired and while there is a possibility to grab enough slices
	    while ( bufOpen ) {
	        index = 0;
	        found = 0;
	        sliceIndex.clear();
	
	        //Wait sema
	        try {
				buf.acquire();
//	        	System.out.println("Aquire Veg");
			} catch (InterruptedException e) {
				return null;
			}
	
	        //Checks that the buff is still open
	        if ( !bufOpen ) {
	            return null; 
	        }
	
	        //Checks if the buff has enough slices to take
	        if (food.size() >= desired && food.size() > 0 ) {
	            //Checks that the buff is still open
	            if ( !bufOpen ) {
		            return null; 
	            }
	
	            //Looks slice by slice for cheese or veggie
	            for ( SliceType var : food ) {
	                if ( found == desired ) {
	                    break;
	                }
	
	                if ( !bufOpen ) {
	    	            return null; 
	                }
	
	                //Checks for cheese or veg slice
	                if ( var.isVeg() ) {
	                    //keeps track of slice index
	                    sliceIndex.add(index);
	
	                    found++;
	                }
	
	                //point to the next slice
	                index++;
	            }
	
	            if ( found == desired ) {
	            	
	            	while( !sliceIndex.isEmpty() ) {
	            		//add the slice to taken
	            		taken.add(food.get(sliceIndex.get(0)));
	            		
	            		//remove the slice from the buffet
		            	food.remove(sliceIndex.get(0));
		            	
		            	//remove the slice just taken from the slices to take
		            	sliceIndex.remove(0);
	            	}
	            }
	
	        }

//        	System.out.println("Release Veg");
	        //Signals that it is done with the buffer
	        buf.release();
	    }
	
	    //Notifies all the thread that one less vegetarian is waiting
	    try {
			vegCounter.acquire();
		} catch (InterruptedException e) {
			return null;
		}
	    vegWait++;
	    vegCounter.release();
	    
	    
	    if ( bufOpen ) {
	        return taken;
	    } else { 
            return null; 
	    }
	}

	@Override
	public boolean AddPizza(int count, SliceType stype) {
		if ( count < 0 ) {
	    	throw new IllegalArgumentException("count was outside of bounds, expected count > 0");
	    }
		if ( stype == null ) {
			throw new NullPointerException("expected stype to be non-null");
		}
		int added = 0;

	    while ( added < count ) {
	        //wait on sema
	        try {
				buf.acquire();
			} catch (InterruptedException e) {
				//Thread interrupted exit immediately
				return false;
			}
	        if ( !bufOpen ) {
	            return false;
	        }
	
	        //Add pizza to vector
	
	        //Checks to see if there is room for the pizza
	        while ( food.size() < maxSlices && added < count) {
	
	            //Checks if the buffet is open 
	            if ( !bufOpen ) {
	                //buffet closed return false
	                return false;
	            } else { 
	                //Adds the slice to the buff
	                food.add(stype);
	                added++;
	            }
	        }
	
	        //Signal sema
	        buf.release();
	    }
	
	    return true;
	}

	@Override
	public void close() {//No more slices can be added
	    maxSlices = 0;
	    bufOpen = false;
	    
	    //Waits on buff
	    try {
			buf.acquire();
		} catch (InterruptedException e) {
			System.err.println("Close interrupted");
		}
	    
	    //Clears food, holding the pizza
	    food.clear();

	    //posts buf
	    buf.release();


	    //While someone is using the semaphore busy wait
	    //Any remaining threads should enter CS and immediately get kicked out
	    //due to the maxSlice being 0 and the vector being empty.
	    while(buf.hasQueuedThreads());
	}

}
