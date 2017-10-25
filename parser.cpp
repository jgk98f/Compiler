
/**
 * Author: Jason Klamert
 * Date: 3/28/2017
 * Description: Parser to verify that the programs grammar is correct.
 **/
#include <iostream>
#include <stdio.h>
#include <string>
#include <cstring>
#include <stdlib.h>
#include "parser.h"
#include "buildTree.h"
#include "token.h"
#include "scanner.h"
#include "node.h"
using namespace std;

static node_t* program();
static node_t* block();
static node_t* vars();
static node_t* mvars();
static node_t* expr();
static node_t* M();
static node_t* T();
static node_t* F();
static node_t* R();
static node_t* stats();
static node_t* mStat();
static node_t* stat();
static node_t* in();
static node_t* out();
static node_t* ifFunction();
static node_t* loop();
static node_t* assign();
static node_t* relationalOperator();
static void error(string errorString);
static node_t* X();
static node_t* Y();

/**
 * Control construct for if mStat -> Empty | stat mStat
 **/
static bool isMstatEmpty = false;

/**
 * Token variable to be used to store current token from scanner.
 **/
static token* tk;

/**
 * Author: Jason Klamert
 * Date: 3/28/2017
 * Description: This is the auxilary function to kick off the recursive decent parsing.
 **/
node_t* parser()
{
	node_t* root;
	tk = getToken();
	root = program();
	if (tk->getTokenName() == "EOF_tk")
	cout << "The parse was successful. EOF_tk was the last token!" << endl;
	else error("Parser reached the end but there was an error in parsing the program. No recovery can be made."); // error message, exit, no recovery
	return root;
}

/**
 * Author: Jason Klamert
 * Date: 3/28/2017
 * Description: This function handles the recursive decent for the Y productions. These result from left factoring the relational operator productions.
 **/
static node_t* Y()
{
	node_t* n = new node_t("Y");
	if(tk->getTokenName() == "=_tk") // <Y> -> =
	{
		n->setToken(tk);
		n->setOperatorIndicator(4);
		tk = getToken();

		return n;
	}
	else
	{
		return n; //Empty production.
	}
}

/**
 * Author: Jason Klamert
 * Date: 3/28/2017
 * Description: This function handles the recursive decent for the X productions. These result from left factoring the relational operator productions.
 **/
static node_t* X()
{
	node_t* n = new node_t("X");
	if(tk->getTokenName() == "=_tk") // <X> -> =
	{
		n->setToken(tk);
		n->setOperatorIndicator(2);
		tk = getToken();

		return n;
	}
	else
	{
		return n; //Empty production.
	}
}

/**
 * Author: Jason Klamert
 * Date: 3/28/2017
 * Description: This function handles the recursive decent for the RO productions.
 * 0 = "==", 1 = "<<", 2 = "<< =", 3 = ">>", 4 = ">> =", 5 = "=!" for the RO indicator.
 **/
static node_t* relationalOperator()
{
	node_t* n = new node_t("relationalOperator");

	if(tk->getTokenName() == "<<_tk") // <R0> -> << X 
	{
		n->setToken(tk);
		n->setOperatorIndicator(1);
		tk = getToken();

		n->setChild1(X());

		return n;
	}
	else if(tk->getTokenName() == ">>_tk")
	{
		n->setToken(tk);
		n->setOperatorIndicator(3);
		tk = getToken();

		n->setChild1(Y());

		return n;
	}
	else if(tk->getTokenName() == "==_tk")
	{
		n->setToken(tk);
		n->setOperatorIndicator(0);
		tk = getToken();

		return n;
	}
	else if(tk->getTokenName() == "=!_tk")
	{
		n->setToken(tk);
		n->setOperatorIndicator(5);
		tk = getToken();

		return n;
	}
	else
	{
		error("Relational Operator expected ['<<','>>','==','=!'] !");
	}
}

/**
 * Author: Jason Klamert
 * Date: 3/28/2017
 * Description: This function handles the recursive decent for the assign productions.
 **/
static node_t* assign()
{
	node_t* n = new node_t("assign");

	if(tk->getTokenName() == "Id_tk")
	{
		n->setToken(tk);
		tk = getToken();

			if(tk->getTokenName() == "=_tk")
			{
				tk = getToken();
			}
			else
			{
				error("'=' operator expected!");
			}

			n->setChild1(expr());

			if(tk->getTokenName() == ";_tk")
			{
				tk = getToken();
			}
			else
			{
				error("';' expected!");
			}

			return n;
	}
	else
	{
		error("Identifier expected!");
	}
}

/**
 * Author: Jason Klamert
 * Date: 3/28/2017
 * Description: This function handles the recursive decent for the loop productions.
 **/
