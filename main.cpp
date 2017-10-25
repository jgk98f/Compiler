
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include "scanner.h"
#include "token.h"
#include "parser.h"
#include "buildTree.h"
#include "node.h"
#include "semantics.h"
using namespace std;

static void throwError(string fileName);

int main(int argc, char** argv)
{

  //Variable Declarations
  string fileName = "out";
  string baseFile = "out";
  FILE* filePointer = NULL;
  char tempName[50];

  //If no arg, use stdin. If one arg use file name in argv[1]. Else, improper usage.
  if(argc == 1)
  {

    filePointer = stdin;
    string line;
    tmpnam(tempName);
    filePointer = fopen( tempName, "w");

    if(filePointer)
    {
      readIntoBuffer(filePointer);
      while (getline(cin,line))
      {
        fputs(line.c_str(), filePointer);
      }
    }

    fclose(filePointer);
    filePointer = fopen( tempName, "r");

  }
  else if(argc == 2)
  {

    fileName = baseFile = argv[1];
    fileName.append(".4280E01");
    filePointer = fopen (fileName.c_str() , "r");

  }
  else
  {

    cout << "You have used this program improperly." << endl;
    cout << "Please use either no arguments or exactly one argument and the program will prompt you for the proper information if it needs any." << endl;
    cout << "With no arguments the program will have you read in some data from the keyboard to create the tree." << endl;
    cout << "With one argument the user will pass a file name to the program and that file must include source code to scan." << endl;

    return 1;
  }

  //If successful, build the tree. Else, use perror.
  if(filePointer)
  {
    readIntoBuffer(filePointer);
    cout << "Starting Parsing." << endl;
    node_t* root = parser();
    //printParseTree(root,0);
    generateCode(root, baseFile + ".asm");
    cout << "The semantic check and target generation was successful!" << endl;
    free(root);
  }
  else
  {
    throwError(fileName);
  }
  
  return 0;

}

/**
 * Author: Jason Klamert
 * Date: 2/4/2017
 * Function that throws the invalid character error and exits with -1.
 **/
static void throwError(string fileName)
{
    cout << "Scanner Error: Could Not Open Source File: " << fileName << "!";
    exit(-3);
}