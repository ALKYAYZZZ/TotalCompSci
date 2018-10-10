package aifolder;

import aifolder.MyState;
public class Player{
    String screenName;
    int playerNumber;
    int moveNumber;

    Player(){
        screenName = "";
        playerNumber = 0;
        moveNumber = 0;
    }

    public void initializePlayer(String name, int playerNum){
        this.screenName = name;
        this.playerNumber = playerNum;
    }

    public void move(){
        
    }
}

