#ifndef TOLELOM_VIRTUALMACHINE_H
#define TOLELOM_VIRTUALMACHINE_H

#include "../lexer/lexer.h"
#include "../parser/parser.h"
#include "../object/object.h"
#include "../code/code.h"
#include "../compiler/compiler.h"
#include "../endian/endian.h"

class VirtualMachine {
public:
    void testIntegerObject(long long expected, Object* object);
    void run(Bytecode bytecode);
private:
    vector<Object*> constants;
    vector<Instruction*> instructions;

    vector<Object*> stack;
    int stackPointer;

    const int StackSize = 2048;
    Endian endian;

    Object* stackTop();
    void push(Object* object);
};


#endif //TOLELOM_VIRTUALMACHINE_H
