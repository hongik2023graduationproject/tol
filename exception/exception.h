#ifndef TOLELOM_EXCEPTION_H
#define TOLELOM_EXCEPTION_H

#include <iostream>
#include <exception>
#include <string>
#include <format>
#include <utility>
#include "../token/token.h"

using namespace std;


class CustomException : public exception {
public:
    virtual void print() const = 0;
private:
};

class LexerException : public CustomException {
public:

};

class NotFoundEndOfString : public LexerException {
public:
    explicit NotFoundEndOfString(long long line) : line(line) {};

    void print() const final {
        cout << line << "번 줄: 입력된 문자열의 끝을 알 수 없습니다." << endl;
    }
private:
    long long line;
};

class ParserException : public CustomException {
public:
};

class NotFoundToken : public ParserException {
public:
    NotFoundToken(long long line, TokenType type, Token token) : line(line), type(type), token(std::move(token)) {};

    void print() const final {
        cout << line << "번 줄: 예상되는 토큰이 존재하지 않습니다. 예상 토큰: " << Token::printTokenType(type) << " 현재 토큰: " << Token::printTokenType(token.tokenType) << endl;
    }

private:
    long long line;
    TokenType type;
    Token token;
};



#endif //TOLELOM_EXCEPTION_H
