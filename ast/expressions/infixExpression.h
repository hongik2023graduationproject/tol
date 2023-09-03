#ifndef TOLELOM_INFIXEXPRESSION_H
#define TOLELOM_INFIXEXPRESSION_H

#include "expression.h"

class InfixExpression : Expression {
public:
    Token* token{};
    Expression *left{};
    Expression *right{};
};

#endif //TOLELOM_INFIXEXPRESSION_H
