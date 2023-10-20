#ifndef TOLELOM_ARRAYLITERAL_H
#define TOLELOM_ARRAYLITERAL_H

#include <vector>

#include "../expressions/expression.h"
#include "literal.h"

class ArrayLiteral : public Literal {
public:
    Token* token;
    vector<Expression*> elements;

    string String() {
        string s = "{";
        for (auto& element : elements) {
            s += element->String() + ", ";
        }
        s += "}";
        return s;
    }
};

#endif //TOLELOM_ARRAYLITERAL_H
