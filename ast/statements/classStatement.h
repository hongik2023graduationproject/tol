#ifndef TOLELOM_CLASSSTATEMENT_H
#define TOLELOM_CLASSSTATEMENT_H

#include <iostream>
#include "statement.h"
#include "../expressions/identifierExpression.h"
#include "blockStatement.h"

using namespace std;

class ClassStatement : public Statement {
public:
    Token* token{}; // FUNCTION
    IdentifierExpression* name;
    BlockStatement* block;

    string String() {
        string s = "class " + name->String() + " {\n" + block->String() + "}\n";
        return s;
    }
};

#endif //TOLELOM_CLASSSTATEMENT_H
