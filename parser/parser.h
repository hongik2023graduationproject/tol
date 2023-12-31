#ifndef TOLELOM_PARSER_H
#define TOLELOM_PARSER_H

#include <iostream>
#include <string>
#include <format>
#include <map>
#include "../lexer/lexer.h"
#include "../token/token.h"
#include "../exception/exception.h"
#include "../ast/program.h"
#include "../ast/expressions/identifierExpression.h"
#include "../ast/expressions/integerExpression.h"
#include "../ast/expressions/infixExpression.h"
#include "../ast/expressions/prefixExpression.h"
#include "../ast/expressions/indexExpression.h"
#include "../ast/expressions/functionExpression.h"
#include "../ast/expressions/classExpression.h"

#include "../ast/statements/ifStatement.h"
#include "../ast/statements/loopStatement.h"
#include "../ast/statements/letStatement.h"
#include "../ast/statements/returnStatement.h"
#include "../ast/statements/assignStatement.h"
#include "../ast/statements/expressionStatement.h"
#include "../ast/statements/blockStatement.h"
#include "../ast/statements/classStatement.h"
#include "../ast/statements/classInitStatement.h"

#include "../ast/literals/integerLiteral.h"
#include "../ast/literals/stringLiteral.h"
#include "../ast/literals/booleanLiteral.h"
#include "../ast/statements/functionStatement.h"
#include "../ast/literals/arrayLiteral.h"
#include "../ast/literals/floatLiteral.h"

enum class Precedence;

class Parser {
public:
    Program* run(vector<Token*> tokens);
private:
    vector<Token*> tokens;
    int currentReadPoint;
    int nextReadPoint;
    int nextnextReadPoint;
    Token* currentToken;
    Token* nextToken;
    Token* nextnextToken;
    Program* program;

    using prefixParseFunction = Expression* (Parser::*)();
    using infixParseFunction =  Expression* (Parser::*)(Expression*);
    std::map<TokenType, prefixParseFunction> prefixParseFunctions = {
            {TokenType::IDENTIFIER, &Parser::parseIdentifierExpression},
            {TokenType::INTEGER, &Parser::parseIntegerLiteral},
			{TokenType::FLOAT, &Parser::parseFloatLiteral},
            {TokenType::STRING, &Parser::parseStringLiteral},
            {TokenType::BANG, &Parser::parsePrefixExpression},
            {TokenType::MINUS, &Parser::parsePrefixExpression},
            {TokenType::TRUE, &Parser::parseBooleanLiteral},
            {TokenType::FALSE, &Parser::parseBooleanLiteral},
            {TokenType::LPAREN, &Parser::parseGroupedExpression},
            {TokenType::LBRACKET, &Parser::parseArrayLiteral}, // ????
            {TokenType::LBRACE, &Parser::parseClassExpression},
            {TokenType::COLON, &Parser::parseFunctionExpression},
    };
    std::map<TokenType, infixParseFunction> infixParseFunctions = {
            {TokenType::PLUS, &Parser::parseInfixExpression},
            {TokenType::MINUS, &Parser::parseInfixExpression},
            {TokenType::ASTERISK, &Parser::parseInfixExpression},
            {TokenType::SLASH, &Parser::parseInfixExpression},
            {TokenType::EQUAL, &Parser::parseInfixExpression},
            {TokenType::NOT_EQUAL, &Parser::parseInfixExpression},
            {TokenType::LESS_THAN, &Parser::parseInfixExpression},
			{TokenType::GREATER_THAN, &Parser::parseInfixExpression},
			{TokenType::LESS_EQUAL, &Parser::parseInfixExpression},
			{TokenType::GREATER_EQUAL, &Parser::parseInfixExpression},
            {TokenType::LBRACKET, &Parser::parseIndexExpression}, // ????
    };
    enum class Precedence {
        LOWEST,
        EQUALS,         // ==
        LESSGREATER,    // <, >
        SUM,            // +
        PRODUCT,        // *
        PREFIX,         // -X, !X
        CALL,           // myFunction(x)
        INDEX,          // array[index]
    };
    std::map<TokenType, Precedence> getPrecedence = {
            {TokenType::EQUAL, Precedence::EQUALS},
            {TokenType::NOT_EQUAL, Precedence::EQUALS},
            {TokenType::LESS_THAN, Precedence::LESSGREATER},
			{TokenType::GREATER_THAN, Precedence::LESSGREATER},
			{TokenType::LESS_EQUAL, Precedence::LESSGREATER},
			{TokenType::GREATER_EQUAL, Precedence::LESSGREATER},
            {TokenType::PLUS, Precedence::SUM},
            {TokenType::MINUS, Precedence::SUM},
            {TokenType::ASTERISK, Precedence::PRODUCT},
            {TokenType::SLASH, Precedence::PRODUCT},
            {TokenType::LBRACKET, Precedence::INDEX},
    };

    void initialization();
    void setNextToken();
    void skipToken(TokenType tokenType);
    Statement* parseStatement();
    LetStatement* parseLetStatement();
    ReturnStatement* parseReturnStatement();
    AssignStatement* parseAssignStatement();
    ExpressionStatement* parseExpressionStatement();
    BlockStatement* parseBlockStatement();
	IfStatement* parseIfStatement();
	LoopStatement* parseLoopStatement();
    ClassStatement* parseClassStatement();
    FunctionStatement* parseFunctionStatement();
    ClassInitStatement* parseClassInitStatement();


	Expression* parseIdentifierExpression();
    Expression* parseExpression(Precedence precedence);
    Expression* parsePrefixExpression();
    Expression* parseGroupedExpression();
    Expression* parseInfixExpression(Expression* left);
    Expression* parseIndexExpression(Expression* left);
    Expression* parseFunctionExpression();
    Expression* parseClassExpression();


    Expression* parseIntegerLiteral();
    Expression* parseBooleanLiteral();
    Expression* parseStringLiteral();
    Expression* parseArrayLiteral();
	Expression* parseFloatLiteral();

    vector<IdentifierExpression*> parseFunctionParameters();
    vector<Expression*> parseFunctionExpressionParameters();
    vector<Expression*> parseExpressionList(TokenType endTokenType);
};


#endif //TOLELOM_PARSER_H
