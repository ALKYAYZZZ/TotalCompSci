package serverClient;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.Vector;

import boardPackage.Board;
import boardPackage.INVALID_NUMBER_OF_PITS;
import boardPackage.INVALID_PIT_NUMBER;
import boardPackage.INVALID_STARTING_SEEDS;
import boardPackage.PICK_FROM_EMPTY_PIT;

public class KalahClient {
	Board board;
	public Integer playerNum;
	Boolean gameOver = false;
	Boolean winnerDetermined = false;
	public Boolean winner = false;
	public Boolean tie = false;
	Integer bufferSize = 50;
	Long timeLimit;
	Socket socket = null;
	PrintWriter out;
	BufferedReader in;
	Boolean humanPlayer;
	
	public KalahClient() 
	{
		
	}
	
	public Boolean setupClient(Boolean hp, String hostName, Integer portNumber)
	{
		
		humanPlayer = hp;
		try {
			socket = new Socket(hostName, portNumber);
		} catch (UnknownHostException e) {
			// TODO Auto-generated catch block
			return false;
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return true;
	}

	public Integer handleAck(Vector<String> args) {
		String ack = args.elementAt(0);
		
		if (ack == "OK") return 0;
		if (ack == "WELCOME") return 0;
		else
		{
			if (!gameOver) gameOver = true;
			if (ack == "WINNER")
			{
				winnerDetermined = true;
				winner = true;
				return 2;
			}
			else if (ack == "LOSER")
			{
				winnerDetermined = true;
				winner = false;
				return 2;
			}
			return 1;
		}
		
	}

	public void sendCommand(String cmdToSend) {
		out.println(cmdToSend);
		
	}
	
	public String determineMove()
	{
		if (humanPlayer) return getPlayerMove();
		return getAIMove();
	}

	public String getPlayerMove() 
	{
//		while() {
//			
//		}
		return null;
	}
	
	public String getAIMove()
	{
		// TODO get move from AI, make move, translate move to cmd
		return null;
	}

	public Integer handleCommand(String cmd)
	{
		Vector<String> args = KalahServer.tokenize(cmd);
		
		// first if else block modifies local copy of board
		Integer cmdType = KalahServer.commandType(args);
		if (cmdType == 0) handleInfo(args);
		else if (cmdType == 1)
		{
			handleMove(args);
		}
		else if (cmdType == 2);
		else
		{
			return -1;
		}
		return cmdType;
	}
	
	public void handleMove(Vector<String> args) {
		if (args.size() == 1 && args.elementAt(0) == "P")
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
		
	}

	public void handleInfo(Vector<String> args) {
		Integer pitsPerSide = Integer.parseInt(args.elementAt(1));
		Integer seedsPerHole = Integer.parseInt(args.elementAt(2));
		timeLimit = Long.parseLong(args.elementAt(3));
		if (args.elementAt(4) == "F") playerNum = 0;
		else playerNum = 1;
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
			board = new Board(startingBoard);
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

	public void sendAck() {
		out.println("OK");
		
	}

	public String waitForCommand() {
		try {
			out = new PrintWriter(socket.getOutputStream(), true);
			in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
			String serverMsg = in.readLine();
			return serverMsg;
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return null;
	}

}