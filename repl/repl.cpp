#include "repl.h"

Repl::Repl() {
    parser.lexer = &lexer;
}


void Repl::run() {
    cout << "한글 프로그래밍 언어 프로젝트" << endl;
    cout << "제작: ezeun, jh-lee-kor, tolelom" << endl;
    cout << "종료는 \"종료하기\"를 입력하세요." << endl;

    Environment* environment = new Environment;
    string input;
    getline(cin, input);
    while (true) {
        cout << ">>> ";
        if (input == "종료하기")
            break;

        input += "\n";
        lexer.insertString(input);

        // parse
        try {
            parser.Parse();
        }
        catch (exception& e) {
            cout << e.what() << endl;
        }

        // evaluate
        try {
            Object *evaluated = evaluator.eval(&parser.program, environment);
            cout << evaluated->print() << endl;
        }
        catch (exception& e) {

        }
    }
}

void Repl::lexerTest() {
    cout << "한글 프로그래밍 언어 프로젝트" << endl;
    cout << "제작: ezeun, jh-lee-kor, tolelom" << endl;
    cout << "종료는 \"종료하기\"를 입력하세요." << endl;
    while (true) {
        cout << ">>> ";
        string input;
        getline(cin, input);
        if (input == "종료하기")
            break;

        input += "\n";
        lexer.insertString(input);
        while (true) {
            try {
                Token* token = lexer.getToken();
                cout << "(\"" << token->literal << "\", " << printTokenType(token->tokenType) << ")" << endl;
            } catch (int ex) {

            }
        }
    }
}

void Repl::parserTest() {
    cout << "한글 프로그래밍 언어 프로젝트" << endl;
    cout << "제작: ezeun, jh-lee-kor, tolelom" << endl;
    cout << "종료는 \"종료하기\"를 입력하세요." << endl;
    while (true) {
        cout << ">>> ";
        string input;
        getline(cin, input);
        if (input == "종료하기")
            break;

        input += "\n";
        lexer.insertString(input);

        try {
            parser.Parse();
            cout << parser.program.String();
        }
        catch (exception& e) {
            cout << e.what() << endl;
        }
    }
}

void Repl::webRun() {
    string fileName = "./main.txt";
    if (access(fileName.c_str(), 0) == -1)
        return;

    Environment* environment = new Environment;
    string input;
    ifstream inputFile(fileName);

    while (inputFile.peek() != EOF) {
        getline(inputFile, input);

        if (input == "종료하기")
            return;

        input += "\n";
        lexer.insertString(input);

        try {
            parser.Parse();
        }
        catch (exception& e) {
            cout << e.what() << endl;
        }

        // evaluate
        try {
            Object *evaluated = evaluator.eval(&parser.program, environment);
            cout << evaluated->print() << endl;
        }
        catch (exception& e) {

        }
    }

}