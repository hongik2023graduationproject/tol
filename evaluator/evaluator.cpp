#include "evaluator.h"

Object* Evaluator::eval(Node* node, Environment* environment) {
    if (Program* program = dynamic_cast<Program*>(node)) {
        return evalProgram(program->statements, environment);
    }
    else if (ExpressionStatement* expressionStatement = dynamic_cast<ExpressionStatement*>(node)) {
        return eval(expressionStatement->expression, environment);
    }
//    else if (PrefixExpression* prefixExpression = dynamic_cast<PrefixExpression*>(node)) {
//        Object* right = eval(prefixExpression->right);
//        return evalPrefixExpression(prefixExpression->token, right);
//    }
//    else if (IntegerExpression* integerExpression = dynamic_cast<IntegerExpression*>(node)) {
//        Integer* integer = new Integer;
//        integer->value = stoll(inte->token->literal);
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
        Object* right = eval(prefixExpression->right, environment);
        return evalPrefixExpression(prefixExpression->token, right);
    }
    else if (InfixExpression* infixExpression = dynamic_cast<InfixExpression*>(node)) {
        Object* left = eval(infixExpression->left, environment);
        Object* right = eval(infixExpression->right, environment);
        return evalInfixExpression(infixExpression->token, left, right);
    }
    else if (ReturnStatement* returnStatement = dynamic_cast<ReturnStatement*>(node)) {
        Object* value = eval(returnStatement->returnValue, environment);
        ReturnValue* returnValue = new ReturnValue;
        returnValue->value = value;
        return returnValue;
    }
    else if (LetStatement* letStatement = dynamic_cast<LetStatement*>(node)) {
        Object* value = eval(letStatement->expression, environment);
        environment->set(letStatement->name->name, value);
        return value;
    }
    else if (IdentifierExpression* identifierExpression = dynamic_cast<IdentifierExpression*>(node)) {
        return evalIdentifier(identifierExpression, environment);
    }
}

// 이게 되나??
Object* Evaluator::evalProgram(vector<Statement*> statements, Environment* environment) {
    Object* object;

    for (auto &statement : statements) {
        object = eval(statement, environment);

        if (ReturnValue* returnValue = dynamic_cast<ReturnValue*>(object)) {
            return returnValue->value;
        }
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
        if (boolean->value) {
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
    if (left->type == ObjectType::INTEGER && right->type == ObjectType::INTEGER) {
        return evalIntegerInfixExpression(token, left, right);
    }
}

Object* Evaluator::evalIntegerInfixExpression(Token *token, Object *left, Object *right) {
    Integer* leftInteger = dynamic_cast<Integer*>(left);
    Integer* rightInteger = dynamic_cast<Integer*>(right);
    long long leftValue = leftInteger->value;
    long long rightValue = rightInteger->value;

    if (token->tokenType == TokenType::PLUS) {
        Integer* returnInteger = new Integer;
        returnInteger->value = leftValue + rightValue;
        return returnInteger;
    }
    else if (token->tokenType == TokenType::MINUS) {
        Integer* returnInteger = new Integer;
        returnInteger->value = leftValue - rightValue;
        return returnInteger;
    }
    else if (token->tokenType == TokenType::ASTERISK) {
        Integer* returnInteger = new Integer;
        returnInteger->value = leftValue * rightValue;
        return returnInteger;
    }
    else if (token->tokenType == TokenType::SLASH) {
        Integer* returnInteger = new Integer;
        returnInteger->value = leftValue / rightValue; // 자동 형 변환에서 오류 날 가능성이 있음
        return returnInteger;
    }
    else if (token->tokenType == TokenType::LESS_THAN) {
        return new Boolean{leftValue < rightValue};//nativeBoolToBooleanObject(leftValue < rightValue);
    }
    else if (token->tokenType == TokenType::EQUAL) {
        return new Boolean{leftValue == rightValue};
    }
    else if (token->tokenType == TokenType::NOT_EQUAL) {
        return new Boolean{leftValue != rightValue};
    }

    delete leftInteger;
    delete rightInteger;

}

Object* Evaluator::evalIdentifier(IdentifierExpression* identifier, Environment* environment) {
    Object* value = environment->get(identifier->name);
    return value;
}