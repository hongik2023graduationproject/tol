#ifndef TOLELOM_INTEGERSTATEMENT_H
#define TOLELOM_INTEGERSTATEMENT_H

#include "statement.h"
#include "../expressions/identifierExpression.h"
#include "../expressions/expression.h"
#include "../expressions/integerExpression.h"

// 실수 사과 = 4
class IntegerStatement : public Statement {
public:
    Token* token; // TokenType::Integer
    IdentifierExpression* identifierExpression;
    Token* equal;
    Expression* expression;

    string String() {
        return token->literal + " " + identifierExpression->String() + " = " + expression->String() + "\n";
    }
};

#endif //TOLELOM_INTEGERSTATEMENT_H
