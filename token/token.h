#ifndef TOLELOM_TOKEN_H
#define TOLELOM_TOKEN_H

#include <string>
#include "tokenType.h"

class Token {
public:
    TokenType tokenType;
    std::string literal;

};


#endif //TOLELOM_TOKEN_H
