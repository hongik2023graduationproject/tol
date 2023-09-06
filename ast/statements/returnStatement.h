#ifndef TOLELOM_RETURNSTATEMENT_H
#define TOLELOM_RETURNSTATEMENT_H

#include "statement.h"
#include "../expressions/expression.h"

class ReturnStatement : public Statement {
public:
    Token* token;
    Expression* returnValue;

    string String() {
        return token->literal + " " + returnValue->String();
    }
};

#endif //TOLELOM_RETURNSTATEMENT_H
