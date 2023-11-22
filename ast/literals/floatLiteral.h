#ifndef TOLELOM_FLOATLITERAL_H
#define TOLELOM_FLOATLITERAL_H

#include "literal.h"
#include "../../token/token.h"

class FloatLiteral : public Literal  {
public:
    Token* token{};
    double value{};

	FloatLiteral() = default;
	FloatLiteral(Token* token, double value) : token(token), value(value) {};

    string String() final {
        return token->literal;
    }
};

#endif //TOLELOM_FLOATLITERAL_H
