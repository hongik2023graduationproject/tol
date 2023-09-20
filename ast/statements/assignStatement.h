#ifndef TOLELOM_ASSIGNSTATEMENT_H
#define TOLELOM_ASSIGNSTATEMENT_H

#include "statement.h"
#include "../expressions/identifierExpression.h"
#include "../expressions/expression.h"

class AssignStatement : public Statement {
public:
    Token* token{};
    IdentifierExpression *name{};
    Expression *value{};

    string String() override {
        return name->String() + " = " + value->String();
    }
};


#endif //TOLELOM_ASSIGNSTATEMENT_H
