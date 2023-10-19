#ifndef TOLELOM_STRINGLITERAL_H
#define TOLELOM_STRINGLITERAL_H

#include <utility>

#include "string"
#include "literal.h"

using namespace std;

class StringLiteral : public Literal {
public:
    Token* token{};
    string value{};

    StringLiteral() = default;
    StringLiteral(Token* token, string s) : token(token), value(std::move(s)) {};

    string String() {
        return "\"" + value + "\"";
    }
};


#endif //TOLELOM_STRINGLITERAL_H
