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

enum class OpcodeType {
    OpConstant = 1,
    OpAdd,
    OpPop,


};


class Definition {
public:
    string name;
    vector<int> operandWidths;
};

class Code {
public:
    static void applyOperand(Instruction* instruction, int offset, int size, vector<byte> operand);
    Instruction* makeInstruction(OpcodeType opType, const vector<int>& operands);
    Definition findDefinition(OpcodeType opType);

    map<OpcodeType, Definition> definitions = {
            {OpcodeType::OpConstant, Definition{"OpConstant", vector<int>{4}}},
            {OpcodeType::OpAdd, Definition{"OpAdd", vector<int>{}}},
            {OpcodeType::OpPop, Definition{"OpPop", vector<int>{}}},
    };
};


#endif //TOLELOM_CODE_H
