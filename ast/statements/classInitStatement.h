#ifndef TOLELOM_CLASSINITSTATEMENT_H
#define TOLELOM_CLASSINITSTATEMENT_H


#include "statement.h"
#include "../expressions/identifierExpression.h"
#include "../expressions/expression.h";


class ClassInitStatement : public Statement {
public:
    Token* token{};

    IdentifierExpression* name;
    Expression* value;

    string String() {
        return "." + name->String() + ": " + value->String();
    }
};


#endif //TOLELOM_CLASSINITSTATEMENT_H