static node_t* loop()
{
	node_t* n = new node_t("loop");

	if(tk->getTokenName() == "while_tk")
	{
		tk = getToken();

			if(tk->getTokenName() == "(_tk")
			{
				tk = getToken();
			}
			else
			{
				error("'(' expected!");
			}

			n->setChild1(expr());

			n->setChild2(relationalOperator());

			n->setChild3(expr());

			if(tk->getTokenName() == ")_tk")
			{
				tk = getToken();
			}
			else
			{
				error("')' expected!");
			}

			n->setChild4(block());

			return n;
	}
	else
	{
		error("while keyword expected!");
	}
}

/**
 * Author: Jason Klamert
 * Date: 3/28/2017
 * Description: This function handles the recursive decent for the if productions.
 **/
static node_t* ifFunction()
{
	node_t* n = new node_t("ifFunction");

	if(tk->getTokenName() == "decision_tk")
	{
		
		tk = getToken();

			if(tk->getTokenName() == "(_tk")
			{
				tk = getToken();
			}
			else
			{
				error("'(' expected!");
			}

			n->setChild1(expr());

			n->setChild2(relationalOperator());

			n->setChild3(expr());

			if(tk->getTokenName() == ")_tk")
			{
				tk = getToken();
			}
			else
			{
				error("')' expected!");
			}

			n->setChild4(block());

			return n;
	}
	else
	{
		error("decision keyword expected!");
	}
}

/**
 * Author: Jason Klamert
 * Date: 3/28/2017
 * Description: This function handles the recursive decent for the out productions.
 **/
static node_t* out()
{
	node_t* n = new node_t("out");

	if(tk->getTokenName() == "print_tk")
	{
		
		n->setToken(tk);
		tk = getToken();

		if(tk->getTokenName() == "<<_tk")
		{
			tk = getToken();
		}
		else
		{
			error("'<<' was expected after print keyword!");
		}

		n->setChild1(expr());

		return n;
	}
	else
	{
		error("print keyword expected!");
	}
}

/**
 * Author: Jason Klamert
 * Date: 3/28/2017
 * Description: This function handles the recursive decent for the in productions.
 **/
static node_t* in()
{
	node_t* n = new node_t("in");

	if(tk->getTokenName() == "read_tk")
	{
		tk = getToken();

		if(tk->getTokenName() == ">>_tk")
		{
			tk = getToken();
		}
		else
		{
			error(">> expected after read keyword!");
		}

		if(tk->getTokenName() == "Id_tk")
		{
			n->setToken(tk);
			tk = getToken();
		}
		else
		{
			error("An identifier was expected after '>>'!");
		}

		if(tk->getTokenName() == ";_tk")
		{
			
			tk = getToken();
		}
		else
		{
			error("A ';' was expected after the identifier!");
		}

		return n;
	}
	else
	{
		error("read keyword expected!");
	}
}

/**
 * Author: Jason Klamert
 * Date: 3/28/2017
 * Description: This function handles the recursive decent for the stat productions.
 **/
static node_t* stat()
{
	node_t* n = new node_t("stat");

	if(tk->getTokenName() == "read_tk")
	{
		n->setChild1(in());		
	}
	else if(tk->getTokenName() == "print_tk")
	{
		n->setChild1(out());		
	}
	else if(tk->getTokenName() == "start_tk")
	{
		n->setChild1(block());	
	}
	else if(tk->getTokenName() == "decision_tk")
	{
		n->setChild1(ifFunction());	
	}
	else if(tk->getTokenName() == "while_tk")
	{
		n->setChild1(loop());	
	}
	else if(tk->getTokenName() == "Id_tk")
	{
		n->setChild1(assign());
	}
	else
	{
		isMstatEmpty = true;
	}
}

/**
 * Author: Jason Klamert
 * Date: 3/28/2017
 * Description: This function handles the recursive decent for the mStat productions.
 **/
static node_t* mStat()
{
	node_t* n = new node_t("mStat");

	/**
	 * Control construct to determine if mStat produces Empty production.
	 **/
	if(isMstatEmpty == true)
	{
		return n;
	}
	
	n->setChild1(stat());

	n->setChild2(mStat());

	isMstatEmpty = false;
}

/**
 * Author: Jason Klamert
 * Date: 3/28/2017
 * Description: This function handles the recursive decent for the stats productions.
 **/
static node_t* stats()
{
	node_t* n = new node_t("stats");
	
	n->setChild1(stat());

	n->setChild2(mStat());
}

/**
 * Author: Jason Klamert
 * Date: 3/28/2017
 * Description: This function handles the recursive decent for the R productions.
 **/
static node_t* R()
{
	node_t* n = new node_t("R");

	if(tk->getTokenName() == "(_tk")
	{
		
		tk = getToken();

		n->setChild1(expr());

		if(tk->getTokenName() == ")_tk")
		{
			
			tk = getToken();
		}
		else
		{
			error("A ')' was expected!");
		}

		return n;
	}
	else if (tk->getTokenName() == "Id_tk")
	{
		
		n->setToken(tk);
		tk = getToken();

		return n;
	}
	else if(tk->getTokenName() == "Num_tk")
	{
		
		n->setToken(tk);
		tk = getToken();

		return n;
	}
	else
	{
		error("A '(' , Identifier, or Number was expected!");
	}
}

