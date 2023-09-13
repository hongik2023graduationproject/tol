#ifndef TOLELOM_RETURNSTATEMENT_H
#define TOLELOM_RETURNSTATEMENT_H

#include "statement.h"
#include "../expressions/expression.h"

class ReturnStatement : public Statement {
public:
    Token* token{};
    Expression* returnValue{};

    ReturnStatement() = default;
    ReturnStatement(Token* token, Expression* expression) : token(token), returnValue(expression) {};

    string String() override {
        return token->literal + " " + returnValue->String();
    }
};

#endif //TOLELOM_RETURNSTATEMENT_H
