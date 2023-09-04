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
#include "../ast/statements/assignStatement.h"
#include "../ast/statements/integerStatement.h"

class Parser {
public:
    Lexer* lexer{};
    Program program;
    void Parse();
private:
    Token* currentToken{};
    Token* nextToken{};

    void setNextToken();
    void skipSpaceToken();
    Statement* parseStatement();
    AssignStatement* parseAssignStatement();
    IntegerStatement* parseIntegerStatement();

    IdentifierExpression* parseIdentifierExpression();
    IntegerExpression* parseIntegerExpression();
};

//enum class Precedences {
//    LOWEST,
//    EQUALS,
//    LESSGREATER,
//    SUM,
//    PRODUCT,
//    PREFIX,
//    CALL
//};


#endif //TOLELOM_PARSER_H
