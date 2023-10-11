#include "compilerTest.h"


void CompilerTest::run() {
    vector<Token*> tokens = lexer.run(input);
    Program* program = parser.run(tokens);
    compiler.run(program);

    testInstructions(compiler.instructions, expectedInstructions);
}


void CompilerTest::testInstructions(vector<Instruction*> instructions, vector<Instruction*> expectedInstructions) {
    if (instructions.size() != expectedInstructions.size())
        throw invalid_argument("testInstructions: ");

    for (int i = 0; i < instructions.size(); ++i) {
        if (instructions[i] != expectedInstructions[i]) {
            throw invalid_argument("testInstructions: ");
        }
    }

    return;
}