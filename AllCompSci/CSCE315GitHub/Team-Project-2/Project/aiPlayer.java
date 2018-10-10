import boardPackage.Board;
import java.util.ArrayList;
import boardPackage.EMPTY_BOARD_EXCEPTION;
import boardPackage.INVALID_PIT_NUMBER;
import boardPackage.PICK_FROM_EMPTY_PIT;

public class aiPlayer{
	
    int moveNumber;
    ArrayList<Thread> threads = new ArrayList<Thread>();
    Object mutex = new Object();

    public class MaxThread extends Thread{
        Move move;
        Board board;
        int currentDepth;
        int maxDepth;
        int alpha;
        int beta;
        Object mutex;
		int timeout;
        
        MaxThread(Move move,Board board, int currentDepth, int maxDepth, int alpha, int beta, Object mutex, int time) {
            this.move = move;
            this.board = board;
            this.mutex = mutex;
			this.timeout = time;
            this.maxDepth = maxDepth;
            this.alpha = alpha;
            this.beta = beta;
            this.currentDepth = currentDepth;
        }

        public void run(){
            try{
                move.setBin((maxValue(board, currentDepth, maxDepth, alpha, beta)).getBin());
            	move.setValue((maxValue(board, currentDepth, maxDepth, alpha, beta)).getValue());
            }
            catch(Exception e){
                System.out.println(e.toString());
            }
        }
        public Move getMove() {
        	return move;
        }
    }

    public class MinThread extends Thread{
        Move move;
        Board board;
        int currentDepth;
        int maxDepth;
        int alpha;
        int beta;
        Object mutex;
		int timeout;
        
        MinThread(Move move,Board board, int currentDepth, int maxDepth, int alpha, int beta, Object mutex, int time) {
            this.move = move;
            this.board = board;
            this.mutex = mutex;
			this.timeout = time;
            this.maxDepth = maxDepth;
            this.alpha = alpha;
            this.beta = beta;
            this.currentDepth = currentDepth;
        }

        public void run(){
            try{
            	move.setBin((minValue(board, currentDepth, maxDepth, alpha, beta)).getBin());
                move.setValue((minValue(board, currentDepth, maxDepth, alpha, beta)).getValue());
            }
            catch(Exception e){
                System.out.println(e.toString());
            }
        }
        
        public Move getMove() {
        	return move;
        }
    }

	public int utility(Board board){
    	
        int utilNum = (board.getPits().get(board.houses) - (board.getPits().get(board.houses * 2 -1)));
        for(int i = 0; i < board.houses; i++) {
        	utilNum -= board.getPits().get(i + board.houses);
        	utilNum += board.getPits().get(i);	
        }
        return utilNum;
    }

    public void makeMove(Board board, int difficulty) throws PICK_FROM_EMPTY_PIT, INVALID_PIT_NUMBER, EMPTY_BOARD_EXCEPTION{
        int depth = 1;

        while(depth <= difficulty){
            
            try{
                    Move finalMove = maxValue(board, 0, depth, -10000, 10000);
                    this.moveNumber = finalMove.getBin();
                    
            }
            catch(Exception e){
                e.printStackTrace();
            }
            
            depth++;
        }
    }

    public Move maxValue(Board board, int currentDepth, int maxDepth, int alpha, int beta) throws PICK_FROM_EMPTY_PIT, INVALID_PIT_NUMBER, EMPTY_BOARD_EXCEPTION{
            Move move = new Move(-10001, 1);
            if(board.ended() || currentDepth == maxDepth){
                move.setBin(14);
                move.setValue(utility(board));
                return move;
            }

            int v = -10000;
            for(int i = board.houses + 1; i <= board.houses*2; i++){
            	Board temp = new Board(board);
                if(temp.getPits().get(i)!=0){
                    int oldV = v;
                    if(temp.pick(i)){
                        try{
                                MinThread thread = new MinThread(move, temp, currentDepth + 1, maxDepth, alpha, beta, mutex, 10000);
                                thread.start();
                                thread.join();
                                threads.add(thread);
                                v = Math.max(v, move.getValue());
                        }
                        catch(Exception e){
                            e.printStackTrace();
                        }

                    }
                    else{
                        try{
                                MaxThread thread = new MaxThread(move, temp, currentDepth + 1, maxDepth, alpha, beta, mutex, 10000);
                                thread.start();
                                thread.join();
                                threads.add(thread);
                                v = Math.max(v, move.getValue());
                        }

                        catch(Exception e){
                            e.printStackTrace();
                        }
                    }

                    if(oldV < v){
                        move.setValue(v);
                        move.setBin(i);
                    }

                    alpha = Math.max(v, alpha);

                    if(v > beta){
                        return move;
                    }
      

                }
            }
            return move;
    }

    public Move minValue(Board board, int currentDepth, int maxDepth, int alpha, int beta) throws PICK_FROM_EMPTY_PIT, INVALID_PIT_NUMBER, EMPTY_BOARD_EXCEPTION{
        Move move = new Move(10001, 1);
        if(board.ended() || currentDepth == maxDepth){
            move.setBin(14);
            move.setValue(utility(board));
            return move;
        }

        int v = 10000;
        for(int i = 0; i <= board.houses-1; i++){
        	Board temp = new Board(board);
            if(temp.getPits().get(i)!=0){
                int oldV = v;
                if(temp.pick(i)){
                    try{
                            MaxThread thread = new MaxThread(move, temp, currentDepth + 1, maxDepth, alpha, beta, mutex, 10000);
                            thread.start();
                            thread.join();
                            threads.add(thread);
                            v = Math.min(v, move.getValue());
                    }
                    catch(Exception e){
                        e.printStackTrace();
                    }
                    
                }
                else{
                    try{
                            MinThread thread = new MinThread(move, temp, currentDepth + 1, maxDepth, alpha, beta, mutex, 10000);
                            thread.start();
                            thread.join();
                            threads.add(thread);
                            v = Math.min(v, move.getValue());
                    }
                    catch(Exception e){
                        e.printStackTrace();
                    }
                }

                if(oldV > v){
                    move.setValue(v);
                    move.setBin(i);
                }

                alpha = Math.min(v, beta);

                if(v < alpha){
                    return move;
                }
            }
        }
        return move;
    }
    

}
