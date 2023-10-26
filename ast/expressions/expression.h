#ifndef TOLELOM_EXPRESSION_H
#define TOLELOM_EXPRESSION_H

#include "../node.h"

class Expression : public Node {
public:
    string String() override = 0;
};

#endif //TOLELOM_EXPRESSION_H
