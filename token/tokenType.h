#ifndef TOLELOM_TOKENTYPE_H
#define TOLELOM_TOKENTYPE_H

enum class TokenType {
    ILLEGAL,
    END_OF_FILE,
    NEW_LINE,
    IDENTIFIER,     // 사과
    INTEGER,        // 1, 2, 3
    SPACE,          // ' '

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
    LESS_THAN,      // <

    // 예약어
    LET,            // LET
    INT,            // 정수
    RETURN,         // return
    TRUE,           // true
    FALSE,          // false
    IF,             // if
    ELSE,           // else
};

#endif //TOLELOM_TOKENTYPE_H
