#ifndef TOLELOM_INTEGERSTATEMENT_H
#define TOLELOM_INTEGERSTATEMENT_H

#include "statement.h"
#include "../expressions/identifierExpression.h"
#include "../expressions/integerExpression.h"

// 실수 사과 = 4
class IntegerStatement : public Statement {
public:
    Token* token; // TokenType::Integer
    IdentifierExpression* identifierExpression;
    Token* equal;
    IntegerExpression* integerExpression;
};

#endif //TOLELOM_INTEGERSTATEMENT_H
