#ifndef TOLELOM_PARSER_H
#define TOLELOM_PARSER_H

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

#include "../ast/statements/letStatement.h"
#include "../ast/statements/returnStatement.h"
#include "../ast/statements/assignStatement.h"
#include "../ast/statements/integerStatement.h"
#include "../ast/statements/expressionStatement.h"

#include "../ast/literals/integerLiteral.h"

enum class Precedence;

class Parser {
public:
    Parser();
    Lexer* lexer{};
    Program program;
    void Parse();

private:
    Token* currentToken{};
    Token* nextToken{};

    using prefixParseFunction = Expression* (Parser::*)();
    using infixParseFunction =  Expression* (Parser::*)(Expression*);
    std::map<TokenType, prefixParseFunction> prefixParseFunctions;
    std::map<TokenType, infixParseFunction> infixParseFunctions;
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

    void setNextToken();
    void skipSpaceToken();
    Statement* parseStatement();
    LetStatement* parseLetStatement();
    ReturnStatement* parseReturnStatement();
    AssignStatement* parseAssignStatement();
    IntegerStatement* parseIntegerStatement();
    ExpressionStatement* parseExpressionStatement();

    Expression* parseIdentifierExpression();
    Expression* parseIntegerExpression();
    Expression* parseExpression(Precedence precedence);
    Expression* parsePrefixExpression();
    Expression* parseInfixExpression(Expression* left);

    Expression* parseIntegerLiteral();
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


#endif //TOLELOM_PARSER_H
