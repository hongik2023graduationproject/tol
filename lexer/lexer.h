#ifndef TOLELOM_LEXER_H
#define TOLELOM_LEXER_H

#include <map>
#include <string>
#include <vector>
#include <iostream>
#include "../token/token.h"
#include "../utf8Converter/utf8Converter.h"
//import utf8Converter;

using namespace std;

class Lexer {
public:
    void insertString(const string &input);
    Token* getToken();

private:
    string input;
    vector<string> characters;
    Utf8Converter utf8Converter;
    int currentReadPoint;
    int nextReadPoint;

    static bool isNumber(const std::string &character);
    string readNumber();
    static bool isLetter(const string &character);
    string readLetter();

    map<string, TokenType> keywords = {
            {"정수", TokenType::INT},
            {"변수", TokenType::LET},
            {"리턴", TokenType::RETURN},
            {"true", TokenType::TRUE},
            {"false", TokenType::FALSE},
            {"if", TokenType::IF},
            {"else", TokenType::ELSE},
    };
};




#endif //TOLELOM_LEXER_H
