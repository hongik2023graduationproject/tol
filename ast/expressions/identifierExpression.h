#ifndef TOLELOM_IDENTIFIEREXPRESSION_H
#define TOLELOM_IDENTIFIEREXPRESSION_H

#include <utility>

#include "expression.h"

class IdentifierExpression : public Expression {
public:
    Token* token{};
    string name;

    IdentifierExpression() = default;
    IdentifierExpression(Token* token, string name) : token(token), name(std::move(name)) {};

    string String() override {
        return name;
    }
};

#endif //TOLELOM_IDENTIFIEREXPRESSION_H
