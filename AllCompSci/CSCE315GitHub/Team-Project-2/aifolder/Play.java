package aifolder;

import java.util.Scanner;
import java.io.*;
import aifolder.Player;

public class Play{
    Player player1 = new Player();
    Player player2 = new aiPlayer();
    String player1Name = "Player 1";
    int player1Num = 1;
    String player2Name = "Player 2";
    int player2Num = 2;
   
    

    public Play(){
        this.player1.initializePlayer(this.player1Name, 1);
        this.player2.initializePlayer(this.player2Name, 2);
        
    }

    public static class TimedThread extends Thread{
        MyState state;
        Player player;
        Object mutex;
        int timeout;
        TimedThread(MyState state, Player player, Object mutex, int time){
            this.state = state;
            this.player = player;
            this.mutex = mutex;
            this.timeout = time;
        }

        public void run(){
            try{
                ((aiPlayer)player).thinkMove(state);
            }
            catch(Exception e){
                System.out.println(e.toString());
            }
        }
    }



    public static void main (String[] args){
        Scanner sc = new Scanner(System.in);
        MyState stateOfGame = new MyState();
        Play game = new Play();
        boolean keepPlaying = true;
        boolean turnAgain = true;;
        game.player2 = ((aiPlayer)(game.player2));
        while(keepPlaying == true){
            while(turnAgain == true){
                System.out.println("Please type the number of your next Bin move.");
                int nextMove = sc.nextInt();
                turnAgain = stateOfGame.extraMove(nextMove);
                stateOfGame.state = stateOfGame.makeMove(stateOfGame.state, nextMove, 1);
                System.out.println("Move Made.");
                System.out.println("Board So Far");
                for(int i = 0; i < stateOfGame.state.length; i++){
                    System.out.println(stateOfGame.state[i]);
                }

                
            }
            turnAgain = true;
            while(turnAgain == true){
                Object mutex = new Object();
                try{
                    synchronized(mutex){
                        TimedThread thread = new TimedThread(stateOfGame,((aiPlayer)game.player2),mutex, 100000);
                        thread.start();
                        
                        mutex.wait(10000);
                        
                        thread.sleep(500);
                        if(thread.isAlive()){
                            thread.stop();
                        }
                    }
                }
                catch(Exception e){
                    e.printStackTrace();
                }

                System.out.println(game.player2.moveNumber);
                turnAgain = stateOfGame.extraMove(game.player2.moveNumber);
                stateOfGame.state = stateOfGame.makeMove(stateOfGame.state, (game.player2.moveNumber), 2);
                System.out.println("Board So Far");
                for(int i = 0; i < stateOfGame.state.length; i++){
                    System.out.println(stateOfGame.state[i]);
                }
            }
            System.out.println("Keep Playing?");
            keepPlaying = sc.nextBoolean();
        }
    }
} 