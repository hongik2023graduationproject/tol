#ifndef TOLELOM_IFSTATEMENT_H
#define TOLELOM_IFSTATEMENT_H

#include "../statements/statement.h"
#include "blockStatement.h"

class IfStatement : public Statement {
public:
    Token* token{};
    Expression* condition{};
    BlockStatement* consequence{};
    BlockStatement* alternative{};

    string String() override {
        string s = "if: " + condition->String() + " " + "{\n" + consequence->String() + "}";
        if (alternative != nullptr) {
            s += " else: {\n" + alternative->String() + "}";
        }
        return s;
    }
};

#endif //TOLELOM_IFSTATEMENT_H
