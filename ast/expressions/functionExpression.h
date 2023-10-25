#ifndef TOLELOM_FUNCTIONEXPRESSION_H
#define TOLELOM_FUNCTIONEXPRESSION_H

#include <vector>
#include "expression.h"
#include "identifierExpression.h"
#include "../statements/blockStatement.h"

class FunctionExpression : public Expression {
public:
    Token* token{};
    Expression* functionBody;
    vector<Expression*> arguments;

    string String() override {
        string s;
        for (auto& it : arguments)
            s += it->String() + ", ";
        s += functionBody->String() + ".";

        return s;
    }
};


#endif //TOLELOM_FUNCTIONEXPRESSION_H
