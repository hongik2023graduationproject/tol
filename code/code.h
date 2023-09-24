#ifndef TOLELOM_CODE_H
#define TOLELOM_CODE_H

#include <string>
#include <map>
#include <format>
#include <vector>
using namespace std;

using Instruction = byte*;
using Opcode = byte;

enum class Opcodes {
    OpConstant,

};

class Definition {
public:
    string name;
    int operandWidths;
};

map<Opcode, Definition> definitions;

Definition findDefinition(Opcode op) {
    if (definitions.find(op) != definitions.end()) {
        return definitions.find(op)->second;
    }
    throw invalid_argument("정의된 opcode가 없습니다.");
}


Instruction makeInstruction(Opcode op, const vector<byte>& operands) {
    Definition definition = findDefinition(op);

    int instructionLength = (int)operands.size() + 1;

    Instruction instruction = new byte[instructionLength];
    instruction[0] = op;

    int offset = 1;
    for (auto& operand : operands) {
        int width = definition.operandWidths;
        switch (width) {
            case 2:
                instruction[offset] = operand;
                break;
        }
        offset += width;
    }

    return instruction;
}

#endif //TOLELOM_CODE_H
