#ifndef TOLELOM_NODE_H
#define TOLELOM_NODE_H

#include "../token/token.h"

class Node {
public:
    virtual ~Node() = default;
    Token* token;
};


#endif //TOLELOM_NODE_H
