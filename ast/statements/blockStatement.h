#ifndef TOLELOM_BLOCKSTATEMENT_H
#define TOLELOM_BLOCKSTATEMENT_H

#include <vector>
#include "statement.h"

class BlockStatement : public Statement {
public:
    Token* token;
    vector<Statement*> statements;

    string String() {
        string s;
        for (auto& statement : statements) {
            s += statement->String() + "\n";
        }
        return s;
    }
};

#endif //TOLELOM_BLOCKSTATEMENT_H
