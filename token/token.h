#ifndef TOLELOM_TOKEN_H
#define TOLELOM_TOKEN_H

#include <string>
#include "tokenType.h"

using namespace std;

class Token {
public:
    TokenType tokenType;
    string literal;
};


#endif //TOLELOM_TOKEN_H
