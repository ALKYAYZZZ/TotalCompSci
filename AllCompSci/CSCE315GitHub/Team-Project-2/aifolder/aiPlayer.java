package aifolder;

import aifolder.Move;
public class aiPlayer extends Player{

    public aiPlayer(){

    }

    public int utility(int[] state){
        int utilNum = ((state[13] - state[6]) - (state[0] + state[1] + state[2] + state[3] + state[4] + state[5]) + (state[7] + state[8] + state[9] + state[10] + state[11] + state [12]));
        return utilNum;
    }

    public void thinkMove(MyState stateOfGame){
        int depth = 1;

        while(true){
            Move finalMove = maxValue(stateOfGame, 0, depth, -10000, 10000);
            this.moveNumber = finalMove.getBin();
            //System.out.println("Iteration " + depth + " Move/Value: " + finalMove.getBin() + " " + finalMove.getValue());
            depth++;
        }
    }

    public Move maxValue(MyState stateOfGame, int currentDepth, int maxDepth, int alpha, int beta){
            Move move = new Move(-10001, 1);
            if(stateOfGame.gameStatus() != 2 || currentDepth == maxDepth){
                move.setBin(13);
                move.setValue(utility(stateOfGame.state));
                return move;
            }

            int v = -10000;
            for(int i = 13; i >= 8; i--){
                //System.out.println(i);
                if(stateOfGame.notIllegalMove(i) == true){
                    //System.out.println("Hello");
                    MyState temp = new MyState();
                    temp.state = temp.makeMove(temp.state, i, 2);
                    int oldV = v;
                    if(stateOfGame.extraMove(i) == false){
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

    public Move minValue(MyState stateOfGame, int currentDepth, int maxDepth, int alpha, int beta){
        Move move = new Move(10001, 1);
        if(stateOfGame.gameStatus() != 2 || currentDepth == maxDepth){
            move.setBin(13);
            move.setValue(utility(stateOfGame.state));
            return move;
        }

        int v = 10000;
        for(int i = 6; i >= 1; i--){
            //System.out.println(i);
            if(stateOfGame.notIllegalMove(i) == true){
                MyState temp = new MyState();
                temp.state = temp.makeMove(temp.state, i, 2);
                int oldV = v;
                if(stateOfGame.extraMove(i) == false){
                    v = Math.min(v, (maxValue(temp, currentDepth + 1, maxDepth, alpha, beta).getValue()));
                }
                else{
                    v = Math.min(v, (minValue(temp, currentDepth + 1, maxDepth, alpha, beta).getValue()));
                }

                if(oldV > v){
                    move.setValue(v);
                    move.setBin(i);
                }

                beta = Math.min(v, beta);

                if(v < alpha){
                    return move;
                }
            }
        }
    return move;
    }
}