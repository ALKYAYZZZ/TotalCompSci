import java.util.Timer;
import java.util.TimerTask;

public class Main2{
	public static void main(String[] args){
		Time counter = new Time();
		TimePrinter timed = new TimePrinter(counter);
		
		PrintMessenger jst7 = new PrintMessenger(counter, 7);
		PrintMessenger jst15 = new PrintMessenger(counter, 15);
	}
	
}