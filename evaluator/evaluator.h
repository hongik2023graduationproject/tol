#ifndef TOLELOM_EVALUATOR_H
#define TOLELOM_EVALUATOR_H

#include <typeinfo>
#include "../ast/node.h"
#include "../ast/program.h"
#include "../ast/statements/statement.h"
#include "../ast/statements/expressionStatement.h"
#include "../ast/statements/returnStatement.h"
#include "../ast/statements/letStatement.h"
#include "../ast/expressions/integerExpression.h"
#include "../ast/expressions/prefixExpression.h"
#include "../ast/expressions/infixExpression.h"
#include "../ast/literals/integerLiteral.h"
#include "../ast/literals/booleanLiteral.h"
#include "../object/object.h"
#include "../object/environment.h"

class Evaluator {
public:
    Object* eval(Node* node, Environment* environment);
private:
    Object* evalProgram(vector<Statement*> statements, Environment* environment);
    Object* evalPrefixExpression(Token* token, Object* right);
    Object* evalInfixExpression(Token* token, Object* left, Object* right);
    Object* evalBangOperatorExpression(Object* right);
    Object* evalMinusPrefixOperatorExpression(Object* right);
    Object* evalIntegerInfixExpression(Token* token, Object* left, Object* right);
    Object* evalIdentifier(IdentifierExpression* identifier, Environment* environment);

    // 아직 적용 안함
    Boolean TRUE{true};
    Boolean FALSE{false};
};


#endif //TOLELOM_EVALUATOR_H
