#ifndef TOLELOM_CLASSSTATEMENT_H
#define TOLELOM_CLASSSTATEMENT_H

#include <iostream>
#include "statement.h"
#include "../expressions/identifierExpression.h"
#include "../statements/letStatement.h"
#include "../statements/functionStatement.h"
#include "blockStatement.h"

using namespace std;

class ClassStatement : public Statement {
public:
    Token* token{}; // FUNCTION
    IdentifierExpression* name;

    vector<LetStatement*> variables;
    vector<FunctionStatement*> functions;

//    BlockStatement* block; // block으로 처리하는게 맞을까?

    string String() {
        string s = "class " + name->String() + " {\n";
        for (auto variable : variables) {
            s += "  " + variable->String() + "\n";
        }
        for (auto function : functions) {
            s += "  " + function->String() + "\n";
        }
        s += "}\n";
        return s;
    }
};

#endif //TOLELOM_CLASSSTATEMENT_H
