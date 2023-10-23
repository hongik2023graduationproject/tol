#ifndef TOLELOM_TOKEN_H
#define TOLELOM_TOKEN_H

#include <string>
#include "tokenType.h"

using namespace std;

class Token {
public:
    Token() = default;
    Token(TokenType type, string literal, long long line) : tokenType(type), literal(literal), line(line) {};

    TokenType tokenType;
    string literal;
    long long line;

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
            case TokenType::STRING:
                return "STRING";
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
            case TokenType::SEMICOLON:
                return "SEMICOLON";
            case TokenType::GREATER_THAN:
                return "GREATER_THAN";
            case TokenType::LOOP:
                return "LOOP";
        }
        return "알수없음";
    }
};


#endif //TOLELOM_TOKEN_H
