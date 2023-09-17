#ifndef TOLELOM_PREFIXEXPRESSION_H
#define TOLELOM_PREFIXEXPRESSION_H

#include "expression.h"

class PrefixExpression : public Expression {
public:
    Token* token{};
    Expression *right{};

    string String() override {
        return "(" + token->literal + right->String() + ")";
    }
};

#endif //TOLELOM_PREFIXEXPRESSION_H
