#include "compiler.h"

Bytecode Compiler::run(Node* node) {
    // Compiler init <- 여기에 적는 것이 맞을까?
	CompilationScope * mainScope = new CompilationScope;
	symbolTable = new SymbolTable;
	scopes.push_back(mainScope);
	scopeIndex = 0;

	try {
        compile(node);
    }
    catch (const exception& e) {
        cout << e.what() << endl;
    }

    return Bytecode{currentInstructions(), constants};
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
    else if (StringLiteral* stringLiteral = dynamic_cast<StringLiteral*>(node)) {
        String* str = new String;
        str->value = stringLiteral->value;
        emit(OpcodeType::OpConstant, vector<int>{addConstant(str)});
    }
    else if (LetStatement* letStatement = dynamic_cast<LetStatement*>(node)) {
        compile(letStatement->expression);
        Symbol symbol = symbolTable->Define(letStatement->name->name);
        if (symbol.scope == GlobalScope) {
			emit(OpcodeType::OpSetGlobal, vector<int>{symbol.index});
		}
		else {
			emit(OpcodeType::OpSetLocal, vector<int>{symbol.index});
		}
    }
    else if (IdentifierExpression* identifierExpression = dynamic_cast<IdentifierExpression*>(node)) {
        Symbol symbol = symbolTable->Resolve(identifierExpression->name); // 오류 생길 수도 처리 필요 할 수도
		if (symbol.scope == GlobalScope) {
			emit(OpcodeType::OpGetGlobal, vector<int>{symbol.index});
		}
		else {
			emit(OpcodeType::OpGetLocal, vector<int>{symbol.index});
		}
    }
	else if (IfStatement* ifExpression = dynamic_cast<IfStatement*>(node)) {
		compile(ifExpression->condition);

		// OpJumpNotTruthy 명령어에 쓰레깃값 9999 널어서 배출
		int jumpNotTruthyPos = emit(OpcodeType::OpJumpNotTruthy, vector<int>{9999});

		compile(ifExpression->consequence);
		int jumpPos = emit(OpcodeType::OpJump, vector<int>{9999});
		int afterConsequencePos = currentInstructions().size();
		changeOperand(jumpNotTruthyPos, afterConsequencePos);

		if(ifExpression->alternative != nullptr){

			compile(ifExpression->alternative);

			afterConsequencePos = currentInstructions().size();
		}
		changeOperand(jumpPos, afterConsequencePos);
	}
	else if (BlockStatement* blockStatement = dynamic_cast<BlockStatement*>(node)){
		for(auto statement : blockStatement->statements){
			compile(statement);
		}
	}
	else if (LoopStatement* loopStatement = dynamic_cast<LoopStatement*>(node)){
		if(loopStatement->initialization != nullptr){ // for loop initialization
			compile(loopStatement->initialization);
		}
		int condPos; // condition evaluation 시작점
		if(scopes[scopeIndex]->lastInstruction == nullptr){ // loop 명령어가 가장 앞머리일 경우
			condPos = 0;
		}
		else{
			condPos = scopes[scopeIndex]->lastInstruction->position + 1;
		}
		compile(loopStatement->condition);

		// back-patching
		int jumpNotTruthyPos = emit(OpcodeType::OpJumpNotTruthy, vector<int>{9999});

		compile(loopStatement->loopBody);
		if(loopStatement->incrementation != nullptr){
			compile(loopStatement->incrementation);
		}
		emit(OpcodeType::OpJump, vector<int>{condPos});
		int afterLoopPos = currentInstructions().size();
		changeOperand(jumpNotTruthyPos, afterLoopPos);
	}
    else if (ArrayLiteral* arrayLiteral = dynamic_cast<ArrayLiteral*>(node)) {
        for (auto element : arrayLiteral->elements) {
            compile(element);
        }

        emit(OpcodeType::OpArray, vector<int>{(int)arrayLiteral->elements.size()});
    }
    else if (IndexExpression* indexExpression = dynamic_cast<IndexExpression*>(node)) {
        compile(indexExpression->left);

        compile(indexExpression->index);

        emit(OpcodeType::OpIndex);
    }
	else if (FunctionLiteral* functionLiteral = dynamic_cast<FunctionLiteral*>(node)) {
		enterScope();

		compile(functionLiteral->blockStatement);

		if(!lastInstructionIs(OpcodeType::OpReturnValue)) {
			emit(OpcodeType::OpReturn);
		}

		int numLocals = symbolTable->numberDefinitions;
		vector<Instruction *> instructions = leaveScope();
		CompiledFunction * compiledFn = new CompiledFunction; // 생성자가 안먹히는 이유는?
		compiledFn->instructions = instructions;
		compiledFn->numLocals = numLocals;

		// 함수 리터럴을 배출
		emit(OpcodeType::OpConstant, vector<int>(addConstant(compiledFn)));

		// 함수 이름과 literal assign 과정
		Symbol symbol = symbolTable->Define(functionLiteral->name->name);
		emit(OpcodeType::OpSetGlobal, vector<int>{symbol.index});
	}
	else if (ReturnStatement* returnStatement = dynamic_cast<ReturnStatement*>(node)) {
		compile(returnStatement->returnValue);

		emit(OpcodeType::OpReturnValue);
	}
	else if (FunctionExpression* functionExpression = dynamic_cast<FunctionExpression*>(node)) {
		compile(functionExpression->functionBody);

		emit(OpcodeType::OpCall);
	}
}

Bytecode Compiler::ReturnBytecode() {
    return Bytecode{currentInstructions(), constants};
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
    int posNewInstruction = currentInstructions().size();
	currentInstructions().push_back(instruction);

    return posNewInstruction;
}

void Compiler::setLastInstruction(OpcodeType opcode, int position) {
	EmittedInstruction* previous = scopes[scopeIndex]->lastInstruction;
	EmittedInstruction* last = new EmittedInstruction{opcode, position};

	delete scopes[scopeIndex]->previousInstruction;
	scopes[scopeIndex]->previousInstruction = previous;
	scopes[scopeIndex]->lastInstruction = last;
}

bool Compiler::lastInstructionIs(OpcodeType opcode) {
	if(currentInstructions().size() == 0){
		return false;
	}

	return scopes[scopeIndex]->lastInstruction->opcode == opcode;
}

void Compiler::removeLastInstruction() {
	// instructions.pop_back(); // remove last instruction(OpPop)
}

void Compiler::replaceInstruction(int position, Instruction *newInstruction) {
	currentInstructions().at(position) = newInstruction;
}

void Compiler::changeOperand(int opPos, int operand) {
	OpcodeType opcode = OpcodeType(currentInstructions().at(opPos)->at(0));
	Instruction* newInstruction = code.makeInstruction(opcode, vector<int>{operand});

	replaceInstruction(opPos, newInstruction);
}

vector<Instruction*>& Compiler::currentInstructions() {
	return scopes[scopeIndex]->instructions;
}

void Compiler::enterScope() {
	CompilationScope* scope = new CompilationScope;
	scopes.push_back(scope);
	scopeIndex++;

	symbolTable = SymbolTable::NewEnclosedSymbolTable(symbolTable);
}

vector<Instruction *> Compiler::leaveScope() {
	auto instructions = currentInstructions();
	scopes.pop_back();
	scopeIndex--;



	return instructions;
}
