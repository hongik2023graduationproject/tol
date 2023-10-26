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
    OpPop,
    OpAdd,
    OpSub,
    OpMul,
    OpDiv,
    OpTrue,
    OpFalse,
    OpEqual,
    OpNotEqual,
    OpLessThan,
    OpMinus,
    OpBang,
    OpSetGlobal,
    OpGetGlobal,
    OpJumpNotTruthy,
    OpJump,
    OpArray,
    OpIndex,
	OpCall,
	OpReturnValue,
	OpReturn,
	OpSetLocal,
	OpGetLocal,

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
            {OpcodeType::OpPop, Definition{"OpPop", vector<int>{}}},
            {OpcodeType::OpAdd, Definition{"OpAdd", vector<int>{}}},
            {OpcodeType::OpSub, Definition{"ObSub", vector<int>{}}},
            {OpcodeType::OpMul, Definition{"ObSub", vector<int>{}}},
            {OpcodeType::OpDiv, Definition{"ObSub", vector<int>{}}},
            {OpcodeType::OpTrue, Definition{"OpTrue", vector<int>{}}},
            {OpcodeType::OpFalse, Definition{"OpFalse", vector<int>{}}},
            {OpcodeType::OpEqual, Definition{"OpEqual", vector<int>{}}},
            {OpcodeType::OpNotEqual, Definition{"OpNotEqual", vector<int>{}}},
            {OpcodeType::OpLessThan, Definition{"OpLessThan", vector<int>{}}},
            {OpcodeType::OpMinus, Definition{"OpMinus", vector<int>{}}},
            {OpcodeType::OpBang, Definition{"OpBang", vector<int>{}}},
            {OpcodeType::OpSetGlobal, Definition{"OpSetGlobal", vector<int>{4}}},
            {OpcodeType::OpGetGlobal, Definition{"OpGetGlobal", vector<int>{4}}},
            {OpcodeType::OpJumpNotTruthy, Definition{"OpJumpNotTruthy", vector<int>{4}}},
            {OpcodeType::OpJump, Definition{"OpJump", vector<int>{4}}},
            {OpcodeType::OpArray, Definition{"OpArray", vector<int>{2}}}, // 배열의 최대 크기 65535
            {OpcodeType::OpIndex, Definition{"OpIndex", vector<int>{}}},
			{OpcodeType::OpCall, Definition{"OpCall", vector<int>{}}},
			{OpcodeType::OpReturnValue, Definition{"OpReturnValue", vector<int>{}}},
			{OpcodeType::OpReturn, Definition{"OpReturn", vector<int>{}}},
			{OpcodeType::OpSetLocal, Definition{"OpSetLocal", vector<int>{1}}},
			{OpcodeType::OpGetLocal, Definition{"OpGetLocal", vector<int>{1}}},



	};
};


#endif //TOLELOM_CODE_H
