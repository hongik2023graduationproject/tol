#include "lexer.h"

void Lexer::initialization() {
    currentReadPoint = 0;
    nextReadPoint = 1;
    indentLevel = 0;
}


vector<Token*> Lexer::run(const string &code) {
    this->input = code;
    characters = utf8Converter.convert(input);
    initialization();

    while (currentReadPoint < characters.size()) {
        if (characters[currentReadPoint] == ":") {
            tokens.push_back(new Token{TokenType::COLON, characters[currentReadPoint]});
        }
        else if (characters[currentReadPoint] == "=") {
            if (characters[nextReadPoint] == "=") {
                tokens.push_back(new Token{TokenType::EQUAL, characters[currentReadPoint] + characters[nextReadPoint]});
                currentReadPoint++;
                nextReadPoint++;
            }
            else {
                tokens.push_back(new Token{TokenType::ASSIGN, characters[currentReadPoint]});
            }
        }
        else if (characters[currentReadPoint] == ",") {
            tokens.push_back(new Token{TokenType::COMMA, characters[currentReadPoint]});
        }
        else if (characters[currentReadPoint] == "+") {
            tokens.push_back(new Token{TokenType::PLUS, characters[currentReadPoint]});
        }
        else if (characters[currentReadPoint] == "-") {
            if (characters[nextReadPoint] == ">") {
                tokens.push_back(new Token{TokenType::RIGHTARROW, characters[currentReadPoint] + characters[nextReadPoint]});
                currentReadPoint++;
                nextReadPoint++;
            }
            else {
                tokens.push_back(new Token{TokenType::MINUS, characters[currentReadPoint]});
            }
        }
        else if (characters[currentReadPoint] == "*") {
            tokens.push_back(new Token{TokenType::ASTERISK, characters[currentReadPoint]});
        }
        else if (characters[currentReadPoint] == "/") {
            tokens.push_back(new Token{TokenType::SLASH, characters[currentReadPoint]});
        }
        else if (characters[currentReadPoint] == "!") {
            if (characters[nextReadPoint] == "=") {
                tokens.push_back(new Token{TokenType::NOT_EQUAL, characters[currentReadPoint] + characters[nextReadPoint]});
                currentReadPoint++;
                nextReadPoint++;
            }
            else {
                tokens.push_back(new Token{TokenType::BANG, characters[currentReadPoint]});
            }
        }
        else if (characters[currentReadPoint] == "\t") { // 보통 tab은 개행 후에 나오므로 삭제해도 될 것으로 보임
            // 보류
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
            tokens.push_back(new Token{TokenType::SPACE, characters[currentReadPoint]});
        }
        else if (characters[currentReadPoint] == "\n") {
            tokens.push_back(new Token{TokenType::NEW_LINE, "\\n"});

            int tabCount = 0;
            while (nextReadPoint < characters.size() && characters[nextReadPoint] == "\t") {
                currentReadPoint++;
                nextReadPoint++;
                tabCount++;
            }

            if (tabCount > indentLevel) {
                int t = tabCount;
                tokens.push_back(new Token{TokenType::STARTBLOCK, ""});
                t--;

                for (; t != indentLevel; t--) {
                    tokens.push_back(new Token{TokenType::NEW_LINE, "\\n"});
                    tokens.push_back(new Token{TokenType::STARTBLOCK, ""});
                }
            }
            else if (tabCount < indentLevel) {
                int t = tabCount;
                tokens.push_back(new Token{TokenType::ENDBLOCK, ""});
                t++;

                for (; t != indentLevel; t++) {
                    tokens.push_back(new Token{TokenType::NEW_LINE, "\\n"});
                    tokens.push_back(new Token{TokenType::ENDBLOCK, ""});
                }
            }
            indentLevel = tabCount;
        }
        else if (characters[currentReadPoint] == "(") {
            tokens.push_back(new Token{TokenType::LPAREN, characters[currentReadPoint]});
        }
        else if (characters[currentReadPoint] == ")") {
            tokens.push_back(new Token{TokenType::RPAREN, characters[currentReadPoint]});
        }
        else if (characters[currentReadPoint] == "{") {
            tokens.push_back(new Token{TokenType::LBRACE, characters[currentReadPoint]});
        }
        else if (characters[currentReadPoint] == "}") {
            tokens.push_back(new Token{TokenType::RBRACE, characters[currentReadPoint]});
        }
        else if (characters[currentReadPoint] == "[") {
            tokens.push_back(new Token{TokenType::LBRACKET, characters[currentReadPoint]});
        }
        else if (characters[currentReadPoint] == "]") {
            tokens.push_back(new Token{TokenType::RBRACKET, characters[currentReadPoint]});
        }
        else if (characters[currentReadPoint] == "<") {
            tokens.push_back(new Token{TokenType::LESS_THAN, characters[currentReadPoint]});
        }
		else if (characters[currentReadPoint] == ">") {
			tokens.push_back(new Token{TokenType::GREATER_THAN, characters[currentReadPoint]});
		}
        else if (isNumber(characters[currentReadPoint])) {
            tokens.push_back(new Token{TokenType::INTEGER, readNumber()});
        }
        else if (isLetter(characters[currentReadPoint])) { // isNumber가 먼저 있어서 숫자로 시작하는 문자열은 존재하지 않는다. 혼동의 여지가 있으니 나중에 정리하는 것도 추천
            string letter = readLetter();
            if (keywords.find(letter) != keywords.end()) {
                tokens.push_back(new Token{keywords.find(letter)->second, letter});
            }
            else {
                tokens.push_back(new Token{TokenType::IDENTIFIER, letter});
            }
        }
        else {
            tokens.push_back(new Token{TokenType::ILLEGAL, characters[currentReadPoint]});
        }
        currentReadPoint++;
        nextReadPoint++;
    }
    tokens.push_back(new Token{TokenType::END_OF_FILE, ""});

    return tokens;
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