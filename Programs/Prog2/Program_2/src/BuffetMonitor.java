import java.util.ArrayList;
import java.util.List;

/**A controller for the buffet interface using java monitors.
 * @author Santiago Andaluz Ruiz
 *
 */
public class BuffetMonitor implements Buffet {

	private int maxSlices;
	private List<SliceType> food;
	private boolean bufOpen;
	private Integer vegWait;
	  
	public BuffetMonitor(final int maxSlice) {
	    bufOpen = true;
	    vegWait = 0;
	    this.maxSlices = maxSlice;
	    food = new ArrayList<SliceType>();
	}
	
	/**Increments the vegetarian counter
	 * 
	 */
	public void incVeg() {
		synchronized( vegWait ) {
			vegWait++;
		}
	}
	
	/**Decrements the vegetarian counter
	 * 
	 */
	public void decVeg() {
		synchronized( vegWait ) {
			vegWait--;
		}
	}
	
	/**Checks to see if there is a vegetarian waiting
	 * @return true if there is a vegetarian waiting
	 */
	public boolean vegWaiting(){
		synchronized( vegWait ) {
			return vegWait > 0;
		}
	}
	
	@Override
	public synchronized List<SliceType> TakeAny(int desired) {
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
	    boolean first = false;
	    
	    while ( bufOpen && taken.size() == 0 ) {
	        index = 0;
	        found = 0;
	        sliceIndex.clear();

	        if ( first ) {
	        	try {
					wait();
				} catch (InterruptedException e) {
					return null;
				}
	        } else {
	        	first = true;
	        }
	        
	        while ( bufOpen && food.size() < desired ) {
	        	try {
					wait();
				} catch (InterruptedException e) {
					return null;
				}
	        }

	        //Checks that the buff is still open
	        if ( !bufOpen ) {
		        return null;
	        }

	        //Checks if the buff has enough slices to take
	        if (food.size() >= desired ) {
	            //Checks that the buff is still open
	            if ( !bufOpen ) {
	    	        return null;
	            }
	            
	            //If a veg is not waiting check if there is enough pizza to take
	            if ( !vegWaiting() ) {
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
	        
	        notifyAll();
	        if ( taken.size() == 0 ) {
	        	try {
					wait();
				} catch (InterruptedException e) {
					return null;
				}
	        }
	    }

	    if ( bufOpen ) {
	        return taken;
	    } else { 
	        return null;
	    }

	}

	@Override
	public synchronized List<SliceType> TakeVeg(int desired) {  

	    List<SliceType> taken = new ArrayList<SliceType>();
		List<Integer> sliceIndex = new ArrayList<Integer>();
		int index,
			found;
		if ( desired < 0 || desired > maxSlices ) {
	    	throw new IllegalArgumentException("Desired was outside of bounds, expected 0 < desired < maxSlices");
	    }
		
		if ( desired == 0 ) {
	    	return taken;
	    }
		
		boolean first = false;
	    //Notifies all the thread that one more vegetarian is waiting
	    incVeg();
	
	    //While the buffet is open, the number of slices found is not the number
	    //desired and while there is a possibility to grab enough slices
	    while ( bufOpen ) {
	        index = 0;
	        found = 0;

	        if ( first ) {
	        	try {
					wait();
				} catch (InterruptedException e) {
					return null;
				}
	        } else {
	        	first = true;
	        }
	        
	
	        //Waits for the buffet to be open and have enough slices for the
	        //chance that there is enough of the appropriate slices to take
	        while ( bufOpen && food.size() < desired ) {
	        	try {
					wait();
				} catch (InterruptedException e) {
					return null;
				}
	        }
	        
	        
	        //Checks that the buff is still open
	        if ( !bufOpen ) {
	            return null; 
	        }
	
	        //Checks if the buff has enough slices to take
	        if ( food.size() > desired ) {
	            //Checks that the buff is still open
	            if ( !bufOpen ) {
		            return null; 
	            }
	
	            //Looks slice by slice for cheese or veggie
	            for ( SliceType var : food ) {
	                if ( found == desired ) {
	                    break;
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
	    	            //Checks that the buff is still open
	    	            if ( !bufOpen ) {
	    		            return null; 
	    	            }
	    	            
	            		//add the slice to taken
	            		taken.add(food.get(sliceIndex.get(0)));
	            		
	            		//remove the slice from the buffet
		            	food.remove(sliceIndex.get(0));
		            	
		            	//remove the slice just taken from the slices to take
		            	sliceIndex.remove(0);
	            	}
	            }
	        }
		    //Notify any waiting threads that there has been a change to the buffet.
		    notifyAll();
	    }
	
	    //One less vegetarian waiting
	    decVeg();
	    
	    
	    if ( bufOpen ) {
	        return taken;
	    } else { 
            return null; 
	    }
	}

	@Override
	public synchronized boolean AddPizza(int count, SliceType stype) {
		
		if ( count < 0 ) {
	    	throw new IllegalArgumentException("count was outside of bounds, expected count > 0");
	    }
		
		if ( stype == null ) {
			throw new NullPointerException("expected stype to be non-null");
		}
		int added = 0;

	    while ( added < count ) {
	    	//Waiting for the buffet to not be full
	    	while ( food.size() == maxSlices ) {
	    		try {
					wait();
				} catch (InterruptedException e) {
					return false;
				}
	    	}
	    	
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
	        
	        //Notify any waiting thread that pizza has been added.
	        notifyAll();
	    }
	
	    return true;
	}

	@Override
	public synchronized void close() {//No more slices can be added
	    maxSlices = 0;
	    
	    bufOpen = false;
	    
	    //Clears food, holding the pizza
	    food.clear();
	    
	    notifyAll();
	}

}
