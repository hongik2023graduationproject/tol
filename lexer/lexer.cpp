#include "lexer.h"

void Lexer::insertString(const std::string &input) {
    this->input = input;
    characters = utf8Converter.convert(input);
    currentReadPoint = 0;
    nextReadPoint = 1;
    indentLevel = 0;
}

vector<Token>& Lexer::tokenization() {
    while (currentReadPoint < characters.size()) {
        if (characters[currentReadPoint] == "=") {
            if (characters[nextReadPoint] == "=") {
                tokens.push_back(Token{TokenType::EQUAL, characters[currentReadPoint] + characters[nextReadPoint]});
                currentReadPoint++;
                nextReadPoint++;
            }
            else {
                tokens.push_back({TokenType::ASSIGN, characters[currentReadPoint]});
            }
        }
        else if (characters[currentReadPoint] == "+") {
            tokens.push_back(Token{TokenType::PLUS, characters[currentReadPoint]});
        }
        else if (characters[currentReadPoint] == "-") {
            tokens.push_back(Token{TokenType::MINUS, characters[currentReadPoint]});
        }
        else if (characters[currentReadPoint] == "*") {
            tokens.push_back(Token{TokenType::ASTERISK, characters[currentReadPoint]});
        }
        else if (characters[currentReadPoint] == "/") {
            tokens.push_back(Token{TokenType::SLASH, characters[currentReadPoint]});
        }
        else if (characters[currentReadPoint] == "!") {
            if (characters[nextReadPoint] == "=") {
                tokens.push_back(Token{TokenType::NOT_EQUAL, characters[currentReadPoint] + characters[nextReadPoint]});
                currentReadPoint++;
                nextReadPoint++;
            }
            else {
                tokens.push_back(Token{TokenType::BANG, characters[currentReadPoint]});
            }
        }
        else if (characters[currentReadPoint] == "\t") {
            int tabCount = 1;
            while (nextReadPoint < characters.size() && characters[nextReadPoint] == "\t") {
                currentReadPoint++;
                nextReadPoint++;
                tabCount++;
            }

            if (tabCount > indentLevel) {
                tokens.push_back(Token{TokenType::STARTBLOCK, ""});
            }
            else if (tabCount == indentLevel - 1) {
                tokens.push_back(Token{TokenType::ENDBLOCK, ""});
            }
            else if (tabCount == indentLevel) {
                // 유지
            }
            else {} // error
        }
        else if (characters[currentReadPoint] == " ") {
//        int spaceCount = 1;
//        while (currentReadPoint + spaceCount < characters.size() && characters[currentReadPoint + spaceCount] == " ") {
//            spaceCount++;
//
//            if (spaceCount == 4) {
//                token = new Token(TokenType::TAB, "\t");
//            }
//        }
            tokens.push_back(Token{TokenType::SPACE, characters[currentReadPoint]});
        }
        else if (characters[currentReadPoint] == "\n") {
            tokens.push_back(Token{TokenType::NEW_LINE, "\\n"});
        }
        else if (characters[currentReadPoint] == "(") {
            tokens.push_back(Token{TokenType::LPAREN, characters[currentReadPoint]});
        }
        else if (characters[currentReadPoint] == ")") {
            tokens.push_back(Token{TokenType::RPAREN, characters[currentReadPoint]});
        }
        else if (characters[currentReadPoint] == "{") {
            tokens.push_back(Token{TokenType::LBRACE, characters[currentReadPoint]});
        }
        else if (characters[currentReadPoint] == "}") {
            tokens.push_back(Token{TokenType::RBRACE, characters[currentReadPoint]});
        }
        else if (characters[currentReadPoint] == "[") {
            tokens.push_back(Token{TokenType::LBRACKET, characters[currentReadPoint]});
        }
        else if (characters[currentReadPoint] == "]") {
            tokens.push_back(Token{TokenType::RBRACKET, characters[currentReadPoint]});
        }
        else if (characters[currentReadPoint] == "<") {
            tokens.push_back(Token{TokenType::LESS_THAN, characters[currentReadPoint]});
        }
        else if (isNumber(characters[currentReadPoint])) {
            tokens.push_back(Token{TokenType::INTEGER, readNumber()});
        }
        else if (isLetter(characters[currentReadPoint])) { // isNumber가 먼저 있어서 숫자로 시작하는 문자열은 존재하지 않는다. 혼동의 여지가 있으니 나중에 정리하는 것도 추천
            string letter = readLetter();
            if (keywords.find(letter) != keywords.end()) {
                tokens.push_back(Token(keywords.find(letter)->second, letter));
            }
            else {
                tokens.push_back(Token{TokenType::IDENTIFIER, letter});
            }
        }
        else {
            tokens.push_back(Token{TokenType::ILLEGAL, characters[currentReadPoint]});
        }
        currentReadPoint++;
        nextReadPoint++;
    }

    return tokens;
}


