#include "virtualMachine.h"

void VirtualMachine::testIntegerObject(long long expected, Object* object) {
    if (Integer* integer = dynamic_cast<Integer*>(object)) {
        if (integer->value != expected) {
            throw invalid_argument("");
        }
        // ok

    }
    else {
        throw invalid_argument("");
    }
}

void VirtualMachine::run(Bytecode bytecode) {
    // init
    instructions = bytecode.instructions;
    constants = bytecode.constants;
    stack.resize(StackSize);
    stackPointer = 0;

    for (int ip = 0; ip < int(instructions.size()); ++ip) { // ip = instruction pointer
        OpcodeType opcode = static_cast<OpcodeType>(int((*instructions[ip])[0]));

        if (opcode == OpcodeType::OpConstant) {
            int constIndex = endian.byteToInt(vector<byte>(instructions[ip]->begin() + 1, instructions[ip]->begin() + 5));
            push(constants[constIndex]);
        }
        else if (opcode == OpcodeType::OpAdd) {
            Object* right = pop();
            Object* left = pop();

            Integer* rightInteger = dynamic_cast<Integer*>(right);
            Integer* leftInteger = dynamic_cast<Integer*>(left);

            Integer* resultInteger = new Integer;
            resultInteger->value = leftInteger->value + rightInteger->value;
            push(resultInteger);
        }
        else if (opcode == OpcodeType::OpPop) {
            pop();
        }
    }
}


Object* VirtualMachine::stackTop() {
    if (stackPointer == 0) {
        throw invalid_argument("");
    }

    return stack[stackPointer - 1];
}

void VirtualMachine::push(Object *object) {
    if (stackPointer >= StackSize) {
        throw invalid_argument("");
    }

    stack[stackPointer] = object;
    stackPointer++;
}

Object* VirtualMachine::pop() {
    Object* object = stack[stackPointer - 1];
    stackPointer--;

    return object;
}

Object* VirtualMachine::lastPoppedElement() {
    return stack[stackPointer];
}