#ifndef TOLELOM_COMPILER_H
#define TOLELOM_COMPILER_H

#include <iostream>
#include <vector>
#include "../code/code.h"
#include "../object/object.h"
#include "../ast/node.h"
#include "../ast/program.h"
#include "../ast/statements/expressionStatement.h"
#include "../ast/statements/letStatement.h"
#include "../ast/expressions/infixExpression.h"
#include "../ast/expressions/prefixExpression.h"
#include "../ast/expressions/indexExpression.h"
#include "../ast/expressions/FunctionExpression.h"
#include "../ast/literals/integerLiteral.h"
#include "../ast/literals/booleanLiteral.h"
#include "../ast/literals/stringLiteral.h"
#include "../ast/literals/arrayLiteral.h"
#include "../ast/literals/functionLiteral.h"
#include "../ast/statements/ifStatement.h"
#include "../ast/statements/loopStatement.h"
#include "../ast/statements/returnStatement.h"
#include "../endian/endian.h"
#include "symbolTable.h"
using namespace std;

// 컴파일러가 만들어낸 Instructions와 컴파일러가 평가한 Constants를 담는다.
class Bytecode {
public:
    vector<Instruction*> instructions;
    vector<Object*> constants;
};

class EmittedInstruction{
public:
	OpcodeType opcode;
	int position;
};


class CompilationScope {
public:
	vector<Instruction*> instructions;
	EmittedInstruction* lastInstruction, * previousInstruction;

};

class Compiler {
public:
    Endian endian;
    Bytecode run(Node* node);

    Code code;
//    vector<Instruction*> instructions;
    vector<Object*> constants;
private:
    SymbolTable* symbolTable; // Scope 구현을 위해 포인터로 변경
//	EmittedInstruction* lastInstruction, * previousInstruction;
	vector<CompilationScope*> scopes;
	int scopeIndex;

    void compile(Node* node);
    Bytecode ReturnBytecode();
    int addConstant(Object* object);
    int addInstruction(Instruction* instruction);
    int emit(OpcodeType opcode, vector<int> operands = vector<int>{});
	void setLastInstruction(OpcodeType opcode, int position);
	bool lastInstructionIs(OpcodeType opcode);
	void removeLastInstruction();
	void replaceInstruction(int position, Instruction* newInstruction);
	void changeOperand(int opPos, int operand);
	vector<Instruction*>& currentInstructions();
	void enterScope();
	vector<Instruction*> leaveScope();
};


#endif //TOLELOM_COMPILER_H
