/**
 * Author: Jason Klamert
 * Date: 4/21/2017
 * Description: This program will provide the utility functions for our stack operations. The stack is made using a basic array of 'stack' elements.
 **/

#include "stack.h"
#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
using namespace std;

/**
 * Static globals.
 **/
static int stackIndex = 0;
static stack s[150];

/**
 * Author: Jason Klamert
 * Date: 4/20/2017
 * Description: Function to push a variable onto the stack and move the index.
 **/
void push(stack temp)
{
	s[stackIndex] = temp;
	stackIndex++;
}

/**
 * Author: Jason Klamert
 * Date: 4/20/2017
 * Description: Function to see if the current var is in stack and in current scope. If so, return true. Else, return false.
 **/
bool searchStackAndScope(stack temp)
{
	for(int count = stackIndex - 1; count >= 0; count--)
	{
		if(s[count].label == temp.label && temp.scope == s[count].scope)
		{
			return true;
		}
	}

	return false;
}

/**
 * Author: Jason Klamert
 * Date: 4/20/2017
 * Description: Function to see if the current var is in stack. If so, return true. Else, return false.
 **/
bool searchStack(stack temp)
{
	for(int count = stackIndex - 1; count >= 0; count--)
	{
		if( s[count].label == temp.label ) 
		{
			return true;
		}
	}
	
	return false;
}

/**
 * Author: Jason Klamert
 * Date: 4/20/2017
 * Description: Function to return stack index of var.
 **/
int getIndex(stack temp)
{
	for(int count = stackIndex - 1; count >= 0; count--)
	{
		if( (s[count].label == temp.label && s[count].scope == temp.scope) || (s[count].label == temp.label && s[count].scope == 0) )
		{
			//cout << "getIndex: " << count << endl;
			return count;
		}
	}
}

/**
 * Author: Jason Klamert
 * Date: 4/20/2017
 * Description: Function to pop a var off the stack.
 **/
void pop()
{
	stackIndex--;
}

/**
 * Author: Jason Klamert
 * Date: 4/20/2017
 * Description: Function to print the current stack starting from index 0.
 **/
void printStack()
{
	for(int count = 0; count < stackIndex; count++)
	{
		cout << s[count].label << ":" << s[count].scope << endl;
	}
}

/**
 * Author: Jason Klamert
 * Date: 4/20/2017
 * Description: Function to return the current stack size.
 **/
int getStackSize()
{
	return stackIndex - 1;
}

/**
 * Author: Jason Klamert
 * Date: 4/20/2017
 * Description: Function to return element's label at the passed index.
 **/
string get(int userIndex)
{
	return s[userIndex].label; 
}

/**
 * Author: Jason Klamert
 * Date: 4/20/2017
 * Description: Function to return stack scope.
 **/
int getStackScope()
{
	return s[stackIndex - 1].scope;
}