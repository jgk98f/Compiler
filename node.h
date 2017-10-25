#ifndef NODE_H
#define NODE_H

#include <string>
#include "token.h"

class node_t {

      private:
      std::string label;            //Holds the label of the current node_t.
      token* tk;                     //Holds the token.
      node_t *child1;                 //Holds the first child node_t.
      node_t *child2;                 //Holds the seconds child node_t.
      node_t *child3;                 //Holds the third child node_t.
      node_t *child4;                 //Holds the fourth child node_t.
      int operatorIndicator;          //0 = "==", 1 = "<<", 2 = "<< =", 3 = ">>", 4 = ">> =", 5 = "=!"


      /**
       * Standard public getters and setters for the fields.
       **/
      public:
      node_t();
      node_t(std::string);

      void setLabel(std::string);
      void setToken(token*);
      void setChild1(node_t*);
      void setChild2(node_t*);
      void setChild3(node_t*);
      void setChild4(node_t*);
      void setOperatorIndicator(int);

      std::string getLabel();
      token* getToken();
      node_t* getChild1();
      node_t* getChild2();
      node_t* getChild3();
      node_t* getChild4();
      int getOperatorIndicator();

};

#endif