#include <iostream>
#include <stdio.h>
#include <string>
#include <cstring>
#include <stdlib.h>
#include <cctype>
#include <algorithm>
#include "scanner.h"
#include "token.h"
using namespace std;

/**
 * The DFA for token mapping with states.
 **/
static int dfa[6][25] =
{ // State   L      D     =    !    <     >     :      +    -     *     /     &     %     .     (      )     {     }     ;    [     ]      ,    WS    EOF
      {0,    1,    2,     3,  -1,   4,    5,   1008, 1009, 1010, 1011, 1012, 1013, 1014, 1015, 1016, 1017, 1018, 1019, 1020, 1021, 1022, 1023,    0, 1024},
      {1,    1,  1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1024},
      {2,  1001,   2,  1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1024},
      {3,  1003, 1003, 1005, 1004, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1024},
      {4,   -1,   -1,    -1,  -1,  1007,  -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  1024},
      {5,   -1,   -1,    -1,  -1,   -1,   1006, -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  1024},
};


static string KEYWORDS[10] = {"start", "stop", "decision", "while", "void", "int", "return", "read", "print", "program"};
static string DELIMETERS[9] = { ".", "(",  ")", ",", "{", "}", ";", "[", "]"};
static string RELATIONAL[4] ={ "==", "<", ">", "=!"};
static string OPERATORS[10] = {"<<", ">>", "=", ":", "+", "-", "*", "/", "%", "&"};

//5 tokens per line. Excluding keyword token names.
static string tokenName[24] = {
                            "Id_tk", "Num_tk", "=_tk", "=!_tk", "==_tk", 
                            ">>_tk", "<<_tk", ":_tk", "+_tk", "-_tk",
                            "*_tk", "/_tk", "&_tk", "%_tk", "._tk", 
                            "(_tk",")_tk", "{_tk", "}_tk", ";_tk",  
                            "[_tk", "]_tk", ",_tk",  "EOF_tk"
                            };

//Same index may be used to retrieve the tokenName and its int mapping.
static int tokenID[24] = {
                        1000, 1001, 1003, 1004, 1005, 
                        1006, 1007, 1008, 1009, 1010, 
                        1011, 1012, 1013, 1014, 1015, 
                        1016, 1017, 1018, 1019, 1020, 
                        1021, 1022, 1023, 1024
                        };

//We have 10 key word tokens in all. 5 per line.
static string keywordTokenName[10] = {
                                    "start_tk", "stop_tk", "decision_tk", "while_tk", "void_tk", 
                                    "int_tk", "return_tk", "read_tk", "print_tk", "program_tk"
                                    };

//Same index can be used to obtain both the keyword and its int mapping.
static int keywordTokenId[10] = {
                        1100, 1101, 1103, 1104, 1105, 
                        1106, 1107, 1108, 1109, 1110
                        };

static unsigned int position = 0;
string buffer;
static int lineCount = 1;
static int state = 0; /* (0=s1 here) */
static int numberCharacters = 0;
static string word = "";

/**
 * Function Prototypes for static functions!
 **/
static int getAscii(char c);
static token* lookupKeyword(string tok, int line);
static void throwError(token* tok);
static void throwError(char c, int line);
static bool isValid(char c, int line);
static int mapCharToCol(char c);
static string getTokenNameFromId(int code);
static bool isAlpha(char c);
static bool isDigit(char c);

/**
 * Author: Jason Klamert
 * Date: 2/26/2017
 * Function that reads the file into the buffer.
 **/
void readIntoBuffer(FILE* file)
{
	char c;
	string s;

    while ((c = getc(file)) != EOF)
    {
        s = s + c;
    }

	buffer = s;
}

/**
 * Author: Jason Klamert
 * Date: 2/26/2017
 * Function that produces one token for the parser to consume.
 **/
