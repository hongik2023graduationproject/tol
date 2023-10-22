#ifndef TOLELOM_INDEXEXPRESSION_H
#define TOLELOM_INDEXEXPRESSION_H

#include "expression.h"


class IndexExpression : public Expression {
public:
    Token* token; // "["
    Expression* left;
    Expression* index;

    string String() {
        return left->String() + "[" + index->String() + "]";
    }
};


#endif //TOLELOM_INDEXEXPRESSION_H
