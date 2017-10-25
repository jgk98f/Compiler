/**
 * AUthor: Jason Klamert
 * Date: 4/21/2017
 * Description: A program to check the static semantics of our program. This utilizes an intermediate parse tree representation. Our semantics shall follow the same as C in many ways. While the tree is being traversed we will perform target generation.
 **/

#include <iostream>
#include <stdio.h>
#include <string>
#include <cstring>
#include <stdlib.h>
#include "semantics.h"
#include "stack.h"
#include "node.h"
#include "token.h"
using namespace std;

/**
 * Static function prototypes.
 **/
static void checkSemantics(node_t *root);
static void scopeCheck(node_t *root);
static void isVarDefined(node_t *root);
static void determineRelationalOperator(node_t *root);
static int getStackDistance(node_t *root);

/**
 * Global static vars for semantic processing and code generation.
 **/
static int currentScope = 0;
static int counter = 0;
static int loopFlag = 0;
static FILE* filePointer;
static int tempVars = 0;

/**
 * AUthor: Jason Klamert
 * Date: 4/21/2017
 * Description: Take in a root and kick off code generation. The breadth of this is done in checkSemantics.
 **/
void generateCode(node_t *root, string targetName)
{
	filePointer = fopen (targetName.c_str(), "w");

    if (filePointer == NULL)
    {
      perror("Could not open file for target generation");
      exit(0);
    }

	checkSemantics(root);
	
	/**
	 * Pop globals off of stack to clean up.
	 **/
	for(int count = 0; count <= getStackSize(); count++)
	{
		fprintf(filePointer, "POP\n");
	}
	
	fprintf(filePointer,"STOP\n");

	/**
	 * Deal with temp vars.
	 **/
	for(int count = 0; count <= tempVars; count++)
	{
		fprintf(filePointer, "X%d 0\n", count);
	}

	fclose(filePointer); 
}

/**
 * AUthor: Jason Klamert
 * Date: 4/26/2017
 * Description: Take in root node and perform a traversal of the tree. While traversing the tree do scope checks and declaration checks. We will also generate the target in this function.
 **/