token* getToken()
{

    	token* tok = new token();

		string temp = buffer;

		if(position >= temp.size())
		{
			tok->setTokenId(1024);
    		tok->setTokenName(getTokenNameFromId(1024));
    		tok->setLineNum(lineCount);
    		tok->setTokenWord("EOF");
    		return(tok);
		}
  
        while ( position < temp.size() )
        { 
            //We have hit a comment character if this occurs.
            //Skip the rest of the line and process a new line.
            if(temp[position] == '#')
            {
            	//Read through the comment line.
                while( temp[position] != '\n' && position < temp.size() )  
                {
                	position++;
                }

                if(position >= temp.size())
				{
					tok->setTokenId(1024);
    				tok->setTokenName(getTokenNameFromId(1024));
    				tok->setLineNum(lineCount);
    				tok->setTokenWord("EOF");
    				return(tok);
				}
            }

            if(isValid(temp[position], lineCount))
            {

                if(getAscii(temp[position]) == 10)
                {
                    lineCount++;
                    position++;

                    if(position >= temp.size())
                    {
                        tok->setTokenId(1024);
                        tok->setTokenName(getTokenNameFromId(1024));
                        tok->setLineNum(lineCount);
                        tok->setTokenWord("EOF");
                        return(tok);
                    }
                }

                //Ignore tab for it is merely for styling and ignore carriage return.
                if(temp[position] != '\t' && getAscii(temp[position]) != 13 && temp[position] != '\r' && temp[position] != '\n' )
                {
                    word = word + temp[position];
                    
                    // trim trailing spaces and tabs
                    size_t endpos = word.find_last_not_of(" \t\n");
                    if( string::npos != endpos )
                    {
                        word = word.substr( 0, endpos + 1 );
                    }

                    //trim leading whitespaces and tabs
                    size_t startpos = word.find_first_not_of(" \t\n");
                    if( string::npos != startpos )
                    {
                        word = word.substr( startpos );
                    }

                    state = dfa[state][ mapCharToCol( temp[position] ) ];

                    if (state == -1)
                    {
                        tok->setTokenId(state);
                        tok->setTokenName("Invalid Token");
                        tok->setLineNum(lineCount);
                        tok->setTokenWord(word);
                        
                        throwError(tok);
                    }

                    if (state == 1000) // need reserved keyword lookup
                    {
                        tok = lookupKeyword(word, lineCount);

                        word = "";
                        numberCharacters = 0;
                        state = 0;
                        position++;
                    	return tok;                        
                    }
                    else if (state > 1000)
                    {
                        tok->setTokenId(state);
                        tok->setTokenName(getTokenNameFromId(state));
                        tok->setLineNum(lineCount);
                        tok->setTokenWord(word);

                        word = "";
                        numberCharacters = 0;
                        state = 0;
                        position++;
                    	return tok;
                    }
                    else /* not final */
                    {
                        if(getAscii(temp[position]) != 32 && getAscii(temp[position]) != 10)
                        {
                            //Our look ahead that is required only for the id_tk and num_tk.
                            if(!(isDigit(temp[position + 1])) && state == 2)
                            {
                                tok->setTokenId(1001);
                                tok->setTokenName(getTokenNameFromId(1001));
                                tok->setLineNum(lineCount);
                                tok->setTokenWord(word);

                                word = "";
                                numberCharacters = 0;
                                state = 0;
                                position++;
                                return tok;
                            }

                            if(!(isAlpha(temp[position + 1])) && state == 1)
                            {
                                tok = lookupKeyword(word, lineCount);

                                word = "";
                                numberCharacters = 0;
                                state = 0;
                                position++;
                                return tok;
                            }

                            if(numberCharacters > 8)
                            {
                                cout << "Scanner Error: Greater than 8 characters in a digit or identifier!" << endl;
                                cout << "We shall exit with status -5!" << endl;

                                exit(-5);
                            }

                            numberCharacters++;
                        }
                        else
                        {
                            if((position + 1) >= temp.size())
                            {
                                tok->setTokenId(1024);
                                tok->setTokenName(getTokenNameFromId(1024));
                                tok->setLineNum(lineCount);
                                tok->setTokenWord("EOF");
                                return(tok);
                            }
                        }

                    }
                }
            }
            else
            {
                throwError(temp[position], lineCount);
            }

            position++;

        }
}

/**
 * Author: Jason Klamert
 * Date: 2/25/2017
 * Function that converts a char to ascii.
 **/
static int getAscii(char c)
{
    int n = c;

    return n;
}

/**
 * Author: Jason Klamert
 * Date: 2/26/2017
 * Function that looks up the ID to see if it is an identifier.
 **/
static token* lookupKeyword(string tok, int line)
{
    token* t = new token();

    for(int i = 0; i < 10; i++)
    {
        if(KEYWORDS[i] == tok)
        {
            t->setTokenId(keywordTokenId[i]);
            t->setTokenName(keywordTokenName[i]);
            t->setLineNum(line);
            t->setTokenWord(tok);

            return t;
        }
    }

    //If found as keyword the function never reaches here so we
    //know it is identifier.
    t->setTokenId(1000);
    t->setTokenName(tokenName[0]);
    t->setLineNum(line);
    t->setTokenWord(tok);

    return t;
}

/**
 * Author: Jason Klamert
 * Date: 2/26/2017
 * Function to find if character is Digit. Return true or false.
 **/
static bool isDigit(char c)
{
    int asciiCode = getAscii(c);

    if(asciiCode > 47 && asciiCode < 58)
    {
        return true;
    }
    else
    {
        return false;
    }
}

