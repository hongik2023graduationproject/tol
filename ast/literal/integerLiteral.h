#ifndef TOLELOM_INTEGERLITERAL_H
#define TOLELOM_INTEGERLITERAL_H

#include "literal.h"
#include "../../token/token.h"

class IntegerLiteral : public Literal {
public:
    Token token;
};

#endif //TOLELOM_INTEGERLITERAL_H
