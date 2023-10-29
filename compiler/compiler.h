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

class Bytecode {
public:
    vector<Instruction*> instructions;
    vector<Object*> constants;
};

// 10.28 김성민: if문이 statement인데 이게 필요한가..?
class EmittedInstruction{
public:
	OpcodeType opcode;
	int position;
};


class CompilationScope {
public:
	vector<Instruction*> instructions;
	EmittedInstruction* lastInstruction;
    EmittedInstruction* previousInstruction;
};


/*  NOTE
 *  AST를 한 번만 순회할 필요는 없다.
 *  조사 처리할 때 한 번 고려해보자.
 *
 */
class Compiler {
public:
    Bytecode run(Node* node);

    Code code;
    vector<Object*> constants;
private:
    SymbolTable* symbolTable;
	vector<CompilationScope*> scopes;
	int scopeIndex;

    void compile(Node* node);
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
