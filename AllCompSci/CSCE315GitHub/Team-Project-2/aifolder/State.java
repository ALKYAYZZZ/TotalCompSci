package aifolder;

public class MyState{
	
	public int[] state =  { 4, 4, 4, 4, 4, 4, 0, 4, 4, 4, 4, 4, 4, 0 };

	MyState(){ }

public int gameStatus(){
	if(countStonesByPlayer(1) != 0 && countStonesByPlayer(2) != 0){
		return 2;
	}
	else if((countStonesByPlayer(1) + farBinStones(1)) > (countStonesByPlayer(2) + farBinStones(2))){
		return 1;
	}
	else if((countStonesByPlayer(1) + farBinStones(1)) < (countStonesByPlayer(2) + farBinStones(2))){
		return -1;
	}
	else{
		return 0;
	}
}

public boolean extraMove(int bin){
	int seeds = this.state[bin - 1];
	if(((bin + seeds) % 14 == 7) || ((bin + seeds) % 14 == 0)){
		return true;
	}
	else{
		return false;
	}
}

public int countStonesByPlayer(int playerNum){
	int total = 0;
	if(playerNum == 1){
		for(int i = 0; i < 6; i++){
			total = total + this.state[i];
		}
	}
	else{
		for(int i = 7; i < 13; i++){
			total = total + this.state[i];
		}

	}
	return total;
}

public int farBinStones(int playerNum){
	if(playerNum == 1){
		return this.state[6];
	}
	else{
		return this.state[13];
	}
}

public int count(int bin){
	if(bin != 0){
		return this.state[bin - 1];
	}
	else{
		return this.state[13];
	}
}

public int[] makeMove(int[] state, int bin, int player){
	boolean moveCheck = notIllegalMove(bin - 1);
	if(moveCheck){
		int count = bin - 1;
		for(int i = count(bin); i > 0; i--){
			count = count % 14;
			state[count] = state[count] + 1;
			count++;
		}
		
		//If last seed lands in an empty pit, add seeds to players main pit
		if(count != 1 || count != 14){
			if((count - 1) > 7 && state[(count - 1)] == 1){
				state[13] = state[13] + state[count - 1] + state[count - 8];
			}
			else if((count - 1) <= 7 && state[(count - 1)] == 1){
				state[6] = state[6] + state[count - 1] + state[count + 6];
			}
		}
			return state;
		
	}
	else{
		//System.out.println("Illegal Move Try Again");
		return state;
	}

}


//PLAYER 1 IS ALWAYS BOTTOM OF BOARD, PLAYER 2 IS TOP OF BOARD
public boolean notIllegalMove(int bin){
	if(state[bin] == 0 || bin == 6 || bin > 13){
		return false;
	}
	else{
		return true;
	}
}
}