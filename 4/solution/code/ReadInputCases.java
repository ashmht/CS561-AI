/**
 * 
 */

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.io.LineNumberReader;
import java.util.Scanner;

public class ReadInputCases 
{
	/*
	 * Data Members
	 */
	private final File inputFile;
	char[][] initialBoardState;
	Scanner scanner;
	FileReader fReader;
	int caseNumber=0;
	
	/*
	 * Constructors and Member Functions
	 */
	public ReadInputCases(String fileName)
	{
		inputFile = new File(fileName);
		initialBoardState = new char[8][8];
	}
	
	public final void setScannerParam()
	{
		try
		{
			//FileReader is used and not File, as File is not Closeable
			fReader = new FileReader(inputFile);
			scanner = new Scanner(fReader);
			scanner.useDelimiter("\\n");
		}
		catch(FileNotFoundException e)
		{
			log(e);
		}
	}
	
	public int getNumberOfTestCases()
	{
		try 
		{
			fReader = new FileReader(inputFile);
		} 
		catch (FileNotFoundException e1) 
		{
			log(e1);
		}
		LineNumberReader  lnr = new LineNumberReader(fReader);
		try 
		{
			lnr.skip(Long.MAX_VALUE);
		} 
		catch (IOException e) 
		{
			log(e);
		}
		
		return lnr.getLineNumber();
	}
	
	public char[][] getNextCase()
	{
		int lineNumber =0;
		//First use a Scanner to get each line
		while(scanner.hasNextLine())
		{
			if(lineNumber > 7)
			{
				break;
			}
			
			String s = scanner.next().trim();
			if(s.equalsIgnoreCase("case "+(caseNumber+1)) || s == null || s.equalsIgnoreCase(""))
			{
				//Do Nothing, we are going to read the corresponding case i
			}
			else
			{
				processString(s,lineNumber);
				lineNumber++;
			}
		}
		
		caseNumber++;
		return initialBoardState;
	}

	/* 
	  * Overridable method for processing lines in different ways. 
	*/
	protected void processString(String aLine,int row)
	{
			char[] rowData;
			rowData = aLine.toCharArray();
			//System.out.println("rowData: "+aLine);
			//System.out.println("Size of rowData: "+rowData.length);
			if(rowData.length <= 0)
			{
				return;
			}
			for(int j=0;j<8;j++)
			{
				initialBoardState[row][j] = rowData[j];
			}
	}
	
	private static void log(Object aObject){
	    System.out.println(String.valueOf(aObject));
	  }
	
	//Destructor
	 protected void finalize() throws Throwable 
	 { 
		 scanner.close();
		 super.finalize();   // Always call parent's finalizer  
	 }
}
