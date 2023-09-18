#ifndef TOLELOM_FUNCTIONEXPRESSION_H
#define TOLELOM_FUNCTIONEXPRESSION_H

#include <vector>
#include "expression.h"
#include "identifierExpression.h"
#include "../statements/blockStatement.h"

class FunctionExpression : public Expression {
public:
    Token* token{};
    vector<IdentifierExpression*> parameters;
    IdentifierExpression* returnType;
    BlockStatement* body;

    string String() override {
        string ret;
        ret += "[";
        for (auto& parameter : parameters)
            ret += parameter->String() + ", ";
        ret += "] -> [" + returnType->String() + "]";

        return ret;
    }
};

#endif //TOLELOM_FUNCTIONEXPRESSION_H
