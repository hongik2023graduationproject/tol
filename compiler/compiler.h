#ifndef TOLELOM_COMPILER_H
#define TOLELOM_COMPILER_H

#include <iostream>
#include <vector>
#include "../code/code.h"
#include "../object/object.h"
#include "../ast/node.h"
#include "../ast/program.h"
#include "../ast/statements/expressionStatement.h"
#include "../ast/expressions/infixExpression.h"
#include "../ast/literals/integerLiteral.h"
#include "../endian/endian.h"

using namespace std;

// 컴파일러가 만들어낸 Instructions와 컴파일러가 평가한 Constants를 담는다.
class Bytecode {
public:
    vector<Instruction*> instructions;
    vector<Object*> constants;
};


class Compiler {
public:
    Endian endian;
    Bytecode run(Node* node);

    Code code;
    vector<Instruction*> instructions;
    vector<Object*> constants;
private:
    void compile(Node* node);
    Bytecode ReturnBytecode();
    int addConstant(Object* object);
    int addInstruction(Instruction* instruction);
    int emit(OpcodeType opcode, vector<int> operands = vector<int>{});
};


#endif //TOLELOM_COMPILER_H
