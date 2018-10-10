import java.util.*;

public class TimePrinter implements Runnable{
	private Time count;
	
	public TimePrinter(Time j){
		this.count = j;
		new Thread(this).start();
	}
	
	@Override
	public void run(){
		while(true){
			try{
				count.increment(count);
				Thread.sleep(1000);
			}
			catch(Exception e){}
		}
	}
}