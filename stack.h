#ifndef STACK_H
#define STACK_H

#include <string>
#include "token.h"

/**
 * Our homemade stack structure for storing variable information.
 **/
typedef struct Stack
{
	std::string label;
	int  lineNum;
	int  scope;
} stack;

/**
 * Function declarations to aide in stack operations.
 **/
void push(stack s);
bool searchStackAndScope(stack s);
bool searchStack(stack s);
void pop();
void printStack();
std::string get(int userIndex);
int getStackSize();
int getIndex(stack s);
int getStackScope();

#endif