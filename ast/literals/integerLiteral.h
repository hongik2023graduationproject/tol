#ifndef TOLELOM_INTEGERLITERAL_H
#define TOLELOM_INTEGERLITERAL_H

#include "literal.h"
#include "../../token/token.h"

class IntegerLiteral : public Literal  {
public:
    Token* token{};
    long long value{};

    IntegerLiteral() = default;
    IntegerLiteral(Token* token, long long value) : token(token), value(value) {};

    string String() final {
        return token->literal;
    }
};

#endif //TOLELOM_INTEGERLITERAL_H
