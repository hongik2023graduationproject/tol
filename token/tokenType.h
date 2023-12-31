#ifndef TOLELOM_TOKENTYPE_H
#define TOLELOM_TOKENTYPE_H

enum class TokenType {
    ILLEGAL,
    END_OF_FILE,
    NEW_LINE,
    IDENTIFIER,     // 사과
    INTEGER,        // 1, 2, 3
    FLOAT,          // 1.0, 1.5, 2.0
    STRING,
    SPACE,          // ' '
    TAB,            // '\t'
    STARTBLOCK,
    ENDBLOCK,


    COLON,          // :
	SEMICOLON,		// ;
    RIGHTARROW,     // ->
    COMMA,          // ,
    DOT,            // .
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
	GREATER_THAN,	// >
	LESS_EQUAL,  	// <=
	GREATER_EQUAL, 	// >=


    // 예약어
    LET,            // LET
    INT,            // 정수
    실수,            // 실수
    STR,            // 문자열
    FUNCTION,       // 함수
    CLASS,          // 클래스
    RETURN,         // return
    TRUE,           // true
    FALSE,          // false
    IF,             // 만약
    END_IF,         // 라면
    ELSE,           // 아니면
	LOOP,			// 반복

};

#endif //TOLELOM_TOKENTYPE_H
