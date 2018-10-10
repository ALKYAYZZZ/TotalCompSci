package boardPackage;

import java.util.Random;
import java.util.Vector;

public class Board {
	Vector<Integer> pits;
	Integer player1StoreIndex;
	Integer player2StoreIndex;
	public int houses;
	
	public Board(Integer startingSeeds, Integer pitsPerSide, Boolean randomStart) throws INVALID_STARTING_SEEDS, INVALID_NUMBER_OF_PITS
	{
		if (startingSeeds < 1 || startingSeeds > 10)
		{
			throw new INVALID_STARTING_SEEDS("");
		}
		if (pitsPerSide < 4 || pitsPerSide > 9)
		{
			throw new INVALID_NUMBER_OF_PITS("");
		}
		
		pits = new Vector<Integer>(2 * pitsPerSide + 2);
		houses = pitsPerSide;
		
		for (int i = 0; i < 2 * pitsPerSide + 2; i++)
		{
			pits.add(0);
		}
		
		if (!randomStart)
		{
			for (int i = 0; i < pitsPerSide; i++)
			{
				pits.setElementAt(startingSeeds, i);
				pits.setElementAt(startingSeeds, i + pitsPerSide + 1);
			}
		}
		else
		{
			Random r = new Random();
			Integer seedsToDistribute = pitsPerSide * startingSeeds;
			Integer currentPit = 0;
			Vector<Integer> generatedSide = new Vector<>();
			
			for (int i = 0; i < pitsPerSide; i++)
			{
				generatedSide.add(0);
			}
			
			while (seedsToDistribute > 0)
			{
				if (r.nextBoolean())
				{
					generatedSide.set(currentPit, generatedSide.elementAt(currentPit) + 1);
					seedsToDistribute--;
				}
				currentPit = (currentPit + 1) % pitsPerSide;
			}
			
			for (int i = 0; i < pitsPerSide; i++)
			{
				pits.setElementAt(generatedSide.elementAt(i), i);
				pits.setElementAt(generatedSide.elementAt(i), i + pitsPerSide + 1);
			}
		}
		
		player1StoreIndex = pitsPerSide;
		player2StoreIndex = player1StoreIndex + pitsPerSide + 1; 
		pits.setElementAt(0, player1StoreIndex);
		pits.setElementAt(0, player2StoreIndex);
	}
	
