#include "compiler.h"

Bytecode Compiler::run(Node* node) {
    try {
        compile(node);
    }
    catch (const exception& e) {
        cout << e.what() << endl;
    }

    return Bytecode{instructions, constants};
}

void Compiler::compile(Node *node) {
    if (Program* program = dynamic_cast<Program*>(node)) {
        for (auto statement : program->statements) {
            compile(statement);
        }
    }
    else if (ExpressionStatement* expressionStatement = dynamic_cast<ExpressionStatement*>(node)) {
        compile(expressionStatement->expression);
        emit(OpcodeType::OpPop);
    }
    else if (InfixExpression* infixExpression = dynamic_cast<InfixExpression*>(node)) {
        compile(infixExpression->left);
        compile(infixExpression->right);

        if (infixExpression->token->tokenType == TokenType::PLUS) {
            emit(OpcodeType::OpAdd);
        }
        else if (infixExpression->token->tokenType == TokenType::MINUS) {
            emit(OpcodeType::OpSub);
        }
        else if (infixExpression->token->tokenType == TokenType::ASTERISK) {
            emit(OpcodeType::OpMul);
        }
        else if (infixExpression->token->tokenType == TokenType::SLASH) {
            emit(OpcodeType::OpDiv);
        }
        else if (infixExpression->token->tokenType == TokenType::LESS_THAN) {
            emit(OpcodeType::OpLessThan);
        }
        else if (infixExpression->token->tokenType == TokenType::EQUAL) {
            emit(OpcodeType::OpEqual);
        }
        else if (infixExpression->token->tokenType == TokenType::NOT_EQUAL) {
            emit(OpcodeType::OpNotEqual);
        }
        else {
            throw invalid_argument("");
        }
    }
    else if (IntegerLiteral* integerLiteral = dynamic_cast<IntegerLiteral*>(node)) {
         Integer* integer  = new Integer;
         integer->value = integerLiteral->value;
         emit(OpcodeType::OpConstant, vector<int>{addConstant(integer)});
    }
    else if (BooleanLiteral* booleanLiteral = dynamic_cast<BooleanLiteral*>(node)) {
        if (booleanLiteral->value) {
            emit(OpcodeType::OpTrue);
        }
        else {
            emit(OpcodeType::OpFalse);
        }
    }
}

Bytecode Compiler::ReturnBytecode() {
    return Bytecode{instructions, constants};
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