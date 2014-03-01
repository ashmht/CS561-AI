/**
 * 
 */

import java.util.ArrayList;

/**
 * @author Karthikeyan S
 *
 */
public class Node 
{
	/*
	 * Data Members
	 */
	public char[][] boardState;
	public ArrayList<Node> jumpSuccesors;
	public ArrayList<Node> simpleMoveSuccesors;
	public ArrayList<Node> prunedNodeList;
	
	public int numberOfJumpMoves = 0;
	public int numberOfSimpleMoves = 0;
	
	public Position move;
	public Position startMove;
	
	/*
	 * Member Functions
	 */
	public Node()
	{
		
	}
	
	public Node(char[][] _boardState)
	{
		//Set the boardState to the current node.
		boardState = new char[8][8];
		for(int l = 0; l < 8; l++)
			boardState[l] = _boardState[l].clone();
		
		jumpSuccesors = new ArrayList<Node>();
		simpleMoveSuccesors = new ArrayList<Node>();
		move  = new Position();
		startMove = new Position();
	}
	
	public int getUtilityValue(boolean player,boolean movePossible)
	{
		/*
		 * We compute the hValue:
		 * h = A' - B'
		 * k = 2 and K = 2
		 * A = B = 1
		 * A' = Sum of Alice's pieces on the board.
		 * B' = Sum of Bill's pieces on the board.
		 */
		int countOfA = 0,countOfB = 0;
		
		for(int i =0;i<8;i++)
		{
			for(int j =0;j<8;j++)
			{
				switch (boardState[i][j])
				{
					case 'A':
						countOfA++;
						break;
					case 'B':
						countOfB++;
						break;
					case 'k':
						countOfA += 2;
						break;
					case 'K':
						countOfB += 2;
						break;
					default:
				}
			}
		}
		
		int h;
		if(countOfB > 0 && countOfA < 1)
		{
			//There are no pieces for A
			//A loses
			h = Integer.MIN_VALUE;
		}
		else if(countOfA > 0 && countOfB < 1)
		{
			//Ther are no pieces for B
			//B loses
			h = Integer.MAX_VALUE;
		}
		else
		{
			if(player == true && movePossible == false)
			{
				//Player is A and there are no valid moves possible for A
				//So B wins.
				h = Integer.MIN_VALUE;
			}
			else if(player == false && movePossible == false)
			{
				//Player is B and there are no valid moves for B
				//So A wins
				h = Integer.MAX_VALUE;
			}
			else
			{
				//There are possible moves left.
				h = countOfA - countOfB;
			}
		}
		
		return h;
	}
	
