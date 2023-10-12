#include "code.h"

void Code::applyOperand(Instruction* instruction, int offset, int size, vector<byte> operand) {
    for (int point = 0; point < size; ++point)
        (*instruction)[point + offset] = operand[point];
}


Instruction* Code::makeInstruction(OpcodeType opType, const vector<int>& operands) {
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

Definition Code::findDefinition(OpcodeType opType) {
    if (definitions.find(opType) != definitions.end()) {
        return definitions.find(opType)->second;
    }
    throw invalid_argument("정의된 opcode가 없습니다.");
}
