#include "compiler.h"

Bytecode Compiler::run(Node* node) {
	CompilationScope* mainScope = new CompilationScope;
	symbolTable = new SymbolTable;
	Builtins builtins;
	for (int i = 0; i < builtins.builtinList.size(); i++){
		symbolTable->DefineBuiltin(i, builtins.builtinList[i].name);
	}

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
        for (Statement* statement : program->statements) {
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
         Integer* integer  = new Integer(integerLiteral->value);
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
        String* str = new String(stringLiteral->value);
        emit(OpcodeType::OpConstant, vector<int>{addConstant(str)});
    }
    else if (ArrayLiteral* arrayLiteral = dynamic_cast<ArrayLiteral*>(node)) {
        for (auto element : arrayLiteral->elements) {
            compile(element); // 타입 통일하려면 여기서 에러 띄워야 할 듯
        }

        emit(OpcodeType::OpArray, vector<int>{(int)arrayLiteral->elements.size()});
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
		loadSymbol(symbol);
    }
	else if (AssignStatement* assignStatement = dynamic_cast<AssignStatement*>(node)) {
		compile(assignStatement->value);
		Symbol symbol = symbolTable->Resolve(assignStatement->name->name);
		if (symbol.scope == GlobalScope) {
			emit(OpcodeType::OpSetGlobal, vector<int>{symbol.index});
		}
		else {
			emit(OpcodeType::OpSetLocal, vector<int>{symbol.index});
		}
	}
	else if (IfStatement* ifStatement = dynamic_cast<IfStatement*>(node)) { // 이거 최적화 해야 함 JUMP 명령어가 무조건 나오는 건 문제가 있음
 		compile(ifStatement->condition);

		int jumpNotTruthyPos = emit(OpcodeType::OpJumpNotTruthy, vector<int>{9999}); // back-patching
		compile(ifStatement->consequence);
		int jumpPos = emit(OpcodeType::OpJump, vector<int>{9999}); // back-patching

		int afterConsequencePos = (int)currentInstructions().size();
		changeOperand(jumpNotTruthyPos, afterConsequencePos);

		if(ifStatement->alternative != nullptr){
			compile(ifStatement->alternative);
			afterConsequencePos = (int)currentInstructions().size();
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
    else if (IndexExpression* indexExpression = dynamic_cast<IndexExpression*>(node)) {
        compile(indexExpression->left);
        compile(indexExpression->index);
        emit(OpcodeType::OpIndex);
    }
    else if (ClassStatement* classStatement = dynamic_cast<ClassStatement*>(node)) {
        enterScope();
        compile(classStatement->block);
        int numLocalDefine = symbolTable->numberDefinitions;
        vector<Instruction*> instructions = leaveScope();

        CompiledClass* compiledClass = new CompiledClass;
        compiledClass->instructions = instructions;
        compiledClass->numLocalDefine = numLocalDefine;
        compiledClass->name = classStatement->name->name;

        emit(OpcodeType::OpConstant, vector<int>{addConstant(compiledClass)});

        Symbol symbol = symbolTable->Define(classStatement->name->name);
        emit(OpcodeType::OpSetGlobal, vector<int>{symbol.index});
    }
    else if (FunctionStatement* functionStatement = dynamic_cast<FunctionStatement*>(node)) {
        enterScope();

        for(auto parameter : functionStatement->parameters){
            symbolTable->Define(parameter->name);
        }
        compile(functionStatement->blockStatement);
        if(!lastInstructionIs(OpcodeType::OpReturnValue)) {
            emit(OpcodeType::OpReturn);
        }
        int numLocals = symbolTable->numberDefinitions;

        vector<Instruction*> instructions = leaveScope();

        CompiledFunction* compiledFn = new CompiledFunction;
        compiledFn->instructions = instructions;
        compiledFn->numLocals = numLocals;
        compiledFn->numParameters = functionStatement->parameters.size();

        // 함수 리터럴을 배출
        emit(OpcodeType::OpConstant, vector<int>{addConstant(compiledFn)});

        // 함수 이름과 literal assign 과정
        Symbol symbol = symbolTable->Define(functionStatement->name->name);
        emit(OpcodeType::OpSetGlobal, vector<int>{symbol.index});

        if (lastInstructionIs(OpcodeType::OpPop)) { // 함수 정의 이후 결과를 pop하는 것을 방지
            removeLastInstruction();
        }

    }
	else if (ReturnStatement* returnStatement = dynamic_cast<ReturnStatement*>(node)) {
		compile(returnStatement->returnValue);

		emit(OpcodeType::OpReturnValue);
	}
	else if (FunctionExpression* functionExpression = dynamic_cast<FunctionExpression*>(node)) {
		compile(functionExpression->function);

		for(auto arg : functionExpression->arguments) {
			compile(arg);
		}

		emit(OpcodeType::OpCall, vector<int>{static_cast<int>(functionExpression->arguments.size())});
	}
}


int Compiler::addConstant(Object* object) {
    constants.push_back(object);
    auto a = constants.size() - 1;
	return a;
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
    delete scopes[scopeIndex]->previousInstruction;
    scopes[scopeIndex]->previousInstruction = scopes[scopeIndex]->lastInstruction;
    scopes[scopeIndex]->lastInstruction = new EmittedInstruction{opcode, position};
}

bool Compiler::lastInstructionIs(OpcodeType opcode) {
	if(currentInstructions().empty()){
		return false;
	}

	return scopes[scopeIndex]->lastInstruction->opcode == opcode;
}

void Compiler::removeLastInstruction() {
	// instructions.pop_back(); // remove last instruction(OpPop)
}

void Compiler::replaceInstruction(int position, Instruction* newInstruction) {
    vector<Instruction*> instructions = currentInstructions();
	instructions[position] = newInstruction; // 기존 명령어 삭제 필요
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

vector<Instruction*> Compiler::leaveScope() {
	auto instructions = currentInstructions();
	scopes.pop_back();
	scopeIndex--;

	symbolTable = symbolTable->outer;

	return instructions;
}

void Compiler::loadSymbol(Symbol symbol) {
	if (symbol.scope == GlobalScope) {
		emit(OpcodeType::OpGetGlobal, vector<int>{symbol.index});
	}
	else if (symbol.scope == LocalScope) {
		emit(OpcodeType::OpGetLocal, vector<int>{symbol.index});
	}
	else if (symbol.scope == BuiltinScope) {
		emit(OpcodeType::OpGetBuiltin, vector<int>{symbol.index});
	}
}
