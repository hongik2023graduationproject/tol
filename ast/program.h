#ifndef TOLELOM_PROGRAM_H
#define TOLELOM_PROGRAM_H

#include <vector>
#include "node.h"
#include "statements/statement.h"

using namespace std;

class Program : public Node {
public:
    vector<Statement*> statements;

    string String() {
        string s;
        for (auto& statement : statements) {
            s += statement->String() + "\n";
        }
        return s;
    }
};

#endif //TOLELOM_PROGRAM_H
