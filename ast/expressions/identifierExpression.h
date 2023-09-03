#ifndef TOLELOM_IDENTIFIEREXPRESSION_H
#define TOLELOM_IDENTIFIEREXPRESSION_H

#include "expression.h"

class IdentifierExpression : Expression {
public:
    Token* token{};
};

#endif //TOLELOM_IDENTIFIEREXPRESSION_H
