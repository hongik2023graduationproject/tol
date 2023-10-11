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

    }
}

BytecodeTemp Compiler::ReturnBytecode() {
    return BytecodeTemp{instructions, constants};
}