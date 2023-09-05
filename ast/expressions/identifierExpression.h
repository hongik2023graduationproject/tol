#ifndef TOLELOM_IDENTIFIEREXPRESSION_H
#define TOLELOM_IDENTIFIEREXPRESSION_H

#include "expression.h"

class IdentifierExpression : public Expression {
public:
    Token* token{};
    string name;

    string String() {
        return name;
    }
};

#endif //TOLELOM_IDENTIFIEREXPRESSION_H
