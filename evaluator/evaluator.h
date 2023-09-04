#ifndef TOLELOM_EVALUATOR_H
#define TOLELOM_EVALUATOR_H

#include <typeinfo>
#include "../ast/node.h"
#include "../ast/program.h"
#include "../ast/statements/statement.h"
#include "../ast/expressions/integerExpression.h"
#include "../ast/expressions/prefixExpression.h"
#include "../object/object.h"

class Evaluator {
public:
    Object* eval(Node* node);
private:
    Object* evalStatements(vector<Statement*> statements);
    Object* evalPrefixExpression(Token* token, Object* right);
    Object* evalBangOperatorExpression(Object* right);
};


#endif //TOLELOM_EVALUATOR_H
