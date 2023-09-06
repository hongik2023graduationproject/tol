#ifndef TOLELOM_BOOLEANLITERAL_H
#define TOLELOM_BOOLEANLITERAL_H

#include "literal.h"

class BooleanLiteral : public Literal {
public:
    Token* token;
    bool value;
    string String() {
        return token->literal;
    }
};

#endif //TOLELOM_BOOLEANLITERAL_H
