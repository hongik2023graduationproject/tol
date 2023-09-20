#ifndef TOLELOM_LETSTATEMENT_H
#define TOLELOM_LETSTATEMENT_H

#include "statement.h"
#include "../expressions/identifierExpression.h"
#include "../expressions/expression.h"

class LetStatement : public Statement {
public:
    Token* token{};
    bool isConst{};
    IdentifierExpression* name;
    Expression* expression;

    string String() override {
        return "[" + token->literal + "] " + name->String() + " = " + expression->String();
    };
};

#endif //TOLELOM_LETSTATEMENT_H
