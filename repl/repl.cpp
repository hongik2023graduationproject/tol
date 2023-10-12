#include "repl.h"

void Repl::run() {
    string code = readInputFile();

    vector<Token*> tokens = lexer.run(code);

    Program* program = parser.run(tokens);

    vector<Object*> evaluated = evaluator.run(program);
    for (auto object : evaluated)
        cout << object->print() << endl;
}

void Repl::runWithVM() {
    string code = readInputFile();

    vector<Token*> tokens = lexer.run(code);

    Program* program = parser.run(tokens);

    compiler.endian.checkBigEndianComputer();
    Bytecode bytecode = compiler.run(program);

    vm.run(bytecode);

    cout << vm.stack[vm.stackPointer - 1]->print() << endl;
}

void Repl::lexerTest() {
    string input = readInputFile();

    vector<Token*> tokens = lexer.run(input);

    for (const Token* token : tokens) {
        cout << "(\"" << token->literal << "\", " << printTokenType(token->tokenType) << ")" << endl;
    }
}

void Repl::parserTest() {
    string input = readInputFile();

    vector<Token*> tokens = lexer.run(input);

    try {
        Program* program = parser.run(tokens);
        cout << program->String();
    }
    catch (exception& e) {
        cout << e.what() << endl;
    }
}

string Repl::readInputFile() {
    string fileName = "./main.tol";
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
