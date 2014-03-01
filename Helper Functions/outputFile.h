#ifndef OUTPUTFILE_H
#define OUTPUTFILE_H

#include <fstream>
#include <sstream>

/*
  Input file Class
*/
class outputFile
{
  private:
  	/*
		Input filestream object
  	*/
      std::ofstream oFile;

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
      outputFile();
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
      Write data to file String input
    */
      void writeToFile(std::string _data);

    /*
      Write data to file Char input
    */
      void writeToFile(char _c);

    /*
      Write data to file Int input
    */
      void writeToFile(int _i);

    /*
      Add a new line after the current data to the file.
    */
      void addNewLine();

    /*
		  Return the fileStream object associated with the file.
    */
      std::ofstream* getFileStreamObjectPointer();

    /*
		  Close the file.
    */
      bool closeFile();
};

#endif
