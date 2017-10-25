#ifndef SCANNER_H
#define SCANNER_H

#include "scanner.h"
#include "token.h"

token* getToken();
void readIntoBuffer(FILE* file);

#endif
