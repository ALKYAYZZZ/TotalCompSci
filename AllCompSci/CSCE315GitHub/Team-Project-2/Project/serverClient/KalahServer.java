package serverClient;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Arrays;
import java.util.Vector;

import boardPackage.*;

public class KalahServer {
	Board board;
	public Long timeLimit;
	public Integer playerNum;
	Boolean gameOver = false;
	Integer bufferSize = 50;
	ServerSocket serverSocket = null;
	Socket clientSocket = null;
	Socket clientSocket1 = null;
	PrintWriter out;
	BufferedReader in;
	PrintWriter out1;
	BufferedReader in1;
	Boolean humanPlayer;
	Boolean multiClientGame;
	public Integer turn;
	Boolean winner = false;
	Boolean tie = true;
	
	public KalahServer() {};
	
	public void launchServer()
	{
		// describe how the game terminated
		Boolean illegalMoveMade = false;
		Boolean tookTooLong = false;
		
//		setupServer();
		
		// only accepts 1 if the game is client to server
		// also sends welcome to all clients
		acceptConnections();
		
		// initialize the game and notify client(s)
//		createBoard();
		
		while (!gameOver)
		{
			// analyzing time to receive command and receiving command
			tookTooLong = false;
			long startTime = System.currentTimeMillis();
			String cmd = waitForCommand(turn);
			long timeToReceive = System.currentTimeMillis() - startTime;
			
			// checks for readable characters and that the command fits in the buffer
			if (!legalInput(cmd, bufferSize))
			{	
				illegalMoveMade = true;
				gameOver = true;
				break;
			}	
			
			Vector<String> args = tokenize(cmd);
			
			// first if else block modifies local copy of board
			Integer cmdType = commandType(args);
			if (cmdType == 0) handleInfo(args);
			else if (cmdType == 1)
			{
				Boolean allLegalMoves = handleMove(args);
				if (!allLegalMoves)
				{
					illegalMoveMade = true;
					gameOver = true;
					break;
				}
			}
			else if (cmdType == 2);
			else
			{
				illegalMoveMade = true;
				gameOver = true;
				break;
			}
			
			// second if else block has to do with what happens after
			if (cmdType != 2)
			{
				tookTooLong = (timeToReceive > timeLimit) && timeLimit > 0;
				
				if (tookTooLong)
				{
					gameOver = true;
					break;
				}
				
				// this happens regardless of whether the game is multiclient or not
				sendAck(turn);
				
				if (!multiClientGame)
				{
					try {
						// check if the game is over before server makes move
						if (!board.ended())
						{
							String cmdToSend = determineMove();
							
							sendCommand(cmdToSend,0);
							
							// check if the game is over after the server makes the move
							if (board.ended())
							{
								gameOver = true;
							}
						}
						else
						{
							gameOver = true;
						}
					} catch (EMPTY_BOARD_EXCEPTION e) {
						gameOver = true;
					}
				}
				// it is a multiclient game
				else
				{
					sendCommand(cmd, 1-turn);
					try {
						if (board.ended()) gameOver = true;
					} catch (EMPTY_BOARD_EXCEPTION e) {
						gameOver = true;
					}
					turn = 1 - turn;
				}
			}
			else handleAck(args);
		}
		// end loop
		
		// these variables only used in non multiclient game
		
		
		if (illegalMoveMade)
		{
			if (turn == 0)
			{
				sendCommand("ILLEGAL",0);
				if (multiClientGame) sendCommand("ILLEGAL",1);
				sendCommand("LOSER",0);
				if (multiClientGame) sendCommand("WINNER",1);
				winner = true;
			}
			else
			{
				sendCommand("ILLEGAL",0);
				sendCommand("ILLEGAL",1);
				sendCommand("WINNER",0);
				sendCommand("LOSER",1);
			}
		}
		if (tookTooLong)
		{
			sendCommand("TIME",0);
			if (multiClientGame) sendCommand("TIME",1);
			
			if (turn == 0 || !multiClientGame)
			{
				sendCommand("LOSER",0);
				if (multiClientGame) sendCommand("WINNER",1);
			}
			else
			{
				sendCommand("WINNER",0);
				sendCommand("LOSER",1);
			}
			winner = true;
		}
		// the game ended like normal
		else 
		{
			if (multiClientGame)
			{
				if (board.player1Score() > board.player2Score())
				{
					sendCommand("WINNER",0);
					sendCommand("LOSER",1);
				}
				else if (board.player1Score() < board.player2Score())
				{
					sendCommand("WINNER",1);
					sendCommand("LOSER",0);
				}
				else
				{
					sendCommand("TIE",0);
					sendCommand("TIE",1);
				}
			}
			else
			{
				if (board.player1Score() > board.player2Score())
				{
					if (playerNum == 0)
					{
						sendCommand("LOSER",0);
						winner = true;
					}
					else
					{
						sendCommand("WINNER",1);
						winner = false;
					}
				}
				else if (board.player1Score() < board.player2Score())
				{
					if (playerNum == 0)
					{
						sendCommand("WINNER",0);
						winner = false;
					}
					else
					{
						sendCommand("LOSER",1);
						winner = true;
					}
				}
				else
				{
					sendCommand("TIE",0);
					tie = true;
				}
			}
		}
		try {
			serverSocket.close();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		// notifying user of the outcome if the server is a player
		if (!multiClientGame)
		{
			if (tie)
			{
				// TODO communicate to user
			}
			else if (winner)
			{
				// TODO communicate to user
			}
			else
			{
				// TODO communicate to user
			}
		}
		
	}
	
	public void setupServer(Boolean hPlayer, Boolean MCG, Integer pNum)
	{
		
		this.humanPlayer = hPlayer;
		multiClientGame = MCG;
		try
		{
			serverSocket = new ServerSocket(pNum);
			
		} 
		catch (IOException e) 
		{
			e.printStackTrace();
		}
	}
	
	public void acceptConnections() {
		try {
			clientSocket = serverSocket.accept();
			if (multiClientGame) clientSocket1 = serverSocket.accept();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}	
		// initializing outputs to clients
		
		try {
			out = new PrintWriter(clientSocket.getOutputStream(),true);
			if (multiClientGame)
			{
				out1 = new PrintWriter(clientSocket1.getOutputStream(),true);
			}
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		sendCommand("WELCOME",0);
		if (multiClientGame) sendCommand("WELCOME",1);
	}

	public void createBoard(Integer pitsPerSide, Integer seedsPerPit, Boolean randomStart, Long tL, Integer playNumber) {
		timeLimit = tL;
		playerNum = 1;
		if (multiClientGame) playerNum = 2;
		turn = 0;

		
		// creating local copy of board
		try {
			board = new Board(seedsPerPit, pitsPerSide, randomStart);
		} catch (INVALID_STARTING_SEEDS e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (INVALID_NUMBER_OF_PITS e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		// translating board info into INFO commands
		String infoCmd = "";
		String cmdForSecond = "";
		infoCmd.concat("INFO ");
		infoCmd.concat(pitsPerSide.toString() + ' ' + seedsPerPit.toString() + ' ' + timeLimit.toString());
		
		if (!multiClientGame)
		{
			if (playerNum == 1) infoCmd.concat("F ");
			else infoCmd.concat("S ");
		}
		else
		{
			cmdForSecond = new String(infoCmd);
			infoCmd.concat("F ");
			cmdForSecond.concat("S ");
		}
		
		if (!randomStart) 
		{
			infoCmd.concat("S ");
			if (multiClientGame) cmdForSecond.concat("S ");
		}
		else
		{
			infoCmd.concat("R ");
			if (multiClientGame) cmdForSecond.concat("R ");
			for (int i = 0; i < board.getPits().size();i++)
			{
				infoCmd.concat(board.getPits().elementAt(i).toString());
				if (multiClientGame) cmdForSecond.concat(board.getPits().elementAt(i).toString());
				if (i != (board.getPits().size() - 1)) 
				{
					infoCmd.concat(" ");
					if (multiClientGame) cmdForSecond.concat(" ");
				}
			}
		}
		
		
		
		// sending board information to clients
		
		sendCommand(infoCmd,0);
		if (multiClientGame) sendCommand(cmdForSecond,1);
		
	}

	public void sendCommand(String cmdToSend, Integer receivingClient) {
		if (receivingClient == 0) out.println(cmdToSend);
		else out1.println(cmdToSend);
		
	}

	public String waitForCommand(Integer clientNum)
	{
		String clientMsg = "";
		try
		{
			if (clientNum == 0)
			{
//				out = new PrintWriter(clientSocket.getOutputStream(),true);
				in = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
				clientMsg = in.readLine();
			}
			else
			{
//				out1 = new PrintWriter(clientSocket1.getOutputStream(),true);
				in1 = new BufferedReader(new InputStreamReader(clientSocket1.getInputStream()));
				clientMsg = in1.readLine();
			}
			return clientMsg;
		}
		catch (IOException e) 
		{
			e.printStackTrace();
		}
		return "";
	}
	
	public void sendAck(Integer clientNum)
	{
		if (clientNum == 1) out1.println("OK");
		else out.println("OK");
	}
	
	// -------------------------------------------------------------------------------------------------------------
	// FUNCTIONS FOR WHEN SERVER IS A PLAYER
	
	private String determineMove() {
		String cmdToSend ="";
		if (humanPlayer) cmdToSend = getPlayerMove();
		else cmdToSend = getAIMove();
		return cmdToSend;
	}
	
	public String getPlayerMove()
	{
		// TODO prompts player for move, makes the move, and translates move to cmd
		
		// wait for gui
		return "";
	}
	
	public String getAIMove()
	{
		// TODO gets move from AI, makes the move, and translates to cmd
		return "";
	}
	
	// --------------------------------------------------------------------------------------------------------------
	// HANDLING INPUTS
	
	public Integer handleCommand(String cmd)
	{
		Vector<String> args = tokenize(cmd);
		
		// first if else block modifies local copy of board
		Integer cmdType = commandType(args);
		if (cmdType == 0) handleInfo(args);
		else if (cmdType == 1)
		{
			Boolean allLegalMoves = handleMove(args);
			if (!allLegalMoves)
			{
				return -1;
			}
		}
		else if (cmdType == 2);
		else
		{
			return -1;
		}
		return cmdType;
	}
	
	public void handleInfo(Vector<String> args)
	{
		Integer pitsPerSide = Integer.parseInt(args.elementAt(1));
		Integer seedsPerHole = Integer.parseInt(args.elementAt(2));
		timeLimit = Long.parseLong(args.elementAt(3));
		if (args.elementAt(4) == "F") playerNum = 1;
		else playerNum = 0;
		Boolean randomStart = args.elementAt(5) == "R";
		if (randomStart)
		{
			Vector<Integer> pitsOnSide = new Vector<Integer>();
			for (int i = 6; i < args.size(); i++)
			{
				pitsOnSide.add(Integer.parseInt(args.elementAt(i)));
			}
			
			Vector<Integer> startingBoard = new Vector<>();
			for (int j = 0; j < 2; j++)
			{
				for (int i = 0; i < pitsOnSide.size(); i++)
				{
					startingBoard.add(pitsOnSide.elementAt(i));
				}
				startingBoard.add(0);
			}
			
		}
		else
		{
			try
			{
				board = new Board(seedsPerHole, pitsPerSide, false);
			}
			catch (INVALID_STARTING_SEEDS e)
			{
				
			}
			catch (INVALID_NUMBER_OF_PITS e)
			{
				
			}
		}
	}
	
	public Boolean handleMove(Vector<String> args)
	{
		if (args.size() == 1 && args.elementAt(0) == "-1")
		{
			board.swapSides();
		}
		else
		{
			Vector<Integer> movesToMake = new Vector<Integer>();
			for (int i = 0; i < args.size(); i++)
			{
				movesToMake.add(Integer.parseInt(args.elementAt(i)));
			}
			
			try
			{
				for (int i = 0; i < movesToMake.size(); i++)
				{
					if (!board.legalMove(movesToMake.elementAt(i), turn + 1)) return false;
					board.pick(movesToMake.elementAt(i));
				}
			}
			catch (INVALID_PIT_NUMBER e)
			{
				
			}
			catch (PICK_FROM_EMPTY_PIT e)
			{
				
			}
			
		}
		return true;
	}
	
	public Integer handleAck(Vector<String> args)
	{
		String ack = args.elementAt(0);
		
		if (ack == "OK") return 0;
		if (ack == "READY")
		{
			
			if (playerNum == 0)
			{
				String cmd = determineMove();
				
				sendCommand(cmd, 0);
			}
			return 0;
		} 
		else
		{
			if (!gameOver) gameOver = true;
			if (ack == "WINNER")
			{
				winner = true;
				return 2;
			}
			else if (ack == "LOSER")
			{
				winner = false;
				return 2;
			}
			return 1;
		}
	}
	
	// -------------------------------------------------------------------------------------- //
	
	// UTILITY FUNCTIONS
	
	
	public static Integer commandType(Vector<String> args)
	{
		if (isInfo(args)) return 0;
		if (isMove(args)) return 1;
		if (isAck(args)) return 2;
		return -1;
	}
	
	public static boolean isInteger(String s) {
	    try 
	    { 
	        Integer.parseInt(s); 
	    } 
	    catch(NumberFormatException e) 
	    { 
	        return false; 
	    } 
	    catch(NullPointerException e) 
	    {
	        return false;
	    }
	    
	    return true;
	}
	
	public static Boolean isInfo(Vector<String> args)
	{
		if (args.size() < 6) return false;
		if (args.elementAt(0) != "INFO") return false;
		for (int i = 1; i < 4; i++)
		{
			if (!isInteger(args.elementAt(i))) return false;
		}
		if ((args.elementAt(4) != "F") && (args.elementAt(4) != "S")) return false;
		if ((args.elementAt(5) != "R") && (args.elementAt(5) != "S")) return false;
		if (args.elementAt(5) == "R")
		{
			Integer numPits = Integer.parseInt(args.elementAt(1));
			if (args.size() != (6 + numPits)) return false;
			for (int i = 6; i < args.size(); i++)
			{
				if (!isInteger(args.elementAt(i))) return false;
			}
		}
		return true;
	}
	
	public static Boolean isMove(Vector<String> args)
	{
		if (args.size() == 0) return false;
		if (args.size() == 1 && args.elementAt(0) == "P") return true;
		for (int i = 0; i < args.size();i++)
		{
			if (!isInteger(args.elementAt(i))) return false;
		}
		return true;
	}
	
	public static Boolean isAck(Vector<String> args)
	{
		if (args.size() != 1) return false;
		Vector<String> possibleAcks = new Vector<String>(Arrays.asList("WELCOME","READY","OK","ILLEGAL","TIME","LOSER","WINNER","TIE"));
		for (int i = 0; i < possibleAcks.size(); i++)
		{
			if (args.elementAt(0) == possibleAcks.elementAt(i)) return true;
		}
		return false;
	}
	
	public static Boolean legalInput(String cmd, Integer bufferSize)
	{
		if (cmd.length() > bufferSize) return false;
		for (int i = 0; i < cmd.length(); i++)
		{
			if (cmd.charAt(i) >= 32 && cmd.charAt(i) < 127)
			{
				// this is okay
			}
			else return false;
		}
		// determine if the command fits in the buffer
		return true;
	}
	
	public static Vector<String> tokenize(String cmd)
	{
		Vector<String> tokens = new Vector<String>();
		
		String currentToken = "";
		
		for (int i = 0; i < cmd.length(); i++)
		{
			if (cmd.charAt(i) == ' ' && currentToken != "")
			{
				tokens.add(currentToken);
				currentToken = "";
			}
			else
			{
				currentToken = currentToken + cmd.charAt(i);
			}
		}
		
		if (currentToken != "") tokens.addElement(currentToken);
		return tokens;
	}
}