// 폐기 예정
Token* Lexer::getToken() {
    if (characters.size() <= currentReadPoint) { // 고민 중
        return new Token{TokenType::END_OF_FILE, ""};
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
    else if (characters[currentReadPoint] == "\t") {
        int tabCount = 1;
        while (nextReadPoint < characters.size() && characters[nextReadPoint] == "\t") {
            currentReadPoint++;
            nextReadPoint++;
            tabCount++;
        }

        if (tabCount > indentLevel) {
            token = new Token{TokenType::STARTBLOCK, ""};
        }
        else if (tabCount == indentLevel - 1) {
            token = new Token{TokenType::ENDBLOCK, ""};
        }
        else if (tabCount == indentLevel) {
            // 유지
        }
        else {} // error
    }
    else if (characters[currentReadPoint] == " ") {
//        int spaceCount = 1;
//        while (currentReadPoint + spaceCount < characters.size() && characters[currentReadPoint + spaceCount] == " ") {
//            spaceCount++;
//
//            if (spaceCount == 4) {
//                token = new Token(TokenType::TAB, "\t");
//            }
//        }
        token = new Token{TokenType::SPACE, characters[currentReadPoint]};
    }
    else if (characters[currentReadPoint] == "\n") {
        token = new Token{TokenType::NEW_LINE, "\\n"};
    }
    else if (characters[currentReadPoint] == "(") {
        token = new Token{TokenType::LPAREN, characters[currentReadPoint]};
    }
    else if (characters[currentReadPoint] == ")") {
        token = new Token{TokenType::RPAREN, characters[currentReadPoint]};
    }
    else if (characters[currentReadPoint] == "{") {
        token = new Token{TokenType::LBRACE, characters[currentReadPoint]};
    }
    else if (characters[currentReadPoint] == "}") {
        token = new Token{TokenType::RBRACE, characters[currentReadPoint]};
    }
    else if (characters[currentReadPoint] == "[") {
        token = new Token{TokenType::LBRACKET, characters[currentReadPoint]};
    }
    else if (characters[currentReadPoint] == "]") {
        token = new Token{TokenType::RBRACKET, characters[currentReadPoint]};
    }
    else if (characters[currentReadPoint] == "<") {
        token = new Token{TokenType::LESS_THAN, characters[currentReadPoint]};
    }
    else if (isNumber(characters[currentReadPoint])) {
        token = new Token{TokenType::INTEGER, readNumber()};
    }
    else if (isLetter(characters[currentReadPoint])) { // isNumber가 먼저 있어서 숫자로 시작하는 문자열은 존재하지 않는다. 혼동의 여지가 있으니 나중에 정리하는 것도 추천
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
    return ("a" <= character && character <= "z") || ("A" <= character && character <= "Z") || character == "_" || ("가" <= character && character <= "힣") || ("0" <= character && character <= "9");
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