#ifndef TOLELOM_LEXER_H
#define TOLELOM_LEXER_H

#include <set>
#include <map>
#include <string>
#include <vector>
#include "../token/token.h"
#include "../utf8Converter/utf8Converter.h"
#include "../exception/exception.h"

using namespace std;

class Lexer {
public:
    vector<Token*> run(const string &code);
private:
    string input;
    vector<string> characters;
    vector<Token*> tokens;
    Utf8Converter utf8Converter;
    int currentReadPoint;
    int nextReadPoint;
    int indentLevel;
    long long line;

    void initialization();
    void tokenizing();

    static bool isNumber(const std::string &character);
    string readNumber(TokenType & numberType);
    static bool isLetter(const string &character);
    string readLetter();
    string readString();

    set<string> classNames;

    map<string, TokenType> keywords = {
            {"정수", TokenType::INT},
            {"함수", TokenType::FUNCTION},
            {"반환", TokenType::RETURN},
            {"참", TokenType::TRUE},
            {"거짓", TokenType::FALSE},
            {"만약", TokenType::IF},
            {"라면", TokenType::END_IF},
            {"아니면", TokenType::ELSE},
			{"반복", TokenType::LOOP},
            {"문자", TokenType::STR},
            {"클래스", TokenType::CLASS},
    };
};




#endif //TOLELOM_LEXER_H
