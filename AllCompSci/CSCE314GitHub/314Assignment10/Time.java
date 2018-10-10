import java.util.*;
import java.util.concurrent.locks.*;

public class Time{
	int timing;
	
	private Condition divise;
	private Lock messageLock;
	
	public Time(){
		this.messageLock = new ReentrantLock();
		this.divise = messageLock.newCondition();
		this.timing = 0;
	}
	
	public int increment(Time j){
		j.messageLock.lock();
		try{
			timing++;
			divise.signalAll();
			return timing;
		}
		finally{
			j.messageLock.unlock();
			System.out.println(timing + " ");
		}
	}
	
	public void checkCondition(Time j, int intv){
		j.messageLock.lock();
		
		try{
			while((j.timing % intv) != 0){
				divise.await();
			}
			if((j.timing % intv) == 0){
				System.out.println();
				System.out.println(intv + " second messages");
			}
		}
		catch(InterruptedException e){
			System.out.println(" ");
		}
		finally{
			j.messageLock.unlock();
		}
	}
}