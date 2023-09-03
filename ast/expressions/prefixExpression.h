#ifndef TOLELOM_PREFIXEXPRESSION_H
#define TOLELOM_PREFIXEXPRESSION_H

#include "expression.h"

class PrefixExpression : Expression {
public:
    Token* token{};
    Expression *right{};
};

#endif //TOLELOM_PREFIXEXPRESSION_H
