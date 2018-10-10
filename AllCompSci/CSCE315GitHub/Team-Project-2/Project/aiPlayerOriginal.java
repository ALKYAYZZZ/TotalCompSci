import boardPackage.Board;
import boardPackage.EMPTY_BOARD_EXCEPTION;
import boardPackage.INVALID_PIT_NUMBER;
import boardPackage.PICK_FROM_EMPTY_PIT;

public class aiPlayer{
	
    int moveNumber;

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
            Move finalMove = maxValue(board, 0, depth, -10000, 10000);
            this.moveNumber = finalMove.getBin();
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
                    if(!temp.pick(i)){
                        v = Math.max(v, (minValue(temp, currentDepth + 1, maxDepth, alpha, beta).getValue()));
                    }
                    else{
                        v = Math.max(v, (maxValue(temp, currentDepth + 1, maxDepth, alpha, beta).getValue()));
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
                if(!temp.pick(i)){
                    v = Math.min(v, (maxValue(temp, currentDepth + 1, maxDepth, alpha, beta).getValue()));
                }
                else{
                    v = Math.min(v, (minValue(temp, currentDepth + 1, maxDepth, alpha, beta).getValue()));
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