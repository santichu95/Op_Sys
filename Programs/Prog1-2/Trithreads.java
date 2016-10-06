/*	Santiago Andaluz Ruiz
 *  Prog 2 Trithreads
 */

import java.util.List;
import java.util.Scanner;
import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;


public class Trithreads {
	private int x;
	private int y;
	
	public Trithreads(int x, int y) {
		this.x = x;
		this.y = y;
	}
	
	public int getX() {
		return x;
	}
	
	public int getY() {
		return y;
	}
	
	public static void main(String[] args) throws FileNotFoundException, InterruptedException{
		Scanner input = new Scanner(new File(args[0]));
		List<Trithreads> points = new ArrayList<Trithreads>();
		
		Integer numberPoints = input.nextInt();
		
		List<Integer> answer = new ArrayList<Integer>();
		
		for( int i = 0; i < numberPoints; i++ ) {
			answer.add(-1);
		}
		
		for (int i = 0; i < numberPoints; i++){
			points.add(new Trithreads(input.nextInt(), input.nextInt()));
		}
		
		Integer numberProc = Integer.parseInt(args[1]);
		int slice = numberPoints / numberProc;
		int remainder = numberPoints % numberProc;
		int tempEnd = 0;
		for (int i = 0; i < numberProc; i++){
			
			
			final int start = tempEnd;
			
			if( remainder > 0 ) {
				tempEnd++;
				remainder--;
			}
			tempEnd += slice;
			
			final int end = tempEnd;
			
			new Thread(new Runnable(){
				@Override
				public void run(){
					int numberRight = 0;
					for ( int i = start; i < end; i++ ) {
				        for ( int j = i + 1; j < numberPoints - 1; j++ ) {
				            for ( int k = j + 1; k < numberPoints; k++ ) {
				                numberRight += checkTriangle(points.get(i), points.get(j), points.get(k));
				            }
				        }

			            answer.set(i, numberRight);
			            numberRight = 0;
				    }
				}
			}).start();
		}
		
		/*
		for( Trithreads var : points ) {
			System.out.println(var.getX() + " " + var.getY());
		}
		*/
		int counter = 0;
		for ( int i = 0; i < numberPoints; i++ ) {
			while(answer.get(i) == -1){
				Thread.sleep(100);
			}
			counter += answer.get(i);
		}
		
		System.out.println(counter);
		
		input.close();
	}
	
	public static int checkTriangle(Trithreads first, Trithreads second, Trithreads third) {
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

	    a = Math.sqrt(a);

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

	    b = Math.sqrt(b);

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

	    c = Math.sqrt(c);

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

	    
	    a *= a;
	    b *= b;
	    c *= c;
	    
	    if ( Math.abs( c - ( a + b ) ) < 0.0000001 ) {
	        return 1;
	    } else {
	        return 0;
	    }
	}
}
