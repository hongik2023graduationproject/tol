#include "evaluator.h"

vector<Object*> Evaluator::run(Program *program) {
    Environment* environment = new Environment;
    return evalProgram(program->statements, environment);
}

Object* Evaluator::eval(Node* node, Environment* environment) {
    if (ExpressionStatement* expressionStatement = dynamic_cast<ExpressionStatement*>(node)) {
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
        integer->type = ObjectType::INTEGER;
        integer->value = integerLiteral->value;
        return integer;
    }
    else if (StringLiteral* stringLiteral = dynamic_cast<StringLiteral*>(node)) {
        String* string = new String;
        string->type = ObjectType::STRING;
        string->value = stringLiteral->value;
        return string;
    }
    else if (BooleanLiteral* booleanLiteral = dynamic_cast<BooleanLiteral*>(node)) {
        Boolean* boolean = new Boolean;
        boolean->type = ObjectType::BOOLEAN;
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
        returnValue->type = ObjectType::RETURN_VALUE_OBJECT;
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
vector<Object*> Evaluator::evalProgram(const vector<Statement*>& statements, Environment* environment) {
    vector<Object*> objects;

    for (auto &statement : statements) {
        objects.push_back(eval(statement, environment));

//        if (ReturnValue* returnValue = dynamic_cast<ReturnValue*>(object)) {
//            return returnValue->value;
//        }
    }

    return objects;
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
        newInteger->type = ObjectType::INTEGER;
        newInteger->value = -(integer->value);
        delete integer;
        return newInteger;
    }
    else {
        // type error
    }
}

Object* Evaluator::evalInfixExpression(Token* token, Object *left, Object *right) {
    if (left->type == ObjectType::INTEGER && right->type == ObjectType::INTEGER) {
        return evalIntegerInfixExpression(token, left, right);
    }
    else if (left->type == ObjectType::STRING && right->type == ObjectType::STRING) {
        return evalStringInfixExpression(token, left, right);
    }
}

Object* Evaluator::evalIntegerInfixExpression(Token* token, Object *left, Object *right) {
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

Object* Evaluator::evalStringInfixExpression(Token *token, Object *left, Object *right) {
    String* leftString = static_cast<String*>(left);
    String* rightString = static_cast<String*>(right);

    if (token->tokenType == TokenType::PLUS) {
        return new String(leftString->value + rightString->value);
    }
}

Object* Evaluator::evalIdentifier(IdentifierExpression* identifier, Environment* environment) {
    Object* value = environment->get(identifier->name);
    return value;
}

