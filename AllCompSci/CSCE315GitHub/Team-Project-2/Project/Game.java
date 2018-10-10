import java.util.Vector;

import boardPackage.Board;
import boardPackage.EMPTY_BOARD_EXCEPTION;
import boardPackage.INVALID_PIT_NUMBER;
import boardPackage.PICK_FROM_EMPTY_PIT;
import serverClient.KalahClient;
import serverClient.KalahServer;

public class Game {
	Board board;
	boolean turn;
	boolean AIflag = false;
	boolean clientFlag = false;
	boolean serverFlag = false;
	boolean swapped = false;
	aiPlayer ai = null;
	KalahClient client = new KalahClient();
	KalahServer server = new KalahServer();
	boolean guiupdated = false;
	boolean ended = false;
	int turnNumber = 0;
	int difficulty = 1;
	//Vector<Integer> movesToCommunicate;
	
	//Two player game constructor
	public Game(Board _board, boolean _turn) {
		board = _board;
		turn = _turn;
		AIflag = false;
	}
	
	//One player game constructor
	public Game(Board _board, boolean _turn, boolean _AIflag, int _difficulty) {
		board = _board;
		turn = _turn;
		if(_AIflag == true) {
			AIflag = true;
			ai = new aiPlayer();
		}
		difficulty = _difficulty;
	}
	
	//Client game constructor
	public Game(Board _board, boolean _AIflag, int _difficulty, String _hostName, Integer portNumber) {
		//movesToCommunicate = new Vector<>();
		board = _board;
		if(_AIflag == true) {
			AIflag = true;
			ai = new aiPlayer();
		}
		difficulty = _difficulty;
		client.setupClient(!_AIflag, _hostName, portNumber);
		
		clientFlag = true;
		
		if (client.playerNum == 1)
		{
			Boolean receivedFirstMove = false;
			while (!receivedFirstMove)
			{
				String cmd = client.waitForCommand();
				
				Integer commandType = client.handleCommand(cmd);
				if (commandType == 1)
				{
					receivedFirstMove = true;
				}
				else if (commandType == -1)
				{
					client.sendCommand("ILLEGAL");
					// TODO end game here
				}
			}
		}
	}
	
	//Server game constructor
	public Game(Integer pitsPerSide, Integer seedsPerHole, Boolean randomStart, Long timeLimit, Integer playerNum, boolean _AIflag, int _difficulty, boolean _multiClient, Integer portNum) {
		//movesToCommunicate = new Vector<>();
		if(_AIflag == true) {
			AIflag = true;
			ai = new aiPlayer();
		}
		difficulty = _difficulty;
		server.setupServer(!AIflag, _multiClient, portNum);
		serverFlag = true;
		server.acceptConnections();
		server.createBoard(pitsPerSide, seedsPerHole, randomStart, timeLimit, playerNum);
		if (playerNum == 1)
		{
			Boolean receivedFirstMove = false;
			Boolean illegalCommand = false;
			while (!receivedFirstMove && !illegalCommand)
			{
				long startTime = System.currentTimeMillis();
				String cmd = server.waitForCommand(server.turn);
				long timeToReceive = System.currentTimeMillis() - startTime;
				Boolean tookTooLong = timeToReceive > server.timeLimit;
				
				if (!server.legalInput(cmd, 50))
				{
					server.sendCommand("ILLEGAL",0);
					server.sendCommand("LOSER", 0);
					illegalCommand = true;
					// TODO end game here
					break;
				}
				
				Integer commandType = server.handleCommand(cmd);
				if (commandType == 1)
				{
					receivedFirstMove = true;
					server.sendAck(0);
				}
				else if (commandType == -1)
				{
					server.sendCommand("ILLEGAL",0);
					server.sendCommand("LOSER", 0);
					illegalCommand = true;	
					// TODO end game here
					break;
				}
				else if (tookTooLong)
				{
					server.sendCommand("TIME", 0);
					server.sendCommand("LOSER", 0);
					illegalCommand = true;
					// TODO end game here
					break;
				}
			}
		}
	}
	