	public void computeSuccesorsOfNode(boolean player)
	{
		if(player)
		{
			for(int i =0;i<8;i++)
			{
				for(int j =0;j<8;j++)
				{
					switch (boardState[i][j])
					{
						case 'A':
						{
							/*
							 * move diagonally forward right.
							 */
							if((i-1) >= 0 && (j+1) < 8 && boardState[i-1][j+1] == 'O')
							{
								char[][] newBoardState = new char[8][8];
								for(int l = 0; l < 8; l++)
									newBoardState[l] = boardState[l].clone();
								
								/*
								 * newBoardState[i][j] = A
								 * Now if this condition is true, we can move A to newBoardState[i-1][j+1]
								 * i.e, newBoardState[i-1][j+1] := A and newBoardState[i][j] := O
								*/
								
								if((i-1) == 0)
								{
									//The piece has reached to the top of B's side,
									//turn it to a king piece.
									newBoardState[i-1][j+1] = 'k';
								}
								else 
								{
									newBoardState[i-1][j+1] = 'A';
								}
								newBoardState[i][j] = 'O';
								
								Node n = new Node(newBoardState);
								n.move.setFromPosition(i, j);
								n.move.setToPosition(i-1, j+1);
								
								simpleMoveSuccesors.add(n);
								numberOfSimpleMoves++;
							}
							/*
							 * move diagonally forward left.
							 */
							if((i-1) >= 0 && (j-1) >= 0 && boardState[i-1][j-1] == 'O')
							{
								char[][] newBoardState = new char[8][8];
								for(int l = 0; l < 8; l++)
									newBoardState[l] = boardState[l].clone();
								
								/*
								 * newBoardState[i][j] = A
								 * Now if this condition is true, we can move A to newBoardState[i-1][j-1]
								 * i.e, newBoardState[i-1][j-1] := A and newBoardState[i][j] := O
								*/
								
								if((i-1) == 0)
								{
									//The piece has reached to the top of B's side,
									//turn it to a king piece.
									newBoardState[i-1][j-1] = 'k';
								}
								else 
								{
									newBoardState[i-1][j-1] = 'A';
								}
								newBoardState[i][j] = 'O';
								
								Node n = new Node(newBoardState);
								n.move.setFromPosition(i, j);
								n.move.setToPosition(i-1, j-1);
								
								simpleMoveSuccesors.add(n);
								numberOfSimpleMoves++;
							}
							/*
							 * Single JUMP diagonally forward right.
							 */
							if((i-1) >= 0 && (j+1) < 8 && (boardState[i-1][j+1] == 'B' || boardState[i-1][j+1] == 'K'))
							{
								//possible jump
								if((i-2) >= 0 && (j+2) < 8 && boardState[i-2][j+2] == 'O')
								{
									char[][] newBoardState = new char[8][8];
									for(int l = 0; l < 8; l++)
										newBoardState[l] = boardState[l].clone();
									
									/*
									 * We can only jump if boardState[i-2][j+2] is empty.
									 * newBoardState[i][j] = A
									 * Need to make 3 switches.
									 * newBoardState[i][j] := O
									 * newBoardState[i-1][j+1] := O
									 * newBoardState[i-2][j+2] := A
									 */
									
									newBoardState[i][j] = 'O';
									newBoardState[i-1][j+1] = 'O'; //jumping over
									if((i-2) == 0)
									{
										newBoardState[i-2][j+2] = 'k';
									}
									else
									{
										newBoardState[i-2][j+2] = 'A';
									}
									
									Node n = new Node(newBoardState);
									n.move.setFromPosition(i, j);
									n.move.setJumpOverPosition(i-1, j+1);
									n.move.setToPosition(i-2, j+2);
									
									jumpSuccesors.add(n);
									numberOfJumpMoves++;
								}
							}
							/*
							 * Single JUMP diagonally forward left.
							 */
							if((i-1) >= 0 && (j-1) >= 0 && (boardState[i-1][j-1] == 'B' || boardState[i-1][j-1] == 'K'))
							{
								//possible jump
								if( (i-2) >= 0 && (j-2) >= 0 && boardState[i-2][j-2] == 'O')
								{
									char[][] newBoardState = new char[8][8];
									for(int l = 0; l < 8; l++)
										newBoardState[l] = boardState[l].clone();
									
									/*
									 * We can only jump if boardState[i-2][j-2] is empty.
									 * newBoardState[i][j] = A
									 * Need to make 3 switches.
									 * newBoardState[i][j] := O
									 * newBoardState[i-1][j-1] := O
									 * newBoardState[i-2][j-2] := A
									 */
									
									newBoardState[i][j] = 'O';
									newBoardState[i-1][j-1] = 'O'; //jumping over
									if((i-2) == 0)
									{
										//The piece has reached to the top of B's side,
										//turn it to a king piece.
										newBoardState[i-2][j-2] = 'k';
									}
									else
									{
										newBoardState[i-2][j-2] = 'A';
									}
									
									Node n = new Node(newBoardState);
									n.move.setFromPosition(i, j);
									n.move.setJumpOverPosition(i-1, j-1);
									n.move.setToPosition(i-2, j-2);
									
									jumpSuccesors.add(n);
									numberOfJumpMoves++;
								}
							}
						}
						break;
						case 'k':
						{
							/*
							 * move diagonally forward right.
							 */
							if((i-1) >= 0 && (j+1) < 8 && boardState[i-1][j+1] == 'O')
							{
								char[][] newBoardState = new char[8][8];
								for(int l = 0; l < 8; l++)
									newBoardState[l] = boardState[l].clone();
								
								/*
								 * newBoardState[i][j] = k
								 * Now if this condition is true, we can move k to newBoardState[i-1][j+1]
								 * i.e, newBoardState[i-1][j+1] := k and newBoardState[i][j] := O
								*/
								
								newBoardState[i-1][j+1] = 'k';
								newBoardState[i][j] = 'O';
								
								Node n = new Node(newBoardState);
								n.move.setFromPosition(i, j);
								n.move.setToPosition(i-1, j+1);
								
								simpleMoveSuccesors.add(n);
								numberOfSimpleMoves++;
							}
							
							/*
							 * move diagonally forward left.
							 */
							if((i-1) >= 0 && (j-1) >= 0 && boardState[i-1][j-1] == 'O')
							{
								char[][] newBoardState = new char[8][8];
								for(int l = 0; l < 8; l++)
									newBoardState[l] = boardState[l].clone();
								
								/*
								 * newBoardState[i][j] = k
								 * Now if this condition is true, we can move k to newBoardState[i-1][j-1]
								 * i.e, newBoardState[i-1][j-1] := k and newBoardState[i][j] := O
								*/
								
								newBoardState[i-1][j-1] = 'k';
								newBoardState[i][j] = 'O';
								
								Node n = new Node(newBoardState);
								n.move.setFromPosition(i, j);
								n.move.setToPosition(i-1, j-1);
								
								simpleMoveSuccesors.add(n);
								numberOfSimpleMoves++;
							}
							
							/*
							 * move diagonally backward left.
							 */
							if((i+1) < 8 && (j-1) >= 0 && boardState[i+1][j-1] == 'O')
							{
								char[][] newBoardState = new char[8][8];
								for(int l = 0; l < 8; l++)
									newBoardState[l] = boardState[l].clone();
								
								/*
								 * newBoardState[i][j] = k
								 * Now if this condition is true, we can move k to newBoardState[i+1][j-1]
								 * i.e, newBoardState[i+1][j-1] := k and newBoardState[i][j] := O
								*/
								
								newBoardState[i+1][j-1] = 'k';
								newBoardState[i][j] = 'O';
								
								Node n = new Node(newBoardState);
								n.move.setFromPosition(i, j);
								n.move.setToPosition(i+1, j-1);
								
								simpleMoveSuccesors.add(n);
								numberOfSimpleMoves++;
							}
							/*
							 * move diagonally backward right.
							 */
							if((i+1) < 8 && (j+1) < 8 && boardState[i+1][j+1] == 'O')
							{
								char[][] newBoardState = new char[8][8];
								for(int l = 0; l < 8; l++)
									newBoardState[l] = boardState[l].clone();
								
								/*
								 * newBoardState[i][j] = k
								 * Now if this condition is true, we can move k to newBoardState[i+1][j+1]
								 * i.e, newBoardState[i+1][j+1] := k and newBoardState[i][j] := O
								*/
								
								newBoardState[i+1][j+1] = 'k';
								newBoardState[i][j] = 'O';
								
								Node n = new Node(newBoardState);
								n.move.setFromPosition(i, j);
								n.move.setToPosition(i+1, j+1);
								
								simpleMoveSuccesors.add(n);
								numberOfSimpleMoves++;
							}
							/*
							 * Jump diagonally forward right.
							 */
							if((i-1) >= 0 && (j+1) < 8 && (boardState[i-1][j+1] == 'B' || boardState[i-1][j+1] == 'K'))
							{
								//possible jump
								if((i-2) >= 0 && (j+2) < 8 && boardState[i-2][j+2] == 'O')
								{
									char[][] newBoardState = new char[8][8];
									for(int l = 0; l < 8; l++)
										newBoardState[l] = boardState[l].clone();
									
									/*
									 * We can only jump if boardState[i-2][j+2] is empty.
									 * newBoardState[i][j] = k
									 * Need to make 3 switches.
									 * newBoardState[i][j] := O
									 * newBoardState[i-1][j+1] := O
									 * newBoardState[i-2][j+2] := k
									 */
									
									newBoardState[i][j] = 'O';
									newBoardState[i-1][j+1] = 'O'; //jumping over
									newBoardState[i-2][j+2] = 'k';
									
									Node n = new Node(newBoardState);
									n.move.setFromPosition(i, j);
									n.move.setJumpOverPosition(i-1, j+1);
									n.move.setToPosition(i-2, j+2);
									
									jumpSuccesors.add(n);
									numberOfJumpMoves++;
								}
							}
							/*
							 * Jump diagonally forward left.
							 */
							if((i-1) >= 0 && (j-1) >= 0 && (boardState[i-1][j-1] == 'B' || boardState[i-1][j-1] == 'K'))
							{
								//possible jump
								if((i-2) >= 0 && (j-2) >= 0 && boardState[i-2][j-2] == 'O')
								{
									char[][] newBoardState = new char[8][8];
									for(int l = 0; l < 8; l++)
										newBoardState[l] = boardState[l].clone();
									
									/*
									 * We can only jump if boardState[i-2][j-2] is empty.
									 * newBoardState[i][j] = k
									 * Need to make 3 switches.
									 * newBoardState[i][j] := O
									 * newBoardState[i-1][j-1] := O
									 * newBoardState[i-2][j-2] := k
									 */
									
									newBoardState[i][j] = 'O';
									newBoardState[i-1][j-1] = 'O'; //jumping over
									newBoardState[i-2][j-2] = 'k';
									
									Node n = new Node(newBoardState);
									n.move.setFromPosition(i, j);
									n.move.setJumpOverPosition(i-1, j-1);
									n.move.setToPosition(i-2, j-2);
									
									jumpSuccesors.add(n);
									numberOfJumpMoves++;
								}
							}
							
							/*
							 * Jump diagonally backward right.
							 */
							if((i+1) < 8 && (j+1) < 8 && (boardState[i+1][j+1] == 'B' || boardState[i+1][j+1] == 'K'))
							{
								//possible jump
								if((i+2) < 8 && (j+2) < 8 &&  boardState[i+2][j+2] == 'O')
								{
									char[][] newBoardState = new char[8][8];
									for(int l = 0; l < 8; l++)
										newBoardState[l] = boardState[l].clone();
									
									/*
									 * We can only jump if boardState[i+2][j+2] is empty.
									 * newBoardState[i][j] = k
									 * Need to make 3 switches.
									 * newBoardState[i][j] := O
									 * newBoardState[i+1][j+1] := O
									 * newBoardState[i+2][j+2] := k
									 */
									
									newBoardState[i][j] = 'O';
									newBoardState[i+1][j+1] = 'O'; //jumping over
									newBoardState[i+2][j+2] = 'k';
									
									Node n = new Node(newBoardState);
									n.move.setFromPosition(i, j);
									n.move.setJumpOverPosition(i+1, j+1);
									n.move.setToPosition(i+2, j+2);
									
									jumpSuccesors.add(n);
									numberOfJumpMoves++;
								}
							}
							
							/*
							 * Jump diagonally backward left.
							 */
							if((j-1) >= 0 && (i+1) < 8 && (boardState[i+1][j-1] == 'B' || boardState[i+1][j-1] == 'K'))
							{
								//possible jump
								if((j-2) >= 0 && (i+2) < 8 && boardState[i+2][j-2] == 'O')
								{
									char[][] newBoardState = new char[8][8];
									for(int l = 0; l < 8; l++)
										newBoardState[l] = boardState[l].clone();
									
									/*
									 * We can only jump if boardState[i+2][j-2] is empty.
									 * newBoardState[i][j] = k
									 * Need to make 3 switches.
									 * newBoardState[i][j] := O
									 * newBoardState[i+1][j-1] := O
									 * newBoardState[i+2][j-2] := k
									 */
									
									newBoardState[i][j] = 'O';
									newBoardState[i+1][j-1] = 'O'; //jumping over
									newBoardState[i+2][j-2] = 'k';
									
									Node n = new Node(newBoardState);
									n.move.setFromPosition(i, j);
									n.move.setJumpOverPosition(i+1, j-1);
									n.move.setToPosition(i+2, j-2);
									
									jumpSuccesors.add(n);
									numberOfJumpMoves++;
								}
							}
						}
						break;
						default:
					}
				}
			}
		}
		else
		{
			for(int i =0;i<8;i++)
			{
				for(int j =0;j<8;j++)
				{
					switch (boardState[i][j])
					{
						case 'B':
						{
							//Move diagonally forward left.
							if((i+1) < 8 && (j+1) < 8 && boardState[i+1][j+1] == 'O')
							{
								char[][] newBoardState = new char[8][8];
								for(int l = 0; l < 8; l++)
									newBoardState[l] = boardState[l].clone();
								
								/*
								 * newBoardState[i][j] = B
								 * Now if this condition is true, we can pos B to newBoardState[i+1][j-1]
								 * i.e, newBoardState[i+1][j-1] := B and newBoardState[i][j] := O
								*/
								
								if((i+1) == 7)
								{
									//The piece has reached to the top of A's side,
									//turn it to a king piece.
									newBoardState[i+1][j+1] = 'K';
								}
								else 
								{
									newBoardState[i+1][j+1] = 'B';
								}
								newBoardState[i][j] = 'O';
								
								Node n = new Node(newBoardState);
								n.move.setFromPosition(i, j);
								n.move.setToPosition(i+1, j+1);
								
								simpleMoveSuccesors.add(n);
								numberOfSimpleMoves++;
							}
							
							//Move diagonally forward right.
							if((i+1) < 8 && (j-1) >= 0 && boardState[i+1][j-1] == 'O')
							{
								char[][] newBoardState = new char[8][8];
								for(int l = 0; l < 8; l++)
									newBoardState[l] = boardState[l].clone();
								
								/*
								 * newBoardState[i][j] = B
								 * Now if this condition is true, we can move B to newBoardState[i+1][j-1]
								 * i.e, newBoardState[i+1][j-1] := B and newBoardState[i][j] := O
								*/
								
								if((i+1) == 7)
								{
									//The piece has reached to the top of A's side,
									//turn it to a king piece.
									newBoardState[i+1][j-1] = 'K';
								}
								else 
								{
									newBoardState[i+1][j-1] = 'B';
								}
								newBoardState[i][j] = 'O';
								
								Node n = new Node(newBoardState);
								n.move.setFromPosition(i, j);
								n.move.setToPosition(i+1, j-1);
								
								simpleMoveSuccesors.add(n);
								numberOfSimpleMoves++;
							}
							
							//Jump diagonally forward left.
							if((i+1) < 8 && (j+1) < 8 && (boardState[i+1][j+1] == 'A' || boardState[i+1][j+1] == 'k'))
							{
								//possible jump
								if((i+2) < 8 && (j+2) < 8 && boardState[i+2][j+2] == 'O')
								{
									char[][] newBoardState = new char[8][8];
									for(int l = 0; l < 8; l++)
										newBoardState[l] = boardState[l].clone();
									
									/*
									 * We can only jump if boardState[i+2][j+2] is empty.
									 * newBoardState[i][j] = B
									 * Need to make 3 switches.
									 * newBoardState[i][j] := O
									 * newBoardState[i+1][j+1] := O
									 * newBoardState[i+2][j+2] := B
									 */
									
									newBoardState[i][j] = 'O';
									newBoardState[i+1][j+1] = 'O'; //jumping over
									if((i+2) == 7)
									{
										//The piece has reached to the top of B's side,
										//turn it to a king piece.
										newBoardState[i+2][j+2] = 'K';
									}
									else
									{
										newBoardState[i+2][j+2] = 'B';
									}
									
									Node n = new Node(newBoardState);
									n.move.setFromPosition(i, j);
									n.move.setJumpOverPosition(i+1, j+1);
									n.move.setToPosition(i+2, j+2);
									
									jumpSuccesors.add(n);
									numberOfJumpMoves++;
								}
							}
							//Jump diagonally forward right.
							if((i+1) < 8 && (j-1) >= 0 && (boardState[i+1][j-1] == 'A' || boardState[i+1][j-1] == 'k'))
							{
								//possible jump
								if( (i+2) < 8 && (j-2) >= 0 && boardState[i+2][j-2] == 'O')
								{
									char[][] newBoardState = new char[8][8];
									for(int l = 0; l < 8; l++)
										newBoardState[l] = boardState[l].clone();
									
									/*
									 * We can only jump if boardState[i+2][j-2] is empty.
									 * newBoardState[i][j] = B
									 * Need to make 3 switches.
									 * newBoardState[i][j] := O
									 * newBoardState[i+1][j-1] := O - Jump Over
									 * newBoardState[i+2][j-2] := B
									 */
									
									newBoardState[i][j] = 'O';
									newBoardState[i+1][j-1] = 'O'; //jumping over
									if((i+2) == 7)
									{
										//The piece has reached to the top of B's side,
										//turn it to a king piece.
										newBoardState[i+2][j-2] = 'K';
									}
									else
									{
										newBoardState[i+2][j-2] = 'B';
									}
									
									Node n = new Node(newBoardState);
									n.move.setFromPosition(i, j);
									n.move.setJumpOverPosition(i+1, j-1);
									n.move.setToPosition(i+2, j-2);
									
									jumpSuccesors.add(n);
									numberOfJumpMoves++;
								}
							}
						}
						break;
						case 'K':
						{
							//move diagonally backward right.
							if((i-1) >= 0 && (j-1) >= 0 && boardState[i-1][j-1] == 'O')
							{
								char[][] newBoardState = new char[8][8];
								for(int l = 0; l < 8; l++)
									newBoardState[l] = boardState[l].clone();
								
								/*
								 * newBoardState[i][j] = K
								 * Now if this condition is true, we can move k to newBoardState[i-1][j-1]
								 * i.e, newBoardState[i-1][j-1] := K and newBoardState[i][j] := O
								*/
								
								newBoardState[i-1][j-1] = 'K';
								newBoardState[i][j] = 'O';
								
								Node n = new Node(newBoardState);
								n.move.setFromPosition(i, j);
								n.move.setToPosition(i-1, j-1);
								
								simpleMoveSuccesors.add(n);
								numberOfSimpleMoves++;
								
							}
							//move diagonally backward left.
							if((i-1) >= 0 && (j+1) < 8 && boardState[i-1][j+1] == 'O')
							{
								char[][] newBoardState = new char[8][8];
								for(int l = 0; l < 8; l++)
									newBoardState[l] = boardState[l].clone();
								
								/*
								 * newBoardState[i][j] = K
								 * Now if this condition is true, we can move k to newBoardState[i-1][j+1]
								 * i.e, newBoardState[i-1][j+1] := K and newBoardState[i][j] := O
								*/
								
								newBoardState[i-1][j+1] = 'K';
								newBoardState[i][j] = 'O';
								
								Node n = new Node(newBoardState);
								n.move.setFromPosition(i, j);
								n.move.setToPosition(i-1, j+1);
								
								simpleMoveSuccesors.add(n);
								numberOfSimpleMoves++;
							}
							//move diagonally forward right.
							if((j-1) >= 0 && (i+1) < 8 && boardState[i+1][j-1] == 'O')
							{
								char[][] newBoardState = new char[8][8];
								for(int l = 0; l < 8; l++)
									newBoardState[l] = boardState[l].clone();
								
								/*
								 * newBoardState[i][j] = K
								 * Now if this condition is true, we can move k to newBoardState[i+1][j-1]
								 * i.e, newBoardState[i+1][j-1] := K and newBoardState[i][j] := O
								*/
								
								newBoardState[i+1][j-1] = 'K';
								newBoardState[i][j] = 'O';
								
								Node n = new Node(newBoardState);
								n.move.setFromPosition(i, j);
								n.move.setToPosition(i+1, j-1);
								
								simpleMoveSuccesors.add(n);
								numberOfSimpleMoves++;
							}
							//move diagonally forward left.
							if((i+1) < 8 && (j+1) < 8 && boardState[i+1][j+1] == 'O')
							{
								char[][] newBoardState = new char[8][8];
								for(int l = 0; l < 8; l++)
									newBoardState[l] = boardState[l].clone();
								
								/*
								 * newBoardState[i][j] = K
								 * Now if this condition is true, we can move k to newBoardState[i+1][j+1]
								 * i.e, newBoardState[i+1][j+1] := K and newBoardState[i][j] := O
								*/
								
								newBoardState[i+1][j+1] = 'K';
								newBoardState[i][j] = 'O';
								
								Node n = new Node(newBoardState);
								n.move.setFromPosition(i, j);
								n.move.setToPosition(i+1, j+1);
								
								simpleMoveSuccesors.add(n);
								numberOfSimpleMoves++;
							}
							//Jump diagonally backward right.
							if((i-1) >= 0 && (j-1) >= 0 && (boardState[i-1][j-1] == 'A' || boardState[i-1][j-1] == 'k'))
							{
								//possible jump
								if((i-2) >= 0 && (j-2) >= 0 && boardState[i-2][j-2] == 'O')
								{
									char[][] newBoardState = new char[8][8];
									for(int l = 0; l < 8; l++)
										newBoardState[l] = boardState[l].clone();
									
									/*
									 * We can only jump if boardState[i-2][j-2] is empty.
									 * newBoardState[i][j] = K
									 * Need to make 3 switches.
									 * newBoardState[i][j] := O
									 * newBoardState[i-1][j-1] := O
									 * newBoardState[i-2][j-2] := K
									 */
									
									newBoardState[i][j] = 'O';
									newBoardState[i-1][j-1] = 'O'; //jumping over
									newBoardState[i-2][j-2] = 'K';
									
									Node n = new Node(newBoardState);
									n.move.setFromPosition(i, j);
									n.move.setJumpOverPosition(i-1, j-1);
									n.move.setToPosition(i-2, j-2);
									
									jumpSuccesors.add(n);
									numberOfJumpMoves++;
								}
							}
							//Jump diagonally backward left.
							if((i-1) >= 0 && (j+1) < 8 && (boardState[i-1][j+1] == 'A' || boardState[i-1][j+1] == 'k'))
							{
								//possible jump
								if((i-2) >= 0 && (j+2) < 8 && boardState[i-2][j+2] == 'O')
								{
									char[][] newBoardState = new char[8][8];
									for(int l = 0; l < 8; l++)
										newBoardState[l] = boardState[l].clone();
									
									/*
									 * We can only jump if boardState[i-2][j+2] is empty.
									 * newBoardState[i][j] = K
									 * Need to make 3 switches.
									 * newBoardState[i][j] := O
									 * newBoardState[i-1][j+1] := O
									 * newBoardState[i-2][j+2] := K
									 */
									
									newBoardState[i][j] = 'O';
									newBoardState[i-1][j+1] = 'O'; //jumping over
									newBoardState[i-2][j+2] = 'K';
									
									Node n = new Node(newBoardState);
									n.move.setFromPosition(i, j);
									n.move.setJumpOverPosition(i-1, j+1);
									n.move.setToPosition(i-2, j+2);
									
									jumpSuccesors.add(n);
									numberOfJumpMoves++;
								}
							}
							//Jump diagonally forward right.
							if((j-1) >= 0 && (i+1) < 8 && (boardState[i+1][j-1] == 'A' || boardState[i+1][j-1] == 'k'))
							{
								//possible jump
								if((j-2) >= 0 && (i+2) < 8 && boardState[i+2][j-2] == 'O')
								{
									char[][] newBoardState = new char[8][8];
									for(int l = 0; l < 8; l++)
										newBoardState[l] = boardState[l].clone();
									
									/*
									 * We can only jump if boardState[i+2][j-2] is empty.
									 * newBoardState[i][j] = K
									 * Need to make 3 switches.
									 * newBoardState[i][j] := O
									 * newBoardState[i+1][j-1] := O
									 * newBoardState[i+2][j-2] := K
									 */
									
									newBoardState[i][j] = 'O';
									newBoardState[i+1][j-1] = 'O'; //jumping over
									newBoardState[i+2][j-2] = 'K';
									
									Node n = new Node(newBoardState);
									n.move.setFromPosition(i, j);
									n.move.setJumpOverPosition(i+1, j-1);
									n.move.setToPosition(i+2, j-2);
									
									jumpSuccesors.add(n);
									numberOfJumpMoves++;
								}
							}
							//Jump diagonally forward left
							if((i+1) < 8 && (j+1) < 8 && (boardState[i+1][j+1] == 'A' || boardState[i+1][j+1] == 'k'))
							{
								//possible jump
								if((i+2) < 8 && (j+2) < 8 &&  boardState[i+2][j+2] == 'O')
								{
									char[][] newBoardState = new char[8][8];
									for(int l = 0; l < 8; l++)
										newBoardState[l] = boardState[l].clone();
									
									/*
									 * We can only jump if boardState[i+2][j+2] is empty.
									 * newBoardState[i][j] = K
									 * Need to make 3 switches.
									 * newBoardState[i][j] := O
									 * newBoardState[i+1][j+1] := O
									 * newBoardState[i+2][j+2] := K
									 */
									
									newBoardState[i][j] = 'O';
									newBoardState[i+1][j+1] = 'O'; //jumping over
									newBoardState[i+2][j+2] = 'K';
									
									Node n = new Node(newBoardState);
									n.move.setFromPosition(i, j);
									n.move.setJumpOverPosition(i+1, j+1);
									n.move.setToPosition(i+2, j+2);
									
									jumpSuccesors.add(n);
									numberOfJumpMoves++;
								}
							}
						}
						break;
						default:
					}
				}
			}
		}
	}
	
	public void printBoardState()
	{
		System.out.print(" ");
		
		for(int nPrime=1;nPrime<9;nPrime++)
			System.out.print(" "+nPrime);
				
		System.out.println();
			
		for(int x=0;x<8;x++)
		{
			System.out.print(x+1);
			for(int k=0;k<8;k++)
			{
				System.out.print(" "+boardState[x][k]);
			}
			System.out.print("\n");
		}
		System.out.print("\n");
	}
}
