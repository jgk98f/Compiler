#include "token.h"
#include <string>
using namespace std;

/**
 * Author: Jason Klamert
 * Date: 2/25/2017
 * Constructor for the token class.
 **/
token::token()
{
	tokenId = 0;
	tokenName = "";
	lineNum = 0;
	tokenWord = "";
}

/**
 * Author: Jason Klamert
 * Date: 2/25/2017
 * Overloaded constructor for the token class.
 **/
token::token(int id, string name, int lineNumber, string word)
{
   	tokenId = id;
    tokenName = name;
    lineNum = lineNumber;
    tokenWord = word;
}

/**
 * Author: Jason Klamert
 * Date: 2/25/2017
 * Functions for setting the class members.
 **/
void token::setTokenId(int id)
{
	tokenId = id;
}

/**
 * Author: Jason Klamert
 * Date: 2/25/2017
 * Functions for setting the class members.
 **/
void token::setTokenName(string name)
{
	tokenName = name;
}

/**
 * Author: Jason Klamert
 * Date: 2/25/2017
 * Functions for setting the class members.
 **/
void token::setLineNum(int n)
{
	lineNum = n;
}

/**
 * Author: Jason Klamert
 * Date: 2/25/2017
 * Functions for setting the class members.
 **/
void token::setTokenWord(string s)
{
	tokenWord = s;
}


/**
 * Author: Jason Klamert
 * Date: 2/25/2017
 * Functions for getting the class members.
 **/
int token::getTokenId()
{
	return tokenId;
}

/**
 * Author: Jason Klamert
 * Date: 2/25/2017
 * Functions for getting the class members.
 **/
string token::getTokenName()
{
	return tokenName;
}

/**
 * Author: Jason Klamert
 * Date: 2/25/2017
 * Functions for getting the class members.
 **/
int token::getLineNum()
{
	return lineNum;
}

/**
 * Author: Jason Klamert
 * Date: 2/25/2017
 * Functions for getting the class members.
 **/
string token::getTokenWord()
{
	return tokenWord;
}