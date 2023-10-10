#include "compiler.h"

Bytecode* Compiler::run(Node* node) {
    return nullptr;
}

BytecodeTemp Compiler::ReturnBytecode() {
    return BytecodeTemp{instructions, constants};
}