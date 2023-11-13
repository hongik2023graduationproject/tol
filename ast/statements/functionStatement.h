
#ifndef TOLELOM_FUNCTIONSTATEMENT_H
#define TOLELOM_FUNCTIONSTATEMENT_H

#include <vector>
#include "../literals/literal.h"
#include "../expressions/identifierExpression.h"
#include "blockStatement.h"

class FunctionStatement : public Statement {
public:
    Token* token{};
    vector<Token*> parameterTypes;
    vector<IdentifierExpression*> parameters;
    IdentifierExpression* name;
    BlockStatement* blockStatement;
    Token* returnType{};

    string String() override {
        string s = "함수: ";

        for (auto& parameter : parameters) {
            s += parameter->String() + ", ";
        }
        s += name->String() + ". {\n";

        s += blockStatement->String();
        s += "}";
        return s;
    }
};


#endif //TOLELOM_FUNCTIONSTATEMENT_H
