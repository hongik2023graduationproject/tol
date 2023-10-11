#include "compiler.h"

Bytecode* Compiler::run(Node* node) {
    try {
        compile(node);
    }
    catch (const exception& e) {
        cout << e.what() << endl;
    }
    return nullptr;
}

void Compiler::compile(Node *node) {
    if (Program* program = dynamic_cast<Program*>(node)) {
        for (auto statement : program->statements) {
            compile(statement);
        }
    }
    else if (ExpressionStatement* expressionStatement = dynamic_cast<ExpressionStatement*>(node)) {
        compile(expressionStatement->expression);
    }
    else if (InfixExpression* infixExpression = dynamic_cast<InfixExpression*>(node)) {
        compile(infixExpression->left);
    }
    else if (IntegerLiteral* integerLiteral = dynamic_cast<IntegerLiteral*>(node)) {
         Integer* integer  = new Integer;
         integer->value = integerLiteral->value;
         emit(OpcodeType::OpConstant, vector<int>{addConstant(integer)});
    }
}

BytecodeTemp Compiler::ReturnBytecode() {
    return BytecodeTemp{instructions, constants};
}

int Compiler::addConstant(Object* object) {
    constants.push_back(object);
    return constants.size() - 1;
}

int Compiler::emit(OpcodeType opcode, vector<int> operands) {
    Instruction* instruction = code.makeInstruction(opcode, operands);
    int pos = addInstruction(instruction);
    return pos;
}

int Compiler::addInstruction(Instruction* instruction) {
    int posNewInstruction = instructions.size();
    instructions.push_back(instruction);
    return posNewInstruction;
}