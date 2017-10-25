#include "token.h"
#include "node.h"
#include <string>
using namespace std;

/**
 * Author: Jason Klamert
 * Date: 4/7/2017
 * Constructor for the node_t class.
 **/
node_t::node_t()
{
	label = "";
	tk = new token();
	child1 = NULL;
	child2 = NULL;
	child3 = NULL;
	child4 = NULL;
	operatorIndicator = -1;
}

/**
 * Author: Jason Klamert
 * Date: 4/7/2017
 * Overloaded constructor for the node_t class.
 **/
node_t::node_t(string labelString)
{
	label = labelString;
	tk = new token();
	child1 = NULL;
	child2 = NULL;
	child3 = NULL;
	child4 = NULL;
	operatorIndicator = -1;

}

/**
 * Author: Jason Klamert
 * Date: 4/7/2017
 * Functions for setting the class members.
 **/
void node_t::setLabel(string s)
{
	label = s;
}

/**
 * Author: Jason Klamert
 * Date: 4/7/2017
 * Functions for setting the class members.
 **/
void node_t::setToken(token* tok)
{
	tk = tok;
}

/**
 * Author: Jason Klamert
 * Date: 4/7/2017
 * Functions for setting the class members.
 **/
void node_t::setChild1(node_t* treeNode)
{
	child1 = treeNode;
}

/**
 * Author: Jason Klamert
 * Date: 4/7/2017
 * Functions for setting the class members.
 **/
void node_t::setChild2(node_t* treeNode)
{
	child2 = treeNode;
}

/**
 * Author: Jason Klamert
 * Date: 4/7/2017
 * Functions for setting the class members.
 **/
void node_t::setChild3(node_t* treeNode)
{
	child3 = treeNode;
}

/**
 * Author: Jason Klamert
 * Date: 4/7/2017
 * Functions for setting the class members.
 **/
void node_t::setChild4(node_t* treeNode)
{
	child4 = treeNode;
}

/**
 * Author: Jason Klamert
 * Date: 4/7/2017
 * Functions for setting the class members.
 **/
void node_t::setOperatorIndicator(int choice)
{
	operatorIndicator = choice;
}

/**
 * Author: Jason Klamert
 * Date: 4/7/2017
 * Functions for getting the class members.
 **/
string node_t::getLabel()
{
	return label;
}

/**
 * Author: Jason Klamert
 * Date: 4/7/2017
 * Functions for getting the class members.
 **/
token* node_t::getToken()
{
	return tk;
}

/**
 * Author: Jason Klamert
 * Date: 4/7/2017
 * Functions for getting the class members.
 **/
node_t* node_t::getChild1()
{
	return child1;
}

/**
 * Author: Jason Klamert
 * Date: 4/7/2017
 * Functions for getting the class members.
 **/
node_t* node_t::getChild2()
{
	return child2;
}

/**
 * Author: Jason Klamert
 * Date: 4/7/2017
 * Functions for getting the class members.
 **/
node_t* node_t::getChild3()
{
	return child3;
}

/**
 * Author: Jason Klamert
 * Date: 4/7/2017
 * Functions for getting the class members.
 **/
node_t* node_t::getChild4()
{
	return child4;
}

/**
 * Author: Jason Klamert
 * Date: 4/7/2017
 * Functions for getting the class members.
 **/
int node_t::getOperatorIndicator()
{
	return operatorIndicator;
}