	public Board(Board board)
	{
		this.pits = new Vector<Integer>(board.pits);
		this.player1StoreIndex = board.player1StoreIndex;
		this.player2StoreIndex = board.player2StoreIndex;
		this.houses = board.houses;
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
		
		if (pitNum == player1StoreIndex || pitNum >= player2StoreIndex || pitNum < 0)
		{
			throw new INVALID_PIT_NUMBER("");
		}
		else if (pitNum < player1StoreIndex)
		{
			ignoredPit = player2StoreIndex;
			startingSide = 0;
			storePit = player1StoreIndex;
		}
		else
		{
			ignoredPit = player1StoreIndex;
			startingSide = 1;
			storePit = player2StoreIndex;
		}
		
		Integer seedsInHand = pits.elementAt(pitNum);
		
		if (seedsInHand == 0)
		{
			throw new PICK_FROM_EMPTY_PIT("");
		}
		
		pits.setElementAt(0, pitNum);
		
		Integer currentPit = (pitNum + 1) % pits.size();
		
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
				if (seedsInHand == 0 && (currentPit / (pits.size() / 2)) == startingSide && currentPit != storePit && pits.elementAt(currentPit) == 1 && pits.elementAt(oppositePit(currentPit)) != 0)
				{
					pits.setElementAt(pits.elementAt(oppositePit(currentPit)) + pits.elementAt(currentPit) + pits.elementAt(storePit), storePit);
					pits.setElementAt(0, currentPit);
					pits.setElementAt(0, oppositePit(currentPit));
				}
			}
			currentPit = (currentPit + 1) % pits.size();
		}
		
		return endedInStore;
	}
	
	public Integer oppositePit(Integer pitNum)
	{
		return pits.size() - 2 - pitNum;
	}
	
	public Integer player1Score()
	{
		return pits.elementAt(player1StoreIndex);
	}
	
	public Integer player2Score()
	{
		return pits.elementAt(player2StoreIndex);
	}
	
	public Integer seedsInPit(Integer pitNum)
	{
		return pits.elementAt(pitNum);
	}
	
	public Boolean ended() throws EMPTY_BOARD_EXCEPTION
	{
		Boolean player1SideEmptied = true;
		Boolean player2SideEmptied = true;
		
		if (player1StoreIndex > player2StoreIndex)
		{
			for (int i = player2StoreIndex + 1; i < player1StoreIndex && player1SideEmptied; i++)
			{
				if (pits.elementAt(i) > 0)
				{
					player1SideEmptied = false;
				}
			}
			for (int i = 0; i < player2StoreIndex && player2SideEmptied; i++)
			{
				if (pits.elementAt(i) > 0)
				{
					player2SideEmptied = false;
				}
			}
		}
		else
		{
			for (int i = 0; i < player1StoreIndex && player1SideEmptied; i++)
			{
				if (pits.elementAt(i) > 0)
				{
					player1SideEmptied = false;
				}
			}
			for (int i = player1StoreIndex + 1; i < player2StoreIndex && player2SideEmptied; i++)
			{
				if (pits.elementAt(i) > 0)
				{
					player2SideEmptied = false;
				}
			}
		}
		
		if (player1SideEmptied && player2SideEmptied)
		{
			throw new EMPTY_BOARD_EXCEPTION("");
		}
		else if (player1SideEmptied)
		{
			int startingPit = 0;
			
			if (player2StoreIndex > player1StoreIndex)
			{
				startingPit = player1StoreIndex + 1;
			}
			
			for (int i = startingPit; i < player2StoreIndex; i++)
			{
				pits.setElementAt(pits.elementAt(player2StoreIndex) + pits.elementAt(i), player2StoreIndex);
				pits.setElementAt(0, i);
			}
		}
		else if (player2SideEmptied)
		{
			int startingPit = 0;
			
			if (player1StoreIndex > player2StoreIndex)
			{
				startingPit = player2StoreIndex + 1;
			}
			
			for (int i = startingPit; i < player1StoreIndex; i++)
			{
				pits.setElementAt(pits.elementAt(player1StoreIndex) + pits.elementAt(i), player1StoreIndex);
				pits.setElementAt(0, i);
			}
		}
		
		return player1SideEmptied || player2SideEmptied;
	}
	
	public void swapSides()
	{
		int tempVal = player1StoreIndex;
		player1StoreIndex = player2StoreIndex;
		player2StoreIndex = tempVal;
	}
	
	// AI STUFF
	
//	public Board boardAfter(Vector<Integer> moves) throws PICK_FROM_EMPTY_PIT, INVALID_PIT_NUMBER
//	{
//		Board newBoard = new Board();
//		
//		for (int i = 0; i < moves.size(); i++)
//		{
//			newBoard.pick(moves.elementAt(i));
//		}
//		
//		 return newBoard;
//	}
	
	public Vector<Integer> possibleMovesPlayer1()
	{
		Vector<Integer> moves = new Vector<Integer>();
		
		int startingPit = 0;
		
		if (player1StoreIndex > player2StoreIndex)
		{
			startingPit = player2StoreIndex + 1;
		}
		
		for (int i = startingPit; i < player1StoreIndex; i++)
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
		
		int startingPit = 0;
		
		if (player2StoreIndex > player1StoreIndex)
		{
			startingPit = player1StoreIndex + 1;
		}
		
		for (int i = startingPit; i < player2StoreIndex; i++)
		{
			if (pits.elementAt(i) > 0)
			{
				moves.addElement(i);
			}
		}
		
		return moves;
	}
}