static void checkSemantics(node_t *root)
{
	/**
	 * Skips a root if null.
	 **/
	if (root == NULL)
	{
		return; 
	}

	/**
	 * Get the token from the node for ease of processing.
	 **/
	token* tk;

	if(root->getToken() != NULL)
	{
		tk = root->getToken();
	}

	/**
	 * Handles the vars node.
	 **/
	if(root->getLabel() == "vars")
	{		
		if(tk->getTokenName() != "")
		{	
			if (tk->getTokenName() == "Id_tk")
			{
				fprintf(filePointer, "PUSH\n");
				scopeCheck(root);
			}
		}
	}

	/**
	 * Handles the block node.
	 **/
	if(root->getLabel() == "block")
	{
		/**
	 	 * Increment to denote a new scope.
	 	 **/
		if(root->getChild1() != NULL)
		{
			currentScope++;
		}
	}
	
	/**
	 * Handles the mvars node.
	 **/
	if(root->getLabel() == "mvars")
	{	
		if(tk->getTokenName() != "")
		{	
			if (tk->getTokenName() == "Id_tk")
			{
				fprintf(filePointer, "PUSH\n");
				scopeCheck(root);
			}
		}
		/**
	 	 * Else, the token label is empty string so mvars must be empty.
	 	 **/
		else
		{
			return;
		}
	}
	
	/**
	 * Handles the mStats node.
	 **/
	if(root->getLabel() == "mStat" && loopFlag == 0)
	{
		if( root->getChild1() == NULL && root->getChild2() == NULL)
		{			
			for(int count = getStackSize() ; count > 0; count--)
			{
				if(currentScope == getStackScope())
				{
					pop();
					fprintf(filePointer, "POP\n");
				}
				else
				{
					break;
				}
			}

			currentScope--;
		}
	}

	/**
	 * Handles the out node.
	 **/	
	if(root->getLabel() == "out")	
	{
		checkSemantics(root->getChild1());

		int currentVars = tempVars;
		tempVars++;

		fprintf(filePointer, "STORE X%d\n", currentVars);
		fprintf(filePointer, "WRITE X%d\n", currentVars);

		return;
	}
	
	/**
	 * Handles the in node.
	 **/
	if(root->getLabel() == "in")	
	{
		isVarDefined(root);	

		int currentVars = tempVars;
		tempVars++;

		fprintf(filePointer, "READ X%d\n", currentVars);
		fprintf(filePointer, "LOAD X%d\n", currentVars);
		fprintf(filePointer, "STACKW %d\n", getStackDistance(root));
				
		return;
	}

	/**
	 * Handles the assign node.
	 **/
	if(root->getLabel() == "assign")	
	{
		checkSemantics(root->getChild1());
	
		isVarDefined(root);	
		
		fprintf(filePointer, "STACKW %d\n", getStackDistance(root));
				
		return;
	}
	
	/**
	 * Handles the ifFunction node.
	 **/
	if(root->getLabel() == "ifFunction")	
	{
		
		int currentVars = tempVars;
		int currentCounter = counter;
		tempVars++;
	
		checkSemantics(root->getChild3());
		fprintf(filePointer, "STORE X%d\n", currentVars);
		checkSemantics(root->getChild1());
		fprintf(filePointer, "SUB X%d\n", currentVars);

		fprintf(filePointer, "LOOP%d: ", currentCounter);
		counter++;
		
		checkSemantics(root->getChild2());
	
		checkSemantics(root->getChild4());
	
		fprintf(filePointer,"\nOUT%d: NOOP\n", currentCounter);	
		
		return;
	}
	
	/**
	 * Handles the loop node.
	 **/
	if(root->getLabel() == "loop")	
	{
		counter++;
		loopFlag++;

		int currentCounter = counter;
	
		fprintf(filePointer, "LOOP%d: ", currentCounter);

		checkSemantics(root->getChild3());

		int currentVars = tempVars;
		tempVars++;

		fprintf(filePointer, "STORE X%d\n", currentVars);

		checkSemantics(root->getChild1());

		fprintf(filePointer, "SUB X%d\n", currentVars);
		counter++;
		checkSemantics(root->getChild2());

		checkSemantics(root->getChild4());

		fprintf(filePointer, "BR LOOP%d \n", currentCounter);
		fprintf(filePointer,"\nOUT%d: NOOP\n", currentCounter);
		
		loopFlag--;

		return;
	}
	
	/**
	 * Handles the relationOperator node.
	 **/
	if(root->getLabel() == "relationalOperator")	
	{
		cout << "Inside RO tk: " << ((root->getToken())->getTokenName()) << endl;
		determineRelationalOperator(root);
		return;
	}

	/**
	 * Handles the expr node.
	 **/
	if(root->getLabel() == "expr")	
	{
		if (tk->getTokenName() != "")
		{
			checkSemantics(root->getChild2());

			int currentVars = tempVars;
			tempVars++;

			fprintf(filePointer, "STORE X%d\n", currentVars);

			checkSemantics(root->getChild1());

			fprintf(filePointer, "MULT X%d\n", currentVars);

			return;
		}
		else
		{
			checkSemantics(root->getChild1());

			return;
		}
	}


	
	/**
	 * Handles the M node.
	 **/
	if(root->getLabel() == "M")	
	{
		if (tk->getTokenName() != "")
		{		
			checkSemantics(root->getChild2());

			int currentVars = tempVars;
			tempVars++;

			fprintf(filePointer, "STORE X%d\n", currentVars);

			checkSemantics(root->getChild1());

			fprintf(filePointer, "DIV X%d\n", currentVars);

			return;
		}
		else
		{
			checkSemantics(root->getChild1());

			return;
		}
	}
	
	/**
	 * Handles the T node.
	 **/
	if(root->getLabel() == "T")	
	{
		/**
	 	 * If T has a token.
	 	 **/
		if(tk->getTokenName() != "")
		{
			checkSemantics(root->getChild2());

			int currentVars = tempVars;
			tempVars++;	
			fprintf(filePointer, "STORE X%d\n", currentVars);

			checkSemantics(root->getChild1());

			if(tk->getTokenName() == "+_tk")
			{
				fprintf(filePointer, "ADD X%d\n", currentVars);
			}
			else if(tk->getTokenName() == "-_tk")
			{
				fprintf(filePointer, "SUB X%d\n", currentVars);
			}
			
			return;
		}
		else
		{
			checkSemantics(root->getChild1());

			return;
		}
	}
	
	/**
	 * Handles the F node.
	 **/
	if(root->getLabel() == "F")	
	{
		if (tk->getTokenName() != "")
		{
			checkSemantics(root->getChild1());
			fprintf(filePointer, "MULT -1\n");

			return;
		}
		else
		{
			checkSemantics(root->getChild1());

			return;
		}

	}
	
	/**
	 * Handles the R node.
	 **/
	if(root->getLabel() == "R")	
	{
		/**
	 	 * If R has a token.
	 	 **/
		if(tk->getTokenName() != "")
		{
			if (tk->getTokenName() == "Id_tk") 
			{
				isVarDefined(root);
				fprintf(filePointer,"STACKR %d\n", getStackDistance(root));

				return;
			}
			/**
			 * Handle number.
			 **/
			else
			{
				fprintf(filePointer,"LOAD %s\n", tk->getTokenWord().c_str());

				return;
			}
		}
		/**
	 	 * Else, handle the expr.
	 	 **/
		else
		{
			checkSemantics(root->getChild1());

			return;
		}
	}
	
	checkSemantics (root->getChild1());
	checkSemantics (root->getChild2());
	checkSemantics (root->getChild3());
	checkSemantics (root->getChild4());

	return;
}

