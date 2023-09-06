#ifndef TOLELOM_INFIXEXPRESSION_H
#define TOLELOM_INFIXEXPRESSION_H

#include "expression.h"

class InfixExpression : public Expression {
public:
    Token* token{};
    Expression *left{};
    Expression *right{};
    string String() {
        return "(" + left->String() + " " + token->literal + " " + right->String() + ")";
    }
};

#endif //TOLELOM_INFIXEXPRESSION_H
