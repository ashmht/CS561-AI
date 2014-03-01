#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include "inputFile.h"
#include "outputFile.h"

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

using namespace std;
typedef std::string str;

int GameState[8][8];

//Get command line arguments and send it to function.
int main(int argc, char *argv[])
    {
             inputFile iFile;
             outputFile oFile;
             
             str fileName = "test.txt";
             
             oFile.setFileName(fileName.c_str(),true);
             
             oFile.writeToFile("Hello");
             oFile.addNewLine();
             
             oFile.writeToFile('C');
             oFile.addNewLine();

             oFile.writeToFile(5);
             
             oFile.closeFile();

             char xx;
             cin >> xx;
             
    return EXIT_SUCCESS;
    }


