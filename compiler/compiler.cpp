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
    else if (PrefixExpression* prefixExpression = dynamic_cast<PrefixExpression*>(node)) {
        compile(prefixExpression->right);

        switch (prefixExpression->token->tokenType) {
            case TokenType::BANG:
                emit(OpcodeType::OpBang);
                break;
            case TokenType::MINUS:
                emit(OpcodeType::OpMinus);
                break;
            default:
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
    else if (LetStatement* letStatement = dynamic_cast<LetStatement*>(node)) {
        compile(letStatement->expression);
        Symbol symbol = symbolTable.Define(letStatement->name->name);
        emit(OpcodeType::OpSetGlobal, vector<int>{symbol.index});
    }
    else if (IdentifierExpression* identifierExpression = dynamic_cast<IdentifierExpression*>(node)) {
        Symbol symbol = symbolTable.Resolve(identifierExpression->name); // 오류 생길 수도 처리 필요 할 수도
        emit(OpcodeType::OpGetGlobal, vector<int>{symbol.index});
    }
	else if (IfExpression* ifExpression = dynamic_cast<IfExpression*>(node)) {
		compile(ifExpression->condition);

		// OpJumpNotTruthy 명령어에 쓰레깃값 9999 널어서 배출
		int jumpNotTruthyPos = emit(OpcodeType::OpJumpNotTruthy, vector<int>{9999});

		compile(ifExpression->consequence);

		int afterConsequencePos = instructions.size();
		changeOperand(jumpNotTruthyPos, afterConsequencePos);

		if(ifExpression->alternative != nullptr){
			int jumpPos = emit(OpcodeType::OpJump, vector<int>{9999});

			compile(ifExpression->alternative);

			afterConsequencePos = instructions.size();
			changeOperand(jumpPos, afterConsequencePos);
		}
	}
	else if (BlockStatement* blockStatement = dynamic_cast<BlockStatement*>(node)){
		for(auto statement : blockStatement->statements){
			compile(statement);
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

	setLastInstruction(opcode, pos);

    return pos;
}

int Compiler::addInstruction(Instruction* instruction) {
    int posNewInstruction = instructions.size();
    instructions.push_back(instruction);
    return posNewInstruction;
}

void Compiler::setLastInstruction(OpcodeType opcode, int position) {
	EmittedInstruction* previous = lastInstruction;
	EmittedInstruction* last = new EmittedInstruction{opcode, position};

	delete previousInstruction;
	previousInstruction = previous;
	lastInstruction = last;
}

bool Compiler::lastInstructionIsPop() {
	return lastInstruction->opcode == OpcodeType::OpPop;
}

void Compiler::removeLastInstruction() {
	instructions.pop_back(); // remove last instruction(OpPop)
}

void Compiler::replaceInstruction(int position, Instruction *newInstruction) {
	instructions[position] = newInstruction;
}

void Compiler::changeOperand(int opPos, int operand) {
	OpcodeType opcode = OpcodeType(instructions[opPos]->at(0));
	Instruction* newInstruction = code.makeInstruction(opcode, vector<int>{operand});

	replaceInstruction(opPos, newInstruction);
}
