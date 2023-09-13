#ifndef TOLELOM_INTEGEREXPRESSION_H
#define TOLELOM_INTEGEREXPRESSION_H

#include "expression.h"

class IntegerExpression : public Expression {
public:
    Token* token{}; // 표현식의 첫 번째 토큰
    long long value{};

    IntegerExpression() = default;
    IntegerExpression(Token* token, long long value) : token(token), value(value) {};

    string String() override {
        return to_string(value);
    }
};

#endif //TOLELOM_INTEGEREXPRESSION_H
