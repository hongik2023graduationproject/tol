#ifndef TOLELOM_CODE_H
#define TOLELOM_CODE_H

#include <string>
#include <map>
#include <format>
#include <vector>
#include <memory>

#include "../endian/endian.h"


using namespace std;
using Instruction = vector<byte>;
using Opcode = byte;
using Bytecode = vector<Instruction>;

enum class OpcodeType {
    OpConstant = 1,
};


// 도움 기능
class Definition {
public:
    string name;
    vector<int> operandWidths;
};

map<OpcodeType, Definition> definitions = {
        {OpcodeType::OpConstant, Definition{"OpConstant", vector<int>{4}}},
};

Definition findDefinition(OpcodeType opType) {
    if (definitions.find(opType) != definitions.end()) {
        return definitions.find(opType)->second;
    }
    throw invalid_argument("정의된 opcode가 없습니다.");
}

void applyOperand(Instruction* instruction, int offset, int size, vector<byte> operand) {
    for (int point = 0; point < size; ++point)
        (*instruction)[point + offset] = operand[point];
}

Instruction* makeInstruction(OpcodeType opType, const vector<int>& operands) {
    Endian endian{};
    Definition definition = findDefinition(opType); // throw

    int instructionLength = 1;
    for (auto operandWidth : definition.operandWidths) {
        instructionLength += operandWidth;
    }

    Instruction* instruction = new Instruction(instructionLength);
    (*instruction)[0] = static_cast<byte>(opType);

    int offset = 1;
    for (int i = 0; i < (int)operands.size(); ++i) {
        int width = definition.operandWidths[i];
        applyOperand(instruction, offset, width, endian.intToByte(operands[i]));
        offset += width;
    }

    return instruction;
}


long long readOperands(Definition definition, Instruction instruction) {

}

#endif //TOLELOM_CODE_H
