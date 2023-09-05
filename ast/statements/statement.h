#ifndef TOLELOM_STATEMENT_H
#define TOLELOM_STATEMENT_H

#include "../node.h"

class Statement : public Node {
public:
    string String() override = 0;
};

#endif //TOLELOM_STATEMENT_H
