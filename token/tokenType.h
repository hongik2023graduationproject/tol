#ifndef TOLELOM_TOKENTYPE_H
#define TOLELOM_TOKENTYPE_H

enum class TokenType {
    ILLEGAL,
    END_OF_FILE,
    NEW_LINE,
    IDENTIFIER,
    INT, // int 혹은 정수
    INTEGER, // 실제 숫자
    PLUS,
    MINUS,
    ASTERISK,
    SLASH,
    ASSIGN, // =
    EQUAL, // ==
    SPACE,
};

#endif //TOLELOM_TOKENTYPE_H
