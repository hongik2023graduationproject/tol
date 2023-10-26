#include "repl.h"

void Repl::runWithEvaluator() {
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
	auto * fe = new FunctionExpression;
	fe->function = new IdentifierExpression;
	((IdentifierExpression*)fe->function)->name = "뭐라도하기";
	program->statements.push_back((Statement*)fe);
    Bytecode bytecode = compiler.run(program);
	vm.run(bytecode);

    cout << vm.lastPoppedElement()->print() << endl;
}

void Repl::lexerTest() {
    string input = readInputFile();
    vector<Token*> tokens = lexer.run(input);

    for (const Token* token : tokens) {
        cout << "(\"" << token->literal << "\", " << Token::printTokenType(token->tokenType) << ")" << endl;
    }
}

void Repl::parserTest() {
    string input = readInputFile();
    vector<Token*> tokens = lexer.run(input);
    Program* program = parser.run(tokens);

    cout << program->String();
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
