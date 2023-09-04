#include "evaluator.h"

Object* Evaluator::eval(Node* node) {
    if (Program* program = dynamic_cast<Program*>(node)) {
        return evalStatements(program->statements);
    }
    else if (PrefixExpression* prefixExpression = dynamic_cast<PrefixExpression*>(node)) {
        Object* right = eval(prefixExpression->right);
        return evalPrefixExpression(prefixExpression->token, right);
    }
    else if (IntegerExpression* integerExpression = dynamic_cast<IntegerExpression*>(node)) {
        Integer* integer = new Integer;
        integer->value = stoll(integerExpression->token->literal);
        return new Integer;
    }
}

Object* Evaluator::evalStatements(vector<Statement*> statements) {
    Object* object;
    for (auto &statement : statements) {
        object = eval(statement);
    }
    return object;
}

Object* Evaluator::evalPrefixExpression(Token* token, Object *right) {
    if (token->tokenType == TokenType::BANG) {
        return evalBangOperatorExpression(right);
    }
}

Object* Evaluator::evalBangOperatorExpression(Object *right) {

}