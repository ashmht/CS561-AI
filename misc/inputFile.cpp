#include "inputFile.h"

inputFile::inputFile()
{
	isFileOpen = false;
}

bool inputFile::setFileName(const char* _fileName,bool _Open)
{
	fileName = _fileName;
	bool returnStatus = true;

	if(_Open)
	{
	  	iFile.open(fileName);

		returnStatus = isFileOpen = iFile.is_open();
	}

	return returnStatus;
}
      
bool inputFile::openFile()
{
	iFile.open(fileName);
	isFileOpen = iFile.is_open();
	return isFileOpen;
}

std::ifstream* inputFile::getFileStreamObjectPointer()
{
	return &iFile;
}

bool inputFile::getFileOpenStatus()
{
	return isFileOpen;
}

bool inputFile::isFileGood()
{
	return iFile.good();
}

bool inputFile::closeFile()
{
	if (isFileOpen)
	{
		iFile.close();
		isFileOpen = iFile.is_open();
	}
	
	return !isFileOpen;
}
