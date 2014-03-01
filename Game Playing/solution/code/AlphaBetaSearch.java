/**
 * 
 */

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.Writer;
import java.util.ArrayList;

/**
 * @author Karthikeyan Sreenivasan
 *
 */
public class AlphaBetaSearch 
{
	/*
	 * Data Members
	 */
	Writer output = null;
	File outputFile;
	
	/*
	 * Member Functions
	 */
	public AlphaBetaSearch(String _fileName)
	{
		outputFile = new File(_fileName);
		
		try 
		{
			output = new BufferedWriter(new FileWriter(outputFile));
		} 
		catch (IOException e) 
		{
			e.printStackTrace();
		}
	}
	
	public void printCaseNumber(int i)
	{
		try {
			output.write("\ncase"+i+"\n");
			output.flush();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	public void startSearch(Node n,int caseNumber)
	{
		printCaseNumber(caseNumber);
		
		Search(n,4,Integer.MIN_VALUE,Integer.MAX_VALUE,true);
		
		printFirstMove(n);
	}
	
	//Algorithm: http://en.wikipedia.org/wiki/Alpha-beta_pruning
	public int Search(Node n,int depth,int alpha,int beta,boolean player)
	{
		try{
			int i,k;
			boolean pruned = false;
			ArrayList<Node> successor;
			
			//Generate successors moves first.
			n.computeSuccesorsOfNode(player);
			
			//Jump takes precedence over simple moves.
			if(n.jumpSuccesors.isEmpty())
			{
				successor = n.simpleMoveSuccesors;
			}
			else
			{
				successor = n.jumpSuccesors;
			}
			
			//Maximizer
			if(player)
			{
				i = 4 - (depth-1);
				
				Node firstMoveNode = new Node();
				int v = Integer.MIN_VALUE;
				
				//Check if there are any successors.
				//If not return utility value.
				if(successor.isEmpty())
				{
					for(int jj=i-1;jj>0;jj--)
						output.write("|-");
					
					int uValue = n.getUtilityValue(player,false);
					output.write("A"+(i-1)+": No possible moves left. h="+uValue+".\n");
					output.flush();
					return uValue;
				}
				
				//Iterate through each successor.
				for(k=0;k<successor.size();k++)
				{
					Node _n = successor.get(k);
					
					if(depth == 0)
					{
						for(int jj=i-2;jj>0;jj--)
							output.write("|-");
						
						int uValue = n.getUtilityValue(player,true);
						output.write("B"+(i-1)+": ("+n.move.getFromRow()+","+n.move.getFromColumn()+")=>("+n.move.getToRow()+","+n.move.getToColumn()+"); h="+uValue+".\n");
						output.flush();
						return uValue;
					}
					else
					{
						for(int jj=i-1;jj>0;jj--)
							output.write("|-");
						
						output.write("A"+i+": ("+_n.move.getFromRow()+","+_n.move.getFromColumn()+")=>("+_n.move.getToRow()+","+_n.move.getToColumn()+").\n");
					}
					alpha = Math.max(alpha, Search(_n,depth-1,alpha,beta,!player));
					
					if(alpha > v)
					{
						v = alpha;
						firstMoveNode = _n;
					}
					
					if(beta <= alpha)
					{
						pruned = true;
						
						break;
					}
				}
				
				if(pruned)
				{
					if(successor.size() != 1)
					{
						for(int jj=i-1;jj>0;jj--)
							output.write("|-");
						
						output.write("A"+i+": Pruning ");
								
						for(int j=k+1;j<successor.size();j++)
						{
							Node nx = successor.get(j);
							output.write("("+nx.move.getFromRow()+","+nx.move.getFromColumn()+")=>("+nx.move.getToRow()+","+nx.move.getToColumn()+")");
							if(j != successor.size()-1)
							{
								output.write(", ");
							}
						}
						
						output.write("; alpha="+alpha+", beta="+beta+".\n");
					}
				}
				
				
				output.flush();
				
				//Set the max node as start move.
				n.startMove = firstMoveNode.move;
				
				return alpha;
			}
			//Minimizer
			else
			{
				Node firstMoveNode = new Node();
				int v = Integer.MIN_VALUE;
				
				i = 4 - (depth-1);
				
				//Check if there are any successors.
				//If not return utility value.
				if(successor.isEmpty())
				{
					for(int jj=i-1;jj>0;jj--)
						output.write("|-");
					
					int uValue = n.getUtilityValue(player,false);
					output.write("B"+(i)+": No possible moves left. h="+uValue+".\n");
					output.flush();
					return uValue;
				}
				
				//Iterate through each successor.
				for(k=0;k<successor.size();k++)
				{
					Node _n = successor.get(k);
					
					if(i != 4)
					{
						for(int jj=i-1;jj>0;jj--)
							output.write("|-");
						
						if(depth == 0)
						{
							int uValue = n.getUtilityValue(player,true);
							output.write("A"+(i-1)+": ("+n.move.getFromRow()+","+n.move.getFromColumn()+")=>("+n.move.getToRow()+","+n.move.getToColumn()+"); h="+uValue+".\n");
							output.flush();
							return uValue;
						}
						else
						{
							output.write("B"+i+": ("+_n.move.getFromRow()+","+_n.move.getFromColumn()+")=>("+_n.move.getToRow()+","+_n.move.getToColumn()+").\n");
						}
					}
					beta = Math.min(beta,Search(_n, depth-1, alpha, beta, !player));
					
					if(beta >= v)
					{
						v = alpha;
						firstMoveNode = _n;
					}
					
					if(beta <= alpha)
					{
						pruned = true;
						
						break;
					}
				}
				
				if(pruned)
				{
					if(successor.size() != 1)
					{
						for(int jj=i-1;jj>0;jj--)
							output.write("|-");
						
						output.write("B"+i+": Pruning ");
								
						for(int j=k+1;j<successor.size();j++)
						{
							Node nx = successor.get(j);
							output.write("("+nx.move.getFromRow()+","+nx.move.getFromColumn()+")=>("+nx.move.getToRow()+","+nx.move.getToColumn()+")");
							if(j != successor.size()-1)
							{
								output.write(", ");
							}
						}
						
						output.write("; alpha="+alpha+", beta="+beta+".\n");
					}
				}
				
				output.flush();
				
				//Set the max node as start move.
				n.startMove = firstMoveNode.move;
				
				return beta;
			}
		}
		catch(IOException e)
		{
			e.printStackTrace();
			return 0;
		}
	}
	
	public void printFirstMove(Node n)
	{
		try 
		{
			output.flush();
			output.write("\nfirst move: ("+n.startMove.getFromRow()+","+n.startMove.getFromColumn()+") => ("+n.startMove.getToRow()+","+n.startMove.getToColumn()+").\n");
			output.flush();
		} 
		catch (IOException e) 
		{
			e.printStackTrace();
		}
	}
	
	protected void finalize() throws Throwable 
	 { 
		 output.close();
		 super.finalize();
	 }
}
