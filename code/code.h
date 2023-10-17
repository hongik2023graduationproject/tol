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
	OpSub,
	OpMul,
	OpDiv,
	OpTrue,
	OpFalse,
	OpEqual,
	OpNotEqual,
	OpGreaterThan,

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
			{OpcodeType::OpSub, Definition{"OpSub", vector<int>{}}},
			{OpcodeType::OpMul, Definition{"OpMul", vector<int>{}}},
			{OpcodeType::OpDiv, Definition{"OpDiv", vector<int>{}}},
			{OpcodeType::OpTrue, Definition{"OpTrue", vector<int>{}}},
			{OpcodeType::OpFalse, Definition{"OpFalse", vector<int>{}}},
			{OpcodeType::OpEqual, Definition{"OpEqual", vector<int>{}}},
			{OpcodeType::OpNotEqual, Definition{"OpNotEqual", vector<int>{}}},
			{OpcodeType::OpGreaterThan, Definition{"OpGreaterThan", vector<int>{}}},
	};
};


#endif //TOLELOM_CODE_H
