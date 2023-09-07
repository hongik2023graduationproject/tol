#include "evaluator.h"

Object* Evaluator::eval(Node* node) {
    if (Program* program = dynamic_cast<Program*>(node)) {
        return evalStatements(program->statements);
    }
    else if (ExpressionStatement* expressionStatement = dynamic_cast<ExpressionStatement*>(node)) {
        return eval(expressionStatement->expression);
    }
//    else if (PrefixExpression* prefixExpression = dynamic_cast<PrefixExpression*>(node)) {
//        Object* right = eval(prefixExpression->right);
//        return evalPrefixExpression(prefixExpression->token, right);
//    }
//    else if (IntegerExpression* integerExpression = dynamic_cast<IntegerExpression*>(node)) {
//        Integer* integer = new Integer;
//        integer->value = stoll(integerExpression->token->literal);
//        return new Integer;
//    }
    else if (IntegerLiteral* integerLiteral = dynamic_cast<IntegerLiteral*>(node)) {
        Integer* integer = new Integer;
        integer->value = integerLiteral->value;
        return integer;
    }
    else if (BooleanLiteral* booleanLiteral = dynamic_cast<BooleanLiteral*>(node)) {
        Boolean* boolean = new Boolean;
        boolean->value = booleanLiteral->value;
        return boolean;
    }
    else if (PrefixExpression* prefixExpression = dynamic_cast<PrefixExpression*>(node)) {
        Object* right = eval(prefixExpression->right);
        return evalPrefixExpression(prefixExpression->token, right);
    }
    else if (InfixExpression* infixExpression = dynamic_cast<InfixExpression*>(node)) {
        Object* left = eval(infixExpression->left);
        Object* right = eval(infixExpression->right);
        return evalInfixExpression(infixExpression->token, left, right);
    }
}

// 이게 되나??
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
    else if (token->tokenType == TokenType::MINUS) {
        return evalMinusPrefixOperatorExpression(right);
    }
}

Object* Evaluator::evalBangOperatorExpression(Object *right) {
    if (Boolean* boolean = dynamic_cast<Boolean*>(right)) {
        if (boolean->value == true) {
            delete boolean;
            return new Boolean{false};
        } else {
            delete boolean;
            return new Boolean{true};
        }
    }
    else {
        // type error
    }
}

Object* Evaluator::evalMinusPrefixOperatorExpression(Object *right) {
    if (Integer* integer = dynamic_cast<Integer*>(right)) {
        Integer* newInteger = new Integer;
        newInteger->value = -(integer->value);
        delete integer;
        return newInteger;
    }
    else {
        // type error
    }
}

Object* Evaluator::evalInfixExpression(Token *token, Object *left, Object *right) {
    if (dynamic_cast<Integer*>(left) && dynamic_cast<Integer*>(right)) {
        return evalIntegerInfixExpression(token, left, right);
    }
}

Object* Evaluator::evalIntegerInfixExpression(Token *token, Object *left, Object *right) {
    Integer* leftInteger = dynamic_cast<Integer*>(left);
    Integer* rightInteger = dynamic_cast<Integer*>(right);
    long long leftValue = leftInteger->value;
    long long rightValue = rightInteger->value;

    Integer* returnInteger = new Integer;
    if (token->tokenType == TokenType::PLUS) {
        returnInteger->value = leftValue + rightValue;
    }
    else if (token->tokenType == TokenType::MINUS) {
        returnInteger->value = leftValue - rightValue;
    }
    else if (token->tokenType == TokenType::ASTERISK) {
        returnInteger->value = leftValue * rightValue;
    }
    else if (token->tokenType == TokenType::SLASH) {
        returnInteger->value = leftValue / rightValue; // 자동 형 변환에서 오류 날 가능성이 있음
    }

    delete leftInteger;
    delete rightInteger;

    return returnInteger;
}