/**
 * AUthor: Jason Klamert
 * Date: 4/21/2017
 * Description: Take in a node and check if the variable has already been declared globally or in the current scope. If not, push it on the stack as it is valid.
 **/
static void scopeCheck(node_t *root)
{
	token* tk = root->getToken();

	/**
	 * If the stack is size 0, then make a stack and push incoming data onto stack.
	 **/
	if(getStackSize() == 0)
	{			
		stack s;
		s.label = tk->getTokenWord();
		s.lineNum = tk->getLineNum();
		s.scope = currentScope;
		
		/**
	 	 * If the variable has not been defined in the scope, push it onto the stack.
	 	 **/
		if (root->getLabel() == "vars" || root->getLabel() == "mvars")
		{
			push(s);
		}
	}
	else
	{
		/**
	 	 * Create a stack, s, and if var is found in current scope or global, throw an error. Otherwise it is valid and push it on stack.
	 	 **/
		stack s;
		s.label = tk->getTokenWord();
		s.lineNum = tk->getLineNum();
		s.scope = currentScope;
		
		bool searchFlag = searchStackAndScope(s);
		
		/**
	 	 * If the variable has been found in the same scope with same label then error. Else we are okay and push onto the stack.
	 	 **/
		if (searchFlag)
		{
			cout << "Semantic Error: Variable " << s.label << " on line " << s.lineNum << " has already been declared in the same scope." << endl;
			exit(1);
		}
		else if(root->getLabel() == "vars" || root->getLabel() == "mvars")
		{
			push(s);
		}
	}

	return;
}

/**
 * AUthor: Jason Klamert
 * Date: 4/21/2017
 * Description: Take in a node and check if the variable has been defined.
 **/
static void isVarDefined(node_t *root)
{
	token* tk = root->getToken();

	if (tk != NULL && tk->getTokenName() == "Id_tk")
	{	
		stack s;
		s.label = tk->getTokenWord();
		s.lineNum = tk->getLineNum();
		s.scope = currentScope;
		
		bool searchFlag = searchStack(s);
		
		if (!searchFlag)
		{
			cout << "Semantic Error: Variable " << s.label << " on line " << s.lineNum << " has not been declared." << endl;
			exit(1);
		}
	}
}

/**
 * AUthor: Jason Klamert
 * Date: 4/26/2017
 * Description: Function to obtain the stack index distance.
 **/
static int getStackDistance(node_t *root)
{
	token* tk = root->getToken();

	stack s;
	s.label = tk->getTokenWord();
	s.lineNum = tk->getLineNum();
	s.scope = currentScope - loopFlag;

	int distance = getStackSize() - getIndex(s);

	if(distance == -1)
	{
		distance == 0;
	}

	return distance;
}

/**
 * AUthor: Jason Klamert
 * Date: 4/26/2017
 * Description: Function to obtain the stack index distance.
 **/
static void determineRelationalOperator(node_t *root)
{
	node_t* temp = NULL;
	token* childTk = NULL;
	temp = root->getChild1();
	token* tk = root->getToken();

	if(temp != NULL)
	{
		childTk = temp->getToken();
	}

	/**
 	* Check if the RO has a child1. If so, see what operator it is and turn the child to contain empty string.
 	**/
	if(childTk->getTokenName() == "=_tk")
	{
		cout << "Inside child = tk." << endl;
		if(tk->getTokenName() == "<<_tk" && childTk->getTokenName() == "=_tk")
		{
			fprintf(filePointer, "BRPOS OUT%d\n",counter - 1);
			childTk->setTokenName("");
		}
		else if (tk->getTokenName() == ">>_tk" && childTk->getTokenName() == "=_tk")
		{
			fprintf(filePointer, "BRNEG OUT%d\n",counter - 1);
			childTk->setTokenName("");
		}
	}
	else if(tk->getTokenName() == "==_tk")
	{
		fprintf(filePointer, "BRPOS OUT%d\n",counter - 1);
		fprintf(filePointer, "BRNEG OUT%d\n",counter - 1);
	}
	else if(tk->getTokenName() == "<<_tk")
	{
		fprintf(filePointer, "BRZPOS OUT%d\n",counter - 1);
	}
	else if(tk->getTokenName() == ">>_tk")
	{
		fprintf(filePointer, "BRZNEG OUT%d\n",counter - 1);
	}
	else if(tk->getTokenName() == "=!_tk")
	{
		fprintf(filePointer, "BRZERO OUT%d\n",counter - 1);
	}
	else if(tk->getTokenName() == "")
	{
		/**
		 * We already ruled that it was either a >>= or <<= so return nothing.
		 **/
		return;
	}
}