/**
 * Author: Jason Klamert
 * Date: 3/28/2017
 * Description: This function handles the recursive decent for the F productions.
 **/
static node_t* F()
{
	node_t* n = new node_t("F");

	if(tk->getTokenName() == "&_tk") // <F> -> & <F>
	{
		
		tk = getToken();

		n->setChild1(F());

		return n;
	}

	n->setChild1(R());

	return n;

}

/**
 * Author: Jason Klamert
 * Date: 3/28/2017
 * Description: This function handles the recursive decent for the T productions.
 **/
static node_t* T()
{
	node_t* n = new node_t("T");

	n->setChild1(F());

	if(tk->getTokenName() == "+_tk") //<T> -> <F> + <T>
	{
		n->setToken(tk);
		tk = getToken();

		n->setChild2(T());

		return n;
	}
	else if(tk->getTokenName() == "-_tk") //<T> -> <F> - <T>
	{
		n->setToken(tk);
		tk = getToken();

		n->setChild2(T());

		return n;
	}
	else
	{
		return n; //<T> -> <F>
	}
}

/**
 * Author: Jason Klamert
 * Date: 3/28/2017
 * Description: This function handles the recursive decent for the M productions.
 **/
static node_t* M()
{
	node_t* n = new node_t("M");

	n->setChild1(T());

	if(tk->getTokenName() == "/_tk")
	{
		n->setToken(tk);
		tk = getToken();

		n->setChild2(M());

		return n;
	}
	else
	{
		return n; //The production is just T.
	}
}

/**
 * Author: Jason Klamert
 * Date: 3/28/2017
 * Description: This function handles the recursive decent for the expr productions.
 **/
static node_t* expr()
{
	node_t* n = new node_t("expr");

	n->setChild1(M());

	if(tk->getTokenName() == "*_tk")
	{
		n->setToken(tk);
		tk = getToken();

		n->setChild2(expr());

		return n;
	}
	else
	{
		return n; //The production is just M.
	}
}

/**
 * Author: Jason Klamert
 * Date: 3/28/2017
 * Description: This function handles the recursive decent for the mvars productions.
 **/
static node_t* mvars()
{
	node_t* n = new node_t("mvars");

	if(tk->getTokenName() == ":_tk") //<mvars> -> : identifier <mvars>
	{
		
		tk = getToken();

			if(tk->getTokenName() == "Id_tk")
			{
				n->setToken(tk);
				tk = getToken();
			}
			else
			{
				error("An identifer must follow the : delimiter!");
			}

			n->setChild1(mvars());
	}
	else
	{
		return n; //<mvars> -> Empty
	}
}

/**
 * Author: Jason Klamert
 * Date: 3/28/2017
 * Description: This function handles the recursive decent for the vars productions.
 **/
static node_t* vars()
{
	node_t* n = new node_t("vars");

	if(tk->getTokenName() == "int_tk") //<vars> -> int identifer <mvars>
	{
		tk = getToken();

		if(tk->getTokenName() == "Id_tk")
		{
			n->setToken(tk);
			tk = getToken();
		}
		else
		{
			error("Identifier expected after integer keyword!");
		}

		n->setChild1(mvars());

		return n;
	}
	else
	{
		return n; //<vars> -> Empty
	}
}

/**
 * Author: Jason Klamert
 * Date: 3/28/2017
 * Description: This function handles the recursive decent for the program productions.
 **/
static node_t* program()
{
	node_t* n = new node_t("program");

	if(tk->getTokenName() != "program_tk")
	{
		error("Your program must start with the program keyword!");
	}
	
	tk = getToken();

	n->setChild1(vars());

	n->setChild2(block());

	return n;
}

/**
 * Author: Jason Klamert
 * Date: 3/28/2017
 * Description: This function handles the recursive decent for the block productions.
 **/
static node_t* block()
{
	node_t* n = new node_t("block");

	if(tk->getTokenName() == "start_tk")
	{
		tk = getToken();

		n->setChild1(vars());

		n->setChild2(stats());

		if(tk->getTokenName() == "stop_tk")
		{
			
			tk = getToken();
		}
		else
		{
			error("The end of a block must end with the stop keyword!");
		}

		return n;
	}
	else
	{
		error("A start keyword was expected!");
	}

}

/**
 * Author: Jason Klamert
 * Date: 3/28/2017
 * Description: Function to print the given error message and exit the program.
 **/
static void error(string errorString)
{
	cout << "Parser Error: " << errorString << endl;
	cout << "The error occured at line " << tk->getLineNum() << "." << endl;
	cout << "The word encountered was '" << tk->getTokenWord() << "'." << endl;

	exit(-1);
}