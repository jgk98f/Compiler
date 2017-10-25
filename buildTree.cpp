#include <iostream>
#include <stdio.h>
#include "buildTree.h"
#include "node.h"
#include "token.h"
using namespace std;

static void printToken( token* t);

void printParseTree(node_t* root, int level) 
{
	token* tk;

	if(root == NULL) 
	{
		return;
	}
	else
	{
		tk = root->getToken();
	}

	printf("%*s" "%d %s ", level * 4, "", level, root->getLabel().c_str());

    if(tk->getTokenName() != "")
	{
		printToken(tk);
	}
	
	if(tk->getTokenName() == "")
	{
		printf("\n");
	}
	
	printParseTree(root->getChild1(), level + 1);
	printParseTree(root->getChild2(), level + 1);
	printParseTree(root->getChild3(), level + 1);
	printParseTree(root->getChild4(), level + 1);
}

static void printToken( token* t)
{
  cout << "Token: [id = " << t->getTokenId() << " | tokenName = \"" << t->getTokenName() << "\" | line# = " << t->getLineNum() << " | tokenWord = \"" << t->getTokenWord() << "\"]" << endl;
}
