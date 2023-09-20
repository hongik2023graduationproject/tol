#ifndef TOLELOM_TOKENTYPE_H
#define TOLELOM_TOKENTYPE_H

enum class TokenType {
    ILLEGAL,
    END_OF_FILE,
    NEW_LINE,
    IDENTIFIER,     // 사과
    INTEGER,        // 1, 2, 3
    SPACE,          // ' '
    TAB,            // '\t'
    STARTBLOCK,
    ENDBLOCK,


    COLON,          // :
    // 기호
    PLUS,           // +
    MINUS,          // -
    ASTERISK,       // *
    SLASH,          // /
    ASSIGN,         // =
    EQUAL,          // ==
    NOT_EQUAL,      // !=
    BANG,           // !
    LPAREN,         // (
    RPAREN,         // )
    LBRACE,         // {
    RBRACE,         // }
    LBRACKET,       // [
    RBRACKET,       // ]
    LESS_THAN,      // <

    // 예약어
    LET,            // LET
    INT,            // 정수
    RETURN,         // return
    TRUE,           // true
    FALSE,          // false
    IF,             // 만약
    END_IF,         // 라면
    ELSE,           // 아니면

};

#endif //TOLELOM_TOKENTYPE_H