/**
 * Author: Jason Klamert
 * Date: 2/26/2017
 * Function to find if character is letter. Return true or false.
 **/
static bool isAlpha(char c)
{
    int asciiCode = getAscii(c);

    if((asciiCode > 64 && asciiCode < 91) || (asciiCode > 96 && asciiCode < 123))
    {
        return true;
    }
    else
    {
        return false;
    }
}

/**
 * Author: Jason Klamert
 * Date: 2/26/2017
 * Function that maps a given char to the correct column number for the FSA table.
 **/
static int mapCharToCol(char c)
{

    int asciiCode = getAscii(c);

    if((asciiCode > 64 && asciiCode < 91) || (asciiCode > 96 && asciiCode < 123)) 
    {
        return 1;
    }
    else if(asciiCode > 47 && asciiCode < 58)
    {
        return 2;
    }
    else if(asciiCode == 61)
    {
        return 3;
    }
    else if(asciiCode == 33)
    {
        return 4;
    }
    else if(asciiCode == 60)
    {
        return 5;
    }
    else if(asciiCode == 62)
    {
        return 6;
    }
    else if(asciiCode == 58)
    {
        return 7;
    }
    else if(asciiCode == 43)
    {
        return 8;
    }
    else if(asciiCode == 45)
    {
        return 9;
    }
    else if(asciiCode == 42)
    {
        return 10;
    }
    else if(asciiCode == 47)
    {
        return 11;
    }
    else if(asciiCode == 38)
    {
        return 12;
    }
    else if(asciiCode == 37)
    {
        return 13;
    }
    else if(asciiCode == 46)
    {
        return 14;
    }
    else if(asciiCode == 40)
    {
        return 15;
    }
    else if(asciiCode == 41)
    {
        return 16;
    }
    else if(asciiCode == 123)
    {
        return 17;
    }
    else if(asciiCode == 125)
    {
        return 18;
    }
    else if(asciiCode == 59)
    {
        return 19;
    }
    else if(asciiCode == 91)
    {
        return 20;
    }
    else if(asciiCode == 93)
    {
        return 21;
    }
    else if(asciiCode == 44)
    {
        return 22;
    }
    else if(asciiCode == 32)
    {
        return 23;
    }
    else if(asciiCode < 0)
    {
        return 24;
    }
    else if(asciiCode == 10)
    {
        return 0;
    }
    else if(asciiCode == 13)
    {
        return 0;
    }
    else
    {
        cout << "Scanner Error: Character " << c << " made it past the validity check!" << endl;
        cout << "We shall exit with status -4!" << endl;

        exit(-4);
    }
}

/**
 * Author: Jason Klamert
 * Date: 2/26/2017
 * Function that find the token name from the token code.
 **/
static string getTokenNameFromId(int code)
{
    for(int i = 0; i < 24; i++)
    {
        if(tokenID[i] == code)
        {
            return tokenName[i];
        }
    }
}

/**
 * Author: Jason Klamert
 * Date: 2/26/2017
 * Function that throws the invalid character error and exits with -1.
 **/
static void throwError(char c, int line)
{
    cout << "Scanner Error: Invalid Character " << c << " found at line " << line << "!" << endl;
    cout << "Its ascii code is " << (int) c << "!" << endl;
    cout << "The program will abort now!" << endl;

    exit(-1);
}

/**
 * Author: Jason Klamert
 * Date: 2/26/2017
 * Function that throws the invalid character error and exits with -1.
 **/
static void throwError(token* tok)
{
    cout << tok->getTokenName() << " with token word " << tok->getTokenWord() << " found at line " << tok->getLineNum() << " with token Id " << tok->getTokenId() << "!" << endl;
    cout << "The program will abort now!" << endl;

    exit(-2);
}

/**
 * Author: Jason Klamert
 * Date: 2/26/2017
 * Function that compares a char's ascii value to see if it is valid. If not the program will abort with an invalid alphabet error.
 **/
static bool isValid(char c, int line)
{
    if(c == '\n')
    {
        return true;
    }

    int asciiCode = getAscii(c);

    if( (asciiCode >= 40 && asciiCode <= 62) || 
        asciiCode == 32 || 
        asciiCode == 33 || 
        asciiCode == 35 || 
        asciiCode == 37 || 
        asciiCode == 38 || 
        (asciiCode <= 65 && asciiCode >= 93) || 
        (asciiCode >= 97 && asciiCode <= 123) || 
        asciiCode == 125 ||
        asciiCode == 10 ||
        asciiCode == 35 ||
        asciiCode == 9 ||
        asciiCode == 91 ||
        asciiCode == 93 ||
        asciiCode == 13)
    {
        return true;
    }
    else
    {
        throwError(c, line);
        exit(-1);
    }
    
}


