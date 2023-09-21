
#ifndef TOLELOM_FUNCTIONLITERAL_H
#define TOLELOM_FUNCTIONLITERAL_H

#include <vector>
#include "literal.h"
#include "../expressions/identifierExpression.h"
#include "../statements/blockStatement.h"

class FunctionLiteral : public Literal {
public:
    Token* token{};
    vector<IdentifierExpression*> parameters;
    IdentifierExpression* name;
    BlockStatement* blockStatement;
    Token* returnType{};

    string String() override {
        string s = "함수: ";

        for (auto& parameter : parameters) {
            s += parameter->String() + ", ";
        }
        s += name->String() + ".\n";

        s += blockStatement->String();
        return s;
    }
};


#endif //TOLELOM_FUNCTIONLITERAL_H
