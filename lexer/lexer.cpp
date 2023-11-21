#include "lexer.h"

void Lexer::initialization() {
    currentReadPoint = 0;
    nextReadPoint = 1;
    indentLevel = 0;
    line = 1;
}

vector<Token*> Lexer::run(const string &code) {
    this->input = code;
    characters = utf8Converter.convert(input);
    initialization();

    try {
        tokenizing();
    }
    catch (const LexerException& e) {
        e.print();
    }

    return tokens;
}

void Lexer::tokenizing() {
    while (currentReadPoint < characters.size()) {
        if (characters[currentReadPoint] == ":") {
            tokens.push_back(new Token{TokenType::COLON, characters[currentReadPoint], line});
        }
        else if (characters[currentReadPoint] == ";") {
            tokens.push_back(new Token{TokenType::SEMICOLON, characters[currentReadPoint], line});
        }
        else if (characters[currentReadPoint] == "=") {
            if (characters[nextReadPoint] == "=") {
                tokens.push_back(new Token{TokenType::EQUAL, characters[currentReadPoint] + characters[nextReadPoint], line});
                currentReadPoint++;
                nextReadPoint++;
            }
            else {
                tokens.push_back(new Token{TokenType::ASSIGN, characters[currentReadPoint], line});
            }
        }
        else if (characters[currentReadPoint] == ",") {
            tokens.push_back(new Token{TokenType::COMMA, characters[currentReadPoint], line});
        }
        else if (characters[currentReadPoint] == ".") {
            tokens.push_back(new Token{TokenType::DOT, characters[currentReadPoint], line});
        }
        else if (characters[currentReadPoint] == "+") {
            tokens.push_back(new Token{TokenType::PLUS, characters[currentReadPoint], line});
        }
        else if (characters[currentReadPoint] == "-") {
            if (characters[nextReadPoint] == ">") {
                tokens.push_back(new Token{TokenType::RIGHTARROW, characters[currentReadPoint] + characters[nextReadPoint], line});
                currentReadPoint++;
                nextReadPoint++;
            }
            else {
                tokens.push_back(new Token{TokenType::MINUS, characters[currentReadPoint], line});
            }
        }
        else if (characters[currentReadPoint] == "*") {
            tokens.push_back(new Token{TokenType::ASTERISK, characters[currentReadPoint], line});
        }
        else if (characters[currentReadPoint] == "/") {
            tokens.push_back(new Token{TokenType::SLASH, characters[currentReadPoint], line});
        }
        else if (characters[currentReadPoint] == "!") {
            if (characters[nextReadPoint] == "=") {
                tokens.push_back(new Token{TokenType::NOT_EQUAL, characters[currentReadPoint] + characters[nextReadPoint], line});
                currentReadPoint++;
                nextReadPoint++;
            }
            else {
                tokens.push_back(new Token{TokenType::BANG, characters[currentReadPoint], line});
            }
        }
        else if (characters[currentReadPoint] == "\t") {
            tokens.push_back(new Token{TokenType::TAB, characters[currentReadPoint], line});
        }
        else if (characters[currentReadPoint] == " ") {
//            int spaceCount = 1;
//            while (currentReadPoint + spaceCount < characters.size() && characters[currentReadPoint + spaceCount] == " ") {
//                spaceCount++;
//
//                if (spaceCount == 4) {
//                    token = new Token(TokenType::TAB, "\t");
//                }
//            }
            tokens.push_back(new Token{TokenType::SPACE, characters[currentReadPoint], line});
        }
        else if (characters[currentReadPoint] == "\n") {
            int tabCount = 0;
            while (nextReadPoint < characters.size() && characters[nextReadPoint] == "\t") {
                currentReadPoint++;
                nextReadPoint++;
                tabCount++;
            }

            if (tabCount > indentLevel) {
                for (int t = tabCount; t != indentLevel; t--) {
                    tokens.push_back(new Token{TokenType::NEW_LINE, "\\n", line});
                    tokens.push_back(new Token{TokenType::STARTBLOCK, "", line});
                }
            }
            else if (tabCount < indentLevel) {
                for (int t = tabCount; t != indentLevel; t++) {
                    tokens.push_back(new Token{TokenType::NEW_LINE, "\\n", line});
                    tokens.push_back(new Token{TokenType::ENDBLOCK, "", line});
                }
            }
            else {
                tokens.push_back(new Token{TokenType::NEW_LINE, "\\n", line});
            }
            indentLevel = tabCount;
            line++;
        }
        else if (characters[currentReadPoint] == "(") {
            tokens.push_back(new Token{TokenType::LPAREN, characters[currentReadPoint], line});
        }
        else if (characters[currentReadPoint] == ")") {
            tokens.push_back(new Token{TokenType::RPAREN, characters[currentReadPoint], line});
        }
        else if (characters[currentReadPoint] == "{") {
            tokens.push_back(new Token{TokenType::LBRACE, characters[currentReadPoint], line});
        }
        else if (characters[currentReadPoint] == "}") {
            tokens.push_back(new Token{TokenType::RBRACE, characters[currentReadPoint], line});
        }
        else if (characters[currentReadPoint] == "[") {
            tokens.push_back(new Token{TokenType::LBRACKET, characters[currentReadPoint], line});
        }
        else if (characters[currentReadPoint] == "]") {
            tokens.push_back(new Token{TokenType::RBRACKET, characters[currentReadPoint], line});
        }
        else if (characters[currentReadPoint] == "<") {
			if (characters[nextReadPoint] == "=") {
				tokens.push_back(new Token{TokenType::LESS_EQUAL, characters[currentReadPoint] + characters[nextReadPoint], line});
				currentReadPoint++;
				nextReadPoint++;
			}
			else {
				tokens.push_back(new Token{TokenType::LESS_THAN, characters[currentReadPoint], line});
			}
        }
        else if (characters[currentReadPoint] == ">") {
			if (characters[nextReadPoint] == "=") {
				tokens.push_back(new Token{TokenType::GREATER_EQUAL, characters[currentReadPoint] + characters[nextReadPoint], line});
				currentReadPoint++;
				nextReadPoint++;
			}
			else {
				tokens.push_back(new Token{TokenType::GREATER_THAN, characters[currentReadPoint], line});
			}
        }
        else if (characters[currentReadPoint] == "\"") {
            tokens.push_back(new Token{TokenType::STRING, readString(), line});
        }
        else if (isNumber(characters[currentReadPoint])) {
			TokenType numberType;
			string readNum = readNumber(numberType);
            tokens.push_back(new Token{numberType, readNum, line});
        }
        else if (isLetter(characters[currentReadPoint])) { // isNumber가 먼저 있어서 숫자로 시작하는 문자열은 존재하지 않는다. 혼동의 여지가 있으니 나중에 정리하는 것도 추천
            string letter = readLetter();
            if (keywords.find(letter) != keywords.end()) {
                tokens.push_back(new Token{keywords.find(letter)->second, letter, line});
            }
            else {
                tokens.push_back(new Token{TokenType::IDENTIFIER, letter, line});
            }
        }
        else {
            tokens.push_back(new Token{TokenType::ILLEGAL, characters[currentReadPoint], line});
        }
        currentReadPoint++;
        nextReadPoint++;
    }
    tokens.push_back(new Token{TokenType::END_OF_FILE, "", line});
    tokens.push_back(new Token{TokenType::END_OF_FILE, "", line});
    tokens.push_back(new Token{TokenType::END_OF_FILE, "", line});
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

string Lexer::readNumber(TokenType & numberType) {
    string number = characters[currentReadPoint];
	numberType = TokenType::INTEGER;
    while (nextReadPoint < characters.size() && isNumber(characters[nextReadPoint])) {
        currentReadPoint++;
        nextReadPoint++;
        number += characters[currentReadPoint];
    }
	if (characters[nextReadPoint] == ".") {
		numberType = TokenType::FLOAT;
		currentReadPoint++;
		nextReadPoint++;
		number += characters[currentReadPoint];
		while (nextReadPoint < characters.size() && isNumber(characters[nextReadPoint])) {
			currentReadPoint++;
			nextReadPoint++;
			number += characters[currentReadPoint];
		}
	}
    return number;
}


string Lexer::readString() {
    currentReadPoint++;
    nextReadPoint++;

    string s;
    while (nextReadPoint < characters.size() && characters[currentReadPoint] != "\"") {
        s += characters[currentReadPoint];
        currentReadPoint++;
        nextReadPoint++;
    }

    if (characters[currentReadPoint] != "\"") {
        throw NotFoundEndOfString(line);
    }

    return s;
}