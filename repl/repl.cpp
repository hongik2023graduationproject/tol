#include "repl.h"

Repl::Repl() {
    parser.lexer = &lexer;
}

void Repl::run() {
    cout << boolalpha;
    string input = readInputFile();

    lexer.insertString(input);

    try {
        parser.Parse();
    }
    catch (exception& e) {
        cout << e.what() << endl;
    }

    // evaluate
    Environment* environment = new Environment;
    try {
        Object *evaluated = evaluator.eval(&parser.program, environment);
        cout << evaluated->print() << endl;
    }
    catch (exception& e) {
        cout << e.what() << endl;
    }
}

void Repl::lexerTest() {
    string input = readInputFile();

    lexer.insertString(input);

    try {
        for (Token* token; token->tokenType != TokenType::END_OF_FILE; ) {
            token = lexer.getToken();
            cout << "(\"" << token->literal << "\", " << printTokenType(token->tokenType) << ")" << endl;
        }
    } catch (exception& e) {
        cout << e.what() << endl;
    }
}

void Repl::parserTest() {
    string input = readInputFile();

    lexer.insertString(input);

    try {
        parser.Parse();
        cout << parser.program.String();
    }
    catch (exception& e) {
        cout << e.what() << endl;
    }
}

string Repl::readInputFile() {
    string fileName = "./main.txt";
    string input;
    ifstream inputStream(fileName);

    if (inputStream.is_open()) {
        inputStream.seekg(0, std::ios::end);
        int size = inputStream.tellg();
        input.resize(size);
        inputStream.seekg(0, std::ios::beg);
        inputStream.read(&input[0], size);
    }

    return input;
}