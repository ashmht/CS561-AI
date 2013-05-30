/**
 * 
 */

/**
 * @author Karthikeyan S
 *
 */
public class Position 
{
	private int fromRow;
	private int fromColumn;
	private int jumpOverRow;
	private int jumpOverColumn;
    private int toRow;
    private int toColumn;

    public Position() 
    {
    	//Empty Constructor
    }
    
    public void setFromPosition(int _row,int _column)
    {
    	this.fromRow = _row;
    	this.fromColumn = _column;
    }
    
    public void setToPosition(int _row,int _column)
    {
    	this.toRow = _row;
    	this.toColumn = _column;
    }
    
    public void setJumpOverPosition(int _row, int _column) 
	{
		this.jumpOverRow = _row;
		this.jumpOverColumn = _column;
	}
    
    public int getFromRow() 
    {
        return fromRow;
    }

    public int getFromColumn()
    {
        return fromColumn;
    }
    
    public int getToRow() 
    {
        return toRow;
    }

    public int getToColumn()
    {
        return toColumn;
    }

	public int getJumpOverColumn() 
	{
		return jumpOverColumn;
	}

	public int getJumpOverRow() {
		return jumpOverRow;
	}
}
