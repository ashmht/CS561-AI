#ifndef INPUTFILE_H
#define INPUTFILE_H

#include <fstream>

/*
  Input file Class
*/
class inputFile
{
  private:
  	/*
		Input filestream object
  	*/
      std::ifstream iFile;

	/*
  		Name of the file
	*/
      const char* fileName;

  /*
      Open status of the file
  */
      bool isFileOpen;
  public:
    /*
      Constructor
    */
      inputFile();
  	/*
  		Sets the name of the file to _fileName
  		Opens the file if _Open = True
  	*/
      bool setFileName(const char* _fileName,bool _Open);
    
    /*
		  Open the file
    */
      bool openFile();

    /*
      Check if file is open.
    */
      bool getFileOpenStatus();

    /*
      Check if the file is good.
    */
      bool isFileGood();

    /*
		  Return the fileStream object associated with the file.
    */
      std::ifstream* getFileStreamObjectPointer();

    /*
		  Close the file.
    */
      bool closeFile();
};

#endif
