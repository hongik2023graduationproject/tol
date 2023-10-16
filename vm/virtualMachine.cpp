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
        else if (opcode == OpcodeType::OpAdd || opcode == OpcodeType::OpSub || opcode == OpcodeType::OpMul || opcode == OpcodeType::OpDiv) {
            executeBinaryOperation(opcode);
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

void VirtualMachine::executeBinaryOperation(OpcodeType opcode) {
    Object* right = pop();
    Object* left = pop();

    if (left->type == ObjectType::INTEGER && right->type == ObjectType::INTEGER) {
        return executeBinaryIntegerOperation(opcode, dynamic_cast<Integer*>(left), dynamic_cast<Integer*>(right));
    }
}

void VirtualMachine::executeBinaryIntegerOperation(OpcodeType opcode, Integer* left, Integer* right) {
    long long leftValue = left->value;
    long long rightValue = right->value;

    long long returnValue;
    switch (opcode) {
        case OpcodeType::OpAdd:
            returnValue = leftValue + rightValue;
            break;
        case OpcodeType::OpSub:
            returnValue = leftValue - rightValue;
            break;
        case OpcodeType::OpMul:
            returnValue = leftValue * rightValue;
            break;
        case OpcodeType::OpDiv:
            returnValue = leftValue / rightValue;
            break;
    }


    push(new Integer{returnValue});
}