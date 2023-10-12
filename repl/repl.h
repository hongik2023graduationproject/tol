#ifndef TOLELOM_REPL_H
#define TOLELOM_REPL_H

#include <iostream>
#include <exception>
#include <fstream>
#include "../lexer/lexer.h"
#include "../parser/parser.h"
#include "../evaluator/evaluator.h"
#include "../compiler/compiler.h"
#include "../vm/virtualMachine.h"
#include "../token/tokenType.h"
using namespace std;

class Repl {
public:
    void run();
    void runWithVM();
    void parserTest();
    void lexerTest();
private:
    Lexer lexer;
    Parser parser;
    Evaluator evaluator;
    Compiler compiler;
    VirtualMachine vm;

    string readInputFile();
    static std::string printTokenType(TokenType tokenType) {
        switch (tokenType) {
            case TokenType::ILLEGAL:
                return "ILLEGAL";
            case TokenType::END_OF_FILE:
                return "END OF FILE";
            case TokenType::NEW_LINE:
                return "NEW LINE";
            case TokenType::IDENTIFIER:
                return "IDENTIFIER";
            case TokenType::INTEGER:
                return "INTEGER";
            case TokenType::FLOAT:
                return "FLOAT";
            case TokenType::SPACE:
                return "SPACE";
            case TokenType::TAB:
                return "TAB";
            case TokenType::STARTBLOCK:
                return "STARTBLOCK";
            case TokenType::ENDBLOCK:
                return "ENDBLOCK";

            case TokenType::COLON:
                return "COLON";
            case TokenType::RIGHTARROW:
                return "RIGHT ARROW";
            case TokenType::COMMA:
                return "COMMA";

            case TokenType::PLUS:
                return "PLUS";
            case TokenType::MINUS:
                return "MINUS";
            case TokenType::ASTERISK:
                return "ASTERISK";
            case TokenType::SLASH:
                return "SLASH";
            case TokenType::ASSIGN:
                return "ASSIGN";
            case TokenType::EQUAL:
                return "EQUAL";
            case TokenType::NOT_EQUAL:
                return "NOT_EQUAL";
            case TokenType::BANG:
                return "BANG";
            case TokenType::LPAREN:
                return "LPAREN";
            case TokenType::RPAREN:
                return "RPAREN";
            case TokenType::LBRACE:
                return "LBRACE";
            case TokenType::RBRACE:
                return "RBRACE";
            case TokenType::LBRACKET:
                return "LBRACKET";
            case TokenType::RBRACKET:
                return "RBRACKET";
            case TokenType::LESS_THAN:
                return "LESS_THAN";

            case TokenType::INT:
                return "INT";
            case TokenType::실수:
                return "실수";
            case TokenType::FUNCTION:
                return "FUNCTION";
            case TokenType::LET:
                return "LET";
            case TokenType::RETURN:
                return "RETURN";
            case TokenType::TRUE:
                return "TRUE";
            case TokenType::FALSE:
                return "FALSE";
            case TokenType::IF:
                return "IF";
            case TokenType::END_IF:
                return "END_IF";
            case TokenType::ELSE:
                return "ELSE";
        }
        return "??";
    }
};



#endif //TOLELOM_REPL_H
