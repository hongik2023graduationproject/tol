#ifndef TOLELOM_REPL_H
#define TOLELOM_REPL_H

#include <exception>
#include <fstream>
#include "../lexer/lexer.h"
#include "../parser/parser.h"
#include "../evaluator/evaluator.h"
#include "../compiler/compiler.h"
#include "../vm/virtualMachine.h"
using namespace std;

class Repl {
public:
    void runWithEvaluator();
    void runWithVM();
    void parserTest();
    void lexerTest();
private:
    Lexer lexer;
    Parser parser;
    Evaluator evaluator;
    Compiler compiler;
    VirtualMachine vm;

    static string readInputFile();
};



#endif //TOLELOM_REPL_H