	// Idle server constructor
	public Game(Integer pitsPerSide, Integer seedsPerHole, Boolean randomStart, Long timeLimit, int _difficulty, boolean _multiClient, Integer portNum) {
		server.setupServer(!AIflag, _multiClient, portNum);
		server.acceptConnections();
		server.createBoard(pitsPerSide, seedsPerHole, randomStart, timeLimit, 2);
		server.launchServer();
	}
	

	public int getHouses() {
		return board.houses;
	}

	public int getPit(int index) {
		return board.getPits().get(index);
	}
	
	public void pick(int i) throws PICK_FROM_EMPTY_PIT, INVALID_PIT_NUMBER, InterruptedException, EMPTY_BOARD_EXCEPTION {
		//movesToCommunicate.add(i);
		boolean endedInStore = board.pick(i);
		if (endedInStore == false) {
			turn = !turn;
			turnNumber++;
			String movesToSend = "";
			//for (int j = 0; j < movesToCommunicate.size(); j++)
			{
				//movesToSend.concat(movesToCommunicate.elementAt(j).toString());
			//	if (j != (movesToCommunicate.size() - 1)) movesToSend.concat(" ");
			}
			//movesToCommunicate = new Vector<>();
			
			if(clientFlag) {
				client.sendCommand(movesToSend);
				
				Boolean doneReceiving = false;
				while (!doneReceiving)
				{
					String cmd = client.waitForCommand();
					
					Integer commandType = client.handleCommand(cmd);
					if (commandType == 1)
					{
						doneReceiving = true;
					}
					if (commandType == 2)
					{
						Integer protocool = client.handleAck(KalahServer.tokenize(cmd));
						if (protocool == 2)
						{
							String result = client.waitForCommand();
							
							// this stores winner in the client
							client.handleAck(KalahServer.tokenize(result));
						}
						if (protocool != 0)
						{
							// TODO endGame
							doneReceiving = true;
						}
					}
					else if (commandType == -1)
					{
						client.sendCommand("ILLEGAL");
						// TODO end game here
					}
				}
			}
			if(serverFlag) {
				server.sendCommand(movesToSend, 0);
				
				Boolean doneReceiving = false;
				Boolean illegalCommand = false;
				
				while (!doneReceiving)
				{
					long startTime = System.currentTimeMillis();
					String cmd = server.waitForCommand(server.turn);
					long timeToReceive = System.currentTimeMillis() - startTime;
					Boolean tookTooLong = timeToReceive > server.timeLimit;
					
					if (!server.legalInput(cmd, 50))
					{
						server.sendCommand("ILLEGAL",0);
						server.sendCommand("LOSER", 0);
						illegalCommand = true;
						// TODO end game here
						break;
					}
					
					Integer commandType = server.handleCommand(cmd);
					if (commandType == 1)
					{
						doneReceiving = true;
						server.sendAck(0);
					}
					else if (commandType == 2)
					{
						Integer protocool = server.handleAck(KalahServer.tokenize(cmd));
						if (protocool == 2)
						{
							String result = server.waitForCommand(0);
							
							server.handleAck(server.tokenize(result));
						}
						if (protocool != 0)
						{
							// TODO end game
							doneReceiving = true;
						}
					}
					else if (commandType == -1)
					{
						server.sendCommand("ILLEGAL",0);
						server.sendCommand("LOSER", 0);
						illegalCommand = true;	
						// TODO end game here
						break;
					}
					else if (tookTooLong)
					{
						server.sendCommand("TIME", 0);
						server.sendCommand("LOSER", 0);
						illegalCommand = true;
						// TODO end game here
						break;
					}
				}
			}
		}
		
		
		ended();
		
		
	}
	
	public void swap() {
		board.swapSides();
		turn = !turn;
		turnNumber++;
	}
	
	public void ended() throws EMPTY_BOARD_EXCEPTION {
		ended = board.ended();
		if (serverFlag)
		{
			
		}
	}
	
	public void AImove() throws PICK_FROM_EMPTY_PIT, INVALID_PIT_NUMBER, InterruptedException, EMPTY_BOARD_EXCEPTION {
	
        Object mutex = new Object();
        try{
            synchronized(mutex){
                TimedThread thread = new TimedThread(board, ai, mutex, 10000);
                thread.start();
                //thread.join();
                
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
                ai.makeMove(board, difficulty);
            }
            catch(Exception e){
                System.out.println(e.toString());
            }
        }
    }
	
}