#ifndef TOLELOM_PROGRAM_H
#define TOLELOM_PROGRAM_H

#include <vector>
#include "node.h"
#include "statements/statement.h"

using namespace std;

class Program : Node {
public:
    vector<Statement*> statements;
};

#endif //TOLELOM_PROGRAM_H
