#ifndef TOLELOM_EXPRESSION_H
#define TOLELOM_EXPRESSION_H

#include "../node.h"

class Expression : public Node {
public:
    string String() override {};
};

#endif //TOLELOM_EXPRESSION_H
