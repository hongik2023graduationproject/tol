#ifndef TOLELOM_NODE_H
#define TOLELOM_NODE_H

#include <string>
#include "../token/token.h"

using namespace std;

class Node {
public:
    Token* token{};

    virtual string String() = 0;
};


#endif //TOLELOM_NODE_H
