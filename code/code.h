#ifndef TOLELOM_CODE_H
#define TOLELOM_CODE_H

#include <string>
#include <map>
#include <format>
#include <vector>
#include <memory>

#include "../endian/endian.h"
#include "../object/object.h"

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
	OpGreaterThan,
	OpLessEqual,
	OpGreaterEqual,
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
	OpGetBuiltin,
    OpMakeClass,
};


class Definition {
public:
    string name;
    vector<int> operandWidths;
};

class Code {
public:
    Endian endian;

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
			{OpcodeType::OpCall, Definition{"OpCall", vector<int>{4}}},
			{OpcodeType::OpReturnValue, Definition{"OpReturnValue", vector<int>{}}},
			{OpcodeType::OpReturn, Definition{"OpReturn", vector<int>{}}},
			{OpcodeType::OpSetLocal, Definition{"OpSetLocal", vector<int>{4}}},
			{OpcodeType::OpGetLocal, Definition{"OpGetLocal", vector<int>{4}}},
			{OpcodeType::OpGetBuiltin, Definition{"OpGetBuiltin", vector<int>{4}}},
            {OpcodeType::OpMakeClass, Definition{"OpMakeClass", vector<int>{4}}}, // 임시 4
	};

    void decodeInstruction(vector<Instruction*> instructions, vector<Object*> constants) {
        for (int i = 0; i < (int)instructions.size(); ++i) {
            Instruction* instruction = instructions[i];
            Definition def = findDefinition(static_cast<OpcodeType>((*instruction)[0]));

            string s = to_string(i) + ": " + def.name;
            for (int i = 0; i < (int) def.operandWidths.size(); ++i) {
                int operand = endian.byteToInt(vector<byte>(instruction->begin() + 1, instruction->begin() + 1 + def.operandWidths[i]));


                s += " " + to_string(operand);
                if (def.name == "OpConstant")
                    s += ": " + constants[operand]->print();
                if (def.name == "OpGetBuiltin") {

                }
            }

            cout << s << endl;
        }
    }
};


#endif //TOLELOM_CODE_H
