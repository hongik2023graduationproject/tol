#ifndef TOLELOM_BOOLEANLITERAL_H
#define TOLELOM_BOOLEANLITERAL_H

#include "literal.h"

class BooleanLiteral : public Literal {
public:
    Token* token{};
    bool value{};

    BooleanLiteral() = default;
    BooleanLiteral(Token* token, bool value) : token(token), value(value) {};

    string String() final {
        return token->literal;
    }
};

#endif //TOLELOM_BOOLEANLITERAL_H
