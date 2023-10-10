#ifndef TOLELOM_COMPILER_H
#define TOLELOM_COMPILER_H

#include <vector>
#include "../code/code.h"
#include "../object/object.h"
#include "../ast/node.h"
#include "../ast/program.h"
#include "../endian/endian.h"


// 컴파일러가 만들어낸 Instructions와 컴파일러가 평가한 Constants를 담는다.
class BytecodeTemp {
public:
    vector<Instruction> instructions;
    vector<Object*> constants;
};


class Compiler {
public:
    Endian endian;
    Bytecode* run(Node* node);

    Code code;
    vector<Instruction> instructions;
    vector<Object*> constants;
private:
    void compile(Node* node);
    BytecodeTemp ReturnBytecode();
};


#endif //TOLELOM_COMPILER_H
