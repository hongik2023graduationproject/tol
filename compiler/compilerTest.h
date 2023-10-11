#ifndef TOLELOM_COMPILERTEST_H
#define TOLELOM_COMPILERTEST_H

#include <string>
#include "../lexer/lexer.h"
#include "../parser/parser.h"
#include "compiler.h"
#include "../code/code.h"

using namespace std;

class CompilerTest {
public:
    string input;
    vector<Instruction*> expectedInstructions;
    Lexer lexer;
    Parser parser;
    Compiler compiler;

    void run();
    void testInstructions(vector<Instruction*> instructions, vector<Instruction*> expectedInstructions);

};



class TestIntegerArithmetic : public CompilerTest {
public:
    string input = "1 + 2";
    vector<Instruction*> expectedInstructions {
            compiler.code.makeInstruction(OpcodeType::OpConstant, vector<int>{0}),
            compiler.code.makeInstruction(OpcodeType::OpConstant, vector<int>{1}),
    };
};


#endif //TOLELOM_COMPILERTEST_H
