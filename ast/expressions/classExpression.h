#ifndef TOLELOM_CLASSEXPRESSION_H
#define TOLELOM_CLASSEXPRESSION_H

#include "expression.h"
#include "../statements/classInitStatement.h"

class ClassExpression : public Expression {
public:
    Token* token{};
    IdentifierExpression* name;
    vector<ClassInitStatement*> statements;

    string String() {
        string s = "{\n";
        for (auto initStatement : statements) {
            s += "  " + initStatement->String() + "\n";
        }
        s += "}";
        return s;
    }
};


#endif //TOLELOM_CLASSEXPRESSION_H
