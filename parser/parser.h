#ifndef TOLELOM_PARSER_H
#define TOLELOM_PARSER_H

#include <iostream>
#include <string>
#include <format>
#include <map>
#include "../lexer/lexer.h"
#include "../token/token.h"
#include "../ast/program.h"
#include "../ast/expressions/identifierExpression.h"
#include "../ast/expressions/integerExpression.h"
#include "../ast/expressions/infixExpression.h"
#include "../ast/expressions/prefixExpression.h"
#include "../ast/statements/ifStatement.h"
#include "../ast/statements/loopStatement.h"

#include "../ast/statements/letStatement.h"
#include "../ast/statements/returnStatement.h"
#include "../ast/statements/assignStatement.h"
#include "../ast/statements/integerStatement.h"
#include "../ast/statements/expressionStatement.h"
#include "../ast/statements/blockStatement.h"

#include "../ast/literals/integerLiteral.h"
#include "../ast/literals/booleanLiteral.h"
#include "../ast/literals/functionLiteral.h"

enum class Precedence;

class Parser {
public:
    Program* run(vector<Token*> tokens);
private:
    vector<Token*> tokens;
    int currentReadPoint;
    int nextReadPoint;
    Token* currentToken;
    Token* nextToken;
    Program* program;

    using prefixParseFunction = Expression* (Parser::*)();
    using infixParseFunction =  Expression* (Parser::*)(Expression*);
    std::map<TokenType, prefixParseFunction> prefixParseFunctions = {
            {TokenType::IDENTIFIER, &Parser::parseIdentifierExpression},
            {TokenType::INTEGER, &Parser::parseIntegerLiteral},
            {TokenType::BANG, &Parser::parsePrefixExpression},
            {TokenType::MINUS, &Parser::parsePrefixExpression},
            {TokenType::TRUE, &Parser::parseBooleanLiteral},
            {TokenType::FALSE, &Parser::parseBooleanLiteral},
            {TokenType::LPAREN, &Parser::parseGroupedExpression},
            {TokenType::FUNCTION, &Parser::parseFunctionLiteral},
    };
    std::map<TokenType, infixParseFunction> infixParseFunctions = {
            {TokenType::PLUS, &Parser::parseInfixExpression},
            {TokenType::MINUS, &Parser::parseInfixExpression},
            {TokenType::ASTERISK, &Parser::parseInfixExpression},
            {TokenType::SLASH, &Parser::parseInfixExpression},
            {TokenType::EQUAL, &Parser::parseInfixExpression},
            {TokenType::NOT_EQUAL, &Parser::parseInfixExpression},
            {TokenType::LESS_THAN, &Parser::parseInfixExpression},
    };
    enum class Precedence {
        LOWEST,
        EQUALS,         // ==
        LESSGREATER,    // <, >
        SUM,            // +
        PRODUCT,        // *
        PREFIX,         // -X, !X
        CALL            // myFunction(x)
    };
    std::map<TokenType, Precedence> getPrecedence = {
            {TokenType::EQUAL, Precedence::EQUALS},
            {TokenType::NOT_EQUAL, Precedence::EQUALS},
            {TokenType::LESS_THAN, Precedence::LESSGREATER},
            {TokenType::PLUS, Precedence::SUM},
            {TokenType::MINUS, Precedence::SUM},
            {TokenType::ASTERISK, Precedence::PRODUCT},
            {TokenType::SLASH, Precedence::PRODUCT},
    };

    void initialization();
    void setNextToken();
    void skipSpaceToken();
    Statement* parseStatement();
    LetStatement* parseLetStatement();
    ReturnStatement* parseReturnStatement();
    AssignStatement* parseAssignStatement();
    IntegerStatement* parseIntegerStatement();
    ExpressionStatement* parseExpressionStatement();
    BlockStatement* parseBlockStatement();
	IfStatement* parseIfStatement();
	LoopStatement* parseLoopStatement();

	Expression* parseIdentifierExpression();
    Expression* parseIntegerExpression();
    Expression* parseExpression(Precedence precedence);
    Expression* parsePrefixExpression();
    Expression* parseGroupedExpression();
    Expression* parseInfixExpression(Expression* left);

    Expression* parseIntegerLiteral();
    Expression* parseBooleanLiteral();
    Expression* parseFunctionLiteral();

    vector<IdentifierExpression*> parseFunctionParameters();
};


#endif //TOLELOM_PARSER_H
