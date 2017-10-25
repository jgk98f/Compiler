#ifndef TOKEN_H
#define TOKEN_H

#include <string>

class token {

      private:
      int tokenId;           //Holds value of the token. (ID_tk: 1000)
      std::string tokenName;      //Holds the token name.
      int lineNum;           //Holds the line number of the token.
      std::string tokenWord;      //Holds the actual string found.

      public:
      token();
      token(int, std::string, int, std::string);

      void setTokenId(int);
      void setTokenName(std::string);
      void setLineNum(int);
      void setTokenWord(std::string);

      int getTokenId();
      std::string getTokenName();
      int getLineNum();
      std::string getTokenWord();

};

#endif
