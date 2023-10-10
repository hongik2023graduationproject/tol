#include "compiler.h"

Bytecode* Compiler::run(Node* node) {
    try {
        compile(node);
    }
    catch (const exception& e) {

    }
    return nullptr;
}

void Compiler::compile(Node *node) {
    if (Program* program = dynamic_cast<Program*>(node)) {
        for (auto statement : program->statements) {
            run(statement);
        }
    }
}

BytecodeTemp Compiler::ReturnBytecode() {
    return BytecodeTemp{instructions, constants};
}