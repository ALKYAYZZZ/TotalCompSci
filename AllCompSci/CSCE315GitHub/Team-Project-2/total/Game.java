import boardPackage.Board;
import boardPackage.EMPTY_BOARD_EXCEPTION;
import boardPackage.INVALID_PIT_NUMBER;
import boardPackage.PICK_FROM_EMPTY_PIT;

public class Game {
	Board board;
	boolean turn;
	boolean AIflag = false;
	boolean swapped = false;
	aiPlayer ai = null;
	boolean guiupdated = false;
	boolean ended = false;
	
	public Game(Board _board, boolean _turn) {
		board = _board;
		turn = _turn;
	}
	
	public Game(Board _board, boolean _turn, boolean _AIflag) {
		board = _board;
		turn = _turn;
		if(_AIflag == true) {
			AIflag = true;
			ai = new aiPlayer();
		}
	}

	public int getHouses() {
		return board.houses;
	}

	public int getPit(int index) {
		return board.getPits().get(index);
	}
	
	public void pick(int i) throws PICK_FROM_EMPTY_PIT, INVALID_PIT_NUMBER, InterruptedException, EMPTY_BOARD_EXCEPTION {
		boolean endedInStore = board.pick(i);
		if (endedInStore == false) {
			turn = !turn;
		}
		ended();
	}
	
	public void swap() {
		
	}
	
	public void ended() throws EMPTY_BOARD_EXCEPTION {
		ended = board.ended();
	}
	
	public void AImove() throws PICK_FROM_EMPTY_PIT, INVALID_PIT_NUMBER, InterruptedException, EMPTY_BOARD_EXCEPTION {
	
        Object mutex = new Object();
        try{
            synchronized(mutex){
                TimedThread thread = new TimedThread(board, ai, mutex, 10000);
                thread.start();
                
                mutex.wait(3000);
                
                thread.sleep(500);
                if(thread.isAlive()){
                    thread.stop();
                }
            }
        }
        catch(Exception e){
            e.printStackTrace();
        }
        
        turn = !board.pick(ai.moveNumber);
        ended();
	}
	
	public class TimedThread extends Thread{
        Board board;
        aiPlayer ai;
        Object mutex;
        int timeout;
        
        TimedThread(Board board, aiPlayer ai, Object mutex, int time) {
            this.board = board;
            this.ai = ai;
            this.mutex = mutex;
            this.timeout = time;
        }

        public void run(){
            try{
                ai.makeMove(board);
            }
            catch(Exception e){
                System.out.println(e.toString());
            }
        }
    }
	
}
