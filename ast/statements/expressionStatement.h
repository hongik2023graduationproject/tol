#ifndef TOLELOM_EXPRESSIONSTATEMENT_H
#define TOLELOM_EXPRESSIONSTATEMENT_H

#include "statement.h"
#include "../expressions/expression.h"

class ExpressionStatement : public Statement {
public:
    Token* token{}; // 표현식의 첫 번째 토큰
    Expression* expression{};

    ExpressionStatement() = default;
    ExpressionStatement(Token* token, Expression* expression) : token(token), expression(expression) {};

    string String() override {
        if (expression != nullptr) {
            return expression->String();
        }
        return "";
    }
};

#endif //TOLELOM_EXPRESSIONSTATEMENT_H
