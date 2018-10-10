import java.util.*;
import java.util.Timer;
import java.util.TimerTask;

public class PrintMessenger implements Runnable{
	private int seconds = 0;
	Time timed = new Time();
	
	public PrintMessenger(Time j, int timer){
		this.timed = j;
		this.seconds = timer;
		new Thread(this).start();
	}
	
	@Override
	public void run(){
		while(true){
			try{
				Thread.sleep(1000);
				timed.checkCondition(timed, seconds);
			}
			catch(Exception e){
				
			}
		}
	}
}