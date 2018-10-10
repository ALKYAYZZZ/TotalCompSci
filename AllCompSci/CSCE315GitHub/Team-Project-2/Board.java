package boardPackage;

import java.util.Arrays;
import java.util.Vector;

public class Board {
	Vector<Integer> pits;
	
	public Board()
	{
		pits = new Vector<Integer>(Arrays.asList(4,4,4,4,4,4,0,4,4,4,4,4,4,0));
	}
	
	public Board(Vector<Integer> argPits)
	{
		pits = argPits;
	}
	
	public Vector<Integer> getPits()
	{
		Vector<Integer> returnPits = pits;
		return returnPits;
	}
	
	public Boolean pick(Integer pitNum) throws PICK_FROM_EMPTY_PIT, INVALID_PIT_NUMBER
	{
		Integer ignoredPit = 0;
		Integer startingSide = 0;
		Integer storePit = 0;
		Boolean endedInStore = false;
		
		if (pitNum == 6 || pitNum == 13 || pitNum < 0 || pitNum > 14)
		{
			throw new INVALID_PIT_NUMBER("");
		}
		else if (pitNum < 6)
		{
			ignoredPit = 13;
			startingSide = 0;
			storePit = 6;
		}
		else
		{
			ignoredPit = 6;
			startingSide = 1;
			storePit = 13;
		}
		
		Integer seedsInHand = pits.elementAt(pitNum);
		
		if (seedsInHand == 0)
		{
			throw new PICK_FROM_EMPTY_PIT("");
		}
		
		pits.setElementAt(0, pitNum);
		
		Integer currentPit = (pitNum + 1) % 14;
		
		while (seedsInHand > 0)
		{
			if (currentPit != ignoredPit)
			{
				pits.setElementAt(pits.elementAt(currentPit) + 1, currentPit);
				seedsInHand--;
				if (seedsInHand == 0 && currentPit == storePit)
				{
					endedInStore = true;
				}
				if (seedsInHand == 0 && (currentPit / 7) == startingSide && currentPit != storePit && pits.elementAt(currentPit) == 1 && pits.elementAt(oppositePit(currentPit)) != 0)
				{
					pits.setElementAt(pits.elementAt(oppositePit(currentPit)) + pits.elementAt(currentPit), storePit);
					pits.setElementAt(0, currentPit);
					pits.setElementAt(0, oppositePit(currentPit));
				}
			}
			currentPit = (currentPit + 1) % 14;
		}
		
		return endedInStore;
	}
	
	public static Integer oppositePit(Integer pitNum)
	{
		return 12 - pitNum;
	}
	
	public Integer side0Score()
	{
		return pits.elementAt(6);
	}
	
	public Integer side1Score()
	{
		return pits.elementAt(13);
	}
	
	public Integer seedsInPit(Integer pitNum)
	{
		return pits.elementAt(pitNum);
	}
	
	public Boolean ended() throws EMPTY_BOARD_EXCEPTION
	{
		Boolean side1Emptied = (pits.elementAt(0) == 0) && (pits.elementAt(1) == 0) && (pits.elementAt(2) == 0) && (pits.elementAt(3) == 0) && (pits.elementAt(4) == 0) && (pits.elementAt(5) == 0);
		Boolean side2Emptied = (pits.elementAt(7) == 0) && (pits.elementAt(8) == 0) && (pits.elementAt(9) == 0) && (pits.elementAt(10) == 0) && (pits.elementAt(11) == 0) && (pits.elementAt(12) == 0);
		
		if (side1Emptied && side2Emptied)
		{
			throw new EMPTY_BOARD_EXCEPTION("");
		}
		else if (side1Emptied)
		{
			Integer seedsOnSide2 = pits.elementAt(7) + pits.elementAt(8) + pits.elementAt(9) + pits.elementAt(10) + pits.elementAt(11) + pits.elementAt(12);
			pits.setElementAt(pits.elementAt(13) + seedsOnSide2, 13);
			for (int i = 7;i < 13; i++)
			{
				pits.setElementAt(0, i);
			}
		}
		else if (side2Emptied)
		{
			Integer seedsOnSide1 = pits.elementAt(0) + pits.elementAt(1) + pits.elementAt(2) + pits.elementAt(3) + pits.elementAt(4) + pits.elementAt(5);
			pits.setElementAt(pits.elementAt(6) + seedsOnSide1, 6);
			for (int i = 0;i < 6; i++)
			{
				pits.setElementAt(0, i);
			}
		}
		return side1Emptied || side2Emptied;
	}
	
	// AI STUFF
	
	public Board boardAfter(Vector<Integer> moves) throws PICK_FROM_EMPTY_PIT, INVALID_PIT_NUMBER
	{
		Board newBoard = new Board(pits);
		
		for (int i = 0; i < moves.size(); i++)
		{
			newBoard.pick(moves.elementAt(i));
		}
		
		 return newBoard;
	}
	
	public Vector<Integer> possibleMovesPlayer1()
	{
		Vector<Integer> moves = new Vector<Integer>();
		
		for (int i = 0; i < 6; i++)
		{
			if (pits.elementAt(i) > 0)
			{
				moves.addElement(i);
			}
		}
		
		return moves;
	}
	
	public Vector<Integer> possibleMovesPlayer2()
	{
		Vector<Integer> moves = new Vector<Integer>();
		
		for (int i = 7; i < 13; i++)
		{
			if (pits.elementAt(i) > 0)
			{
				moves.addElement(i);
			}
		}
		
		return moves;
	}
}
