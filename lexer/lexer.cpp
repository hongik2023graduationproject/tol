#include "lexer.h"


void Lexer::insertString(const std::string &input) {
    this->input = input;
    characters = utf8Converter.convert(input);
    currentReadPoint = 0;
    nextReadPoint = 1;
}

Token* Lexer::getToken() {
    if (characters.size() <= currentReadPoint) { // 고민 중
        return new Token{TokenType::NEW_LINE, "\n"};
    }

    Token* token;
    if (characters[currentReadPoint] == "=") {
        if (characters[nextReadPoint] == "=") {
            token = new Token{TokenType::EQUAL, characters[currentReadPoint] + characters[nextReadPoint]};
            currentReadPoint++;
            nextReadPoint++;
        }
        else {
            token = new Token{TokenType::ASSIGN, characters[currentReadPoint]};
        }
    }
    else if (characters[currentReadPoint] == "+") {
        token = new Token{TokenType::PLUS, characters[currentReadPoint]};
    }
    else if (characters[currentReadPoint] == "-") {
        token = new Token{TokenType::MINUS, characters[currentReadPoint]};
    }
    else if (characters[currentReadPoint] == "*") {
        token = new Token{TokenType::ASTERISK, characters[currentReadPoint]};
    }
    else if (characters[currentReadPoint] == "/") {
        token = new Token{TokenType::SLASH, characters[currentReadPoint]};
    }
    else if (characters[currentReadPoint] == "!") {
        if (characters[nextReadPoint] == "=") {
            token = new Token{TokenType::NOT_EQUAL, characters[currentReadPoint] + characters[nextReadPoint]};
            currentReadPoint++;
            nextReadPoint++;
        }
        else {
            token = new Token{TokenType::BANG, characters[currentReadPoint]};
        }
    }
    else if (characters[currentReadPoint] == " ") {
        token = new Token{TokenType::SPACE, characters[currentReadPoint]};
    }
    else if (characters[currentReadPoint] == "\n") {
        token = new Token{TokenType::NEW_LINE, characters[currentReadPoint]};
    }
    else if (characters[currentReadPoint] == "(") {
        token = new Token{TokenType::LPAREN, characters[currentReadPoint]};
    }
    else if (characters[currentReadPoint] == ")") {
        token = new Token{TokenType::RPAREN, characters[currentReadPoint]};
    }
    else if (characters[currentReadPoint] == "<") {
        token = new Token{TokenType::LESS_THAN, characters[currentReadPoint]};
    }
    else if (isNumber(characters[currentReadPoint])) {
        token = new Token{TokenType::INTEGER, readNumber()};
    }
    else if (isLetter(characters[currentReadPoint])) {
        string letter = readLetter();
        if (keywords.find(letter) != keywords.end()) {
            token = new Token(keywords.find(letter)->second, letter);
        }
        else {
            token = new Token{TokenType::IDENTIFIER, letter};
        }
    }
    else {
        token = new Token{TokenType::ILLEGAL, characters[currentReadPoint]};
    }

    currentReadPoint++;
    nextReadPoint++;
    return token;
}

bool Lexer::isLetter(const std::string &character) {
    return ("a" <= character && character <= "z") || ("A" <= character && character <= "Z") || character == "_" || ("가" <= character && character <= "힣");
}

string Lexer::readLetter() {
    string letter = characters[currentReadPoint];
    while (nextReadPoint < characters.size() && isLetter(characters[nextReadPoint])) {
        currentReadPoint++;
        nextReadPoint++;
        letter += characters[currentReadPoint];
    }
    return letter;
}


bool Lexer::isNumber(const std::string &character) {
    return ("0" <= character && character <= "9");
}

string Lexer::readNumber() {
    string number = characters[currentReadPoint];
    while (nextReadPoint < characters.size() && isNumber(characters[nextReadPoint])) {
        currentReadPoint++;
        nextReadPoint++;
        number += characters[currentReadPoint];
    }
    return number;
}