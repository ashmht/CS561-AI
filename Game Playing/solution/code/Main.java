

/**
 * @author Karthikeyan S
 *
 */
public class Main 
{
	/**
	 * @param args
	 */
	public static void main (String[] args) 
	{
		String inputFileName = args[0],outputFileName = args[1];
		
		AlphaBetaSearch alphabeta = new AlphaBetaSearch(outputFileName);
		Node n;
		int testCases;
		char[][] cases;
		
		ReadInputCases rCase = new ReadInputCases(inputFileName);
		
		testCases = rCase.getNumberOfTestCases()+1;
		testCases /= 10;
		
		rCase.setScannerParam();
		
		for(int i=1;i<=testCases;i++)
		{
			cases = rCase.getNextCase();
			
			n = new Node(cases);
			
			alphabeta.startSearch(n, i);
		}
		
	}

}
