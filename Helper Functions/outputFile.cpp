#include "outputFile.h"

outputFile::outputFile()
{
	isFileOpen = false;
}

bool outputFile::setFileName(const char* _fileName,bool _Open)
{
	fileName = _fileName;
	bool returnStatus = true;

	if(_Open)
	{
	  	oFile.open(fileName);

		returnStatus = isFileOpen = oFile.is_open();
	}

	return returnStatus;
}
      
bool outputFile::openFile()
{
	oFile.open(fileName);
	isFileOpen = oFile.is_open();
	return isFileOpen;
}

std::ofstream* outputFile::getFileStreamObjectPointer()
{
	return &oFile;
}

bool outputFile::getFileOpenStatus()
{
	return isFileOpen;
}

bool outputFile::isFileGood()
{
	return oFile.good();
}

void outputFile::writeToFile(std::string _data)
{
	if (isFileOpen)
	{
		oFile << _data;
	}
}

void outputFile::writeToFile(char _c)
{
	if (isFileOpen)
	{
		oFile << _c;
	}
}

void outputFile::writeToFile(int _i)
{
	if (isFileOpen)
	{
		std::stringstream ss;
		ss << _i;
		oFile << ss.str();
		ss.str("");
	}
}

void outputFile::addNewLine()
{
	if(isFileOpen)
	{
		oFile << "\n";
	}
}

bool outputFile::closeFile()
{
	if (isFileOpen)
	{
		oFile.close();
		isFileOpen = oFile.is_open();
	}
	
	return !isFileOpen;
}
