#ifndef TOLELOM_CLASSEXPRESSION_H
#define TOLELOM_CLASSEXPRESSION_H

#include "expression.h"
#include "../statements/classInitStatement.h"

class ClassExpression : public Expression {
public:
    Token* token{};
    vector<ClassInitStatement*> statements; // 선언, 함수만 들어가야함 체크는 컴파일러에서


    string String() {
        string s = "{\n";
        for (auto initStatement : statements) {
            s += "  " + initStatement->String() + "\n";
        }
        s += "}";
        return s;
    }
};


#endif //TOLELOM_CLASSEXPRESSION_H
