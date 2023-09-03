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

};


#endif //TOLELOM_ASSIGNSTATEMENT_H
