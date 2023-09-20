#ifndef TOLELOM_IFEXPRESSION_H
#define TOLELOM_IFEXPRESSION_H

#include "expression.h"
#include "../statements/blockStatement.h"

class IfExpression : public Expression {
public:
    Token* token{};
    Expression* condition{};
    BlockStatement* consequence{};
    BlockStatement* alternative{};

    string String() override {
        string s = "if " + condition->String() + " " + "{\n" + consequence->String() + "}";
        if (alternative != nullptr) {
            s += " else {\n" + alternative->String() + "}";
        }
        return s;
    }
};

#endif //TOLELOM_IFEXPRESSION_H
