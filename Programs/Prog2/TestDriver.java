import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.ThreadLocalRandom;

public class TestDriver {

	private final static int MAX = 10;
	private final static int MIN = 0;
	private final static int MINSLICES = 0;
	private final static int MAXSLICES = 4;
	private final static int NUMMEAT = 2;
	private final static int NUMVEG = 2;
	private static Buffet buff;
	
	public static void main(String args[]) throws InterruptedException {
		buff = new BuffetSemaphore(20);
		
		List<Thread> threads = new ArrayList<Thread>();
		
		//Add four consumers
		for ( int i = 0; i < NUMMEAT; i++ ) {
			threads.add( new Thread(new Runnable(){
				@Override
				public void run(){
					randMeat();
				}
			}) );
			threads.get(i).start();
		}
		
		//Add one producer
		threads.add(new Thread(new Runnable(){
			@Override
			public void run(){
				randomAdd();
			}
		}) );
		threads.get(threads.size() - 1).start();
	}
	
	public static void randMeat() {
		List<SliceType> ret = null;
		do {
			int randomNum = ThreadLocalRandom.current().nextInt(MIN, MAX + 1);
			ret = buff.TakeAny(randomNum);
		} while ( ret != null );
		
	}
	
	public static void randVeg() {
		List<SliceType> ret = null;
		do {
			int randomNum = ThreadLocalRandom.current().nextInt(MIN, MAX + 1);
			ret = buff.TakeVeg(randomNum);
		} while ( ret != null );
		
	}
	
	public static void randomAdd() {
		boolean ret = true;
		while ( ret ) {
			int randomNum = ThreadLocalRandom.current().nextInt(MIN, MAX + 1);
			
			int randomSlice = ThreadLocalRandom.current().nextInt(MINSLICES, MAXSLICES);
			SliceType slice = null;
			int counter = 0;
			
			for ( SliceType var : SliceType.values() ) {
				if ( counter == randomSlice) {
					slice = var;
					break;
				}
				counter++;
			}
			
			ret = buff.AddPizza(randomNum, slice);
		}
		
	}
}
