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
        return Bytecode{currentInstructions(), constants};
    }
    catch (const exception& e) {
        cout << e.what() << endl;
    }
    return {};
}

ObjectType Compiler::compile(Node *node) {
    if (Program* program = dynamic_cast<Program*>(node)) {
        for (Statement* statement : program->statements) {
            compile(statement);
        }
    }
    else if (ExpressionStatement* expressionStatement = dynamic_cast<ExpressionStatement*>(node)) {
        compile(expressionStatement->expression);
        emit(OpcodeType::OpPop);
    }
    else if (LetStatement* letStatement = dynamic_cast<LetStatement*>(node)) {
        string type = letStatement->token->literal;
        if (classSet.find(type) != classSet.end()) {
            emit(OpcodeType::OpGetGlobal, vector<int>{classSet.find(type)->second});
        }

        ObjectType expressionType = compile(letStatement->expression);

        // type checking
        letStatementTypeCheck(type, expressionType);


        Symbol symbol = symbolTable->Define(letStatement->name->name, expressionType);
        if (symbol.scope == GlobalScope) {
			emit(OpcodeType::OpSetGlobal, vector<int>{symbol.index});
		}
		else {
			emit(OpcodeType::OpSetLocal, vector<int>{symbol.index});
		}
    }
	else if (AssignStatement* assignStatement = dynamic_cast<AssignStatement*>(node)) {
		ObjectType valueType = compile(assignStatement->value);
		Symbol symbol = symbolTable->Resolve(assignStatement->name->name);

        if (symbol.type != valueType) {
            throw invalid_argument("assign statement 변수와 값의 타입이 다릅니다.");
        }

		if (symbol.scope == GlobalScope) {
			emit(OpcodeType::OpSetGlobal, vector<int>{symbol.index});
		}
		else {
			emit(OpcodeType::OpSetLocal, vector<int>{symbol.index});
		}
	}
	else if (IfStatement* ifStatement = dynamic_cast<IfStatement*>(node)) { // 이거 최적화 해야 함 JUMP 명령어가 무조건 나오는 건 문제가 있음
 		ObjectType conditionType = compile(ifStatement->condition);
        if (conditionType != ObjectType::BOOLEAN) {
            throw invalid_argument("if statement, condition 값이 bool 값이 아닙니다.");
        }

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
		if (loopStatement->initialization != nullptr) { // for loop initialization
			compile(loopStatement->initialization);
		}
		int condPos; // condition evaluation 시작점
		if (scopes[scopeIndex]->lastInstruction == nullptr) { // loop 명령어가 가장 앞머리일 경우
			condPos = 0;
		}
		else {
			condPos = scopes[scopeIndex]->lastInstruction->position + 1;
		}
		ObjectType conditionType =compile(loopStatement->condition);
        if (conditionType != ObjectType::BOOLEAN) {
            throw invalid_argument("loop의 condition이 bool 타입이 아닙니다.");
        }

		// back-patching
		int jumpNotTruthyPos = emit(OpcodeType::OpJumpNotTruthy, vector<int>{9999});

		compile(loopStatement->loopBody);
		if (loopStatement->incrementation != nullptr) {
			compile(loopStatement->incrementation);
		}
		emit(OpcodeType::OpJump, vector<int>{condPos});
		int afterLoopPos = currentInstructions().size();
		changeOperand(jumpNotTruthyPos, afterLoopPos);
	}
    // 이 아래로는 타입 체크 아직 안 봄
    else if (ClassStatement* classStatement = dynamic_cast<ClassStatement*>(node)) {
        // compile class statement
        enterScope();
//        compile(classStatement->block); // 이걸 바꿔야 되나 아니면 체킹을 해야되나
        int numLocalDefine = symbolTable->numberDefinitions;
        vector<Instruction*> instructions = leaveScope();

        auto* compiledClass = new CompiledClass(classStatement->name->name, instructions, numLocalDefine);
        emit(OpcodeType::OpConstant, vector<int>{addConstant(compiledClass)});

        // binding
        Symbol symbol = symbolTable->Define(classStatement->name->name, ObjectType::COMPILED_CLASS);
        classSet[classStatement->name->name] = symbol.index; // 테스팅 중
        emit(OpcodeType::OpSetGlobal, vector<int>{symbol.index});
    }
    else if (FunctionStatement* functionStatement = dynamic_cast<FunctionStatement*>(node)) {
        // compile function statement
        enterScope();
        for(auto parameter : functionStatement->parameters){
//            임시로 테스트 위해서 INTEGER로 해놓음
            symbolTable->Define(parameter->name, ObjectType::INTEGER);
        }
        compile(functionStatement->blockStatement);
        if(!lastInstructionIs(OpcodeType::OpReturnValue)) {
            emit(OpcodeType::OpReturn);
        }
        int numLocals = symbolTable->numberDefinitions;
        vector<Instruction*> instructions = leaveScope();

        CompiledFunction* compiledFn = new CompiledFunction(instructions, numLocals, functionStatement->parameters.size());
        emit(OpcodeType::OpConstant, vector<int>{addConstant(compiledFn)});

        // 함수 이름과 literal assign 과정
        Symbol symbol = symbolTable->Define(functionStatement->name->name, ObjectType::COMPILED_FUNCTION);
        emit(OpcodeType::OpSetGlobal, vector<int>{symbol.index});

        if (lastInstructionIs(OpcodeType::OpPop)) { // 함수 정의 이후 결과를 pop하는 것을 방지
            removeLastInstruction();
        }
    }
    else if (ReturnStatement* returnStatement = dynamic_cast<ReturnStatement*>(node)) { // 리턴 값의 체크도 필요한데 아직 미완성
        compile(returnStatement->returnValue);
        emit(OpcodeType::OpReturnValue);
    }
    else if (ClassInitStatement* classInitStatement = dynamic_cast<ClassInitStatement*>(node)) {
        Symbol symbol = symbolTable->Define(classInitStatement->name->name, ObjectType::COMPILED_CLASS);
        if (symbol.scope == GlobalScope) {
            emit(OpcodeType::OpSetGlobal, vector<int>{symbol.index});
        }
        else {
            emit(OpcodeType::OpSetLocal, vector<int>{symbol.index});
        }
    }


    else if (IntegerLiteral* integerLiteral = dynamic_cast<IntegerLiteral*>(node)) {
        Integer* integer  = new Integer(integerLiteral->value);
        emit(OpcodeType::OpConstant, vector<int>{addConstant(integer)});
        return ObjectType::INTEGER;
    }
    else if (BooleanLiteral* booleanLiteral = dynamic_cast<BooleanLiteral*>(node)) {
        if (booleanLiteral->value) {
            emit(OpcodeType::OpTrue);
        }
        else {
            emit(OpcodeType::OpFalse);
        }
        return ObjectType::BOOLEAN;
    }
    else if (StringLiteral* stringLiteral = dynamic_cast<StringLiteral*>(node)) {
        String* str = new String(stringLiteral->value);
        emit(OpcodeType::OpConstant, vector<int>{addConstant(str)});
        return ObjectType::STRING;
    }
    else if (ArrayLiteral* arrayLiteral = dynamic_cast<ArrayLiteral*>(node)) {
        for (auto element : arrayLiteral->elements) {
            compile(element); // 타입 통일하려면 여기서 에러 띄워야 할 듯
        }
        emit(OpcodeType::OpArray, vector<int>{(int)arrayLiteral->elements.size()});
    }


    else if (PrefixExpression* prefixExpression = dynamic_cast<PrefixExpression*>(node)) {
        ObjectType rightValueType = compile(prefixExpression->right);

        switch (prefixExpression->token->tokenType) {
            case TokenType::BANG:
                emit(OpcodeType::OpBang);
                return ObjectType::BOOLEAN;
            case TokenType::MINUS:
                emit(OpcodeType::OpMinus);
                return rightValueType;
            default:
                throw invalid_argument("정의되지 않은 prefix 연산자입니다.");
        }
    }
    else if (InfixExpression* infixExpression = dynamic_cast<InfixExpression*>(node)) {
        ObjectType leftValueType = compile(infixExpression->left);
        ObjectType rightValueType = compile(infixExpression->right);

        if (leftValueType != rightValueType) { // 중요: 나중에 다른 타입간의 연산도 허용해야 함 (아래 포함 수정)
            throw invalid_argument("infix 연산자의 좌항과 우항의 타입이 일치하지 않습니다.");
        }

        if (infixExpression->token->tokenType == TokenType::PLUS) {
            emit(OpcodeType::OpAdd);
            return leftValueType;
        }
        else if (infixExpression->token->tokenType == TokenType::MINUS) {
            emit(OpcodeType::OpSub);
            return leftValueType;
        }
        else if (infixExpression->token->tokenType == TokenType::ASTERISK) {
            emit(OpcodeType::OpMul);
            return leftValueType;
        }
        else if (infixExpression->token->tokenType == TokenType::SLASH) {
            emit(OpcodeType::OpDiv);
            return leftValueType;
        }
        else if (infixExpression->token->tokenType == TokenType::LESS_THAN) {
            emit(OpcodeType::OpLessThan);
            return ObjectType::BOOLEAN;
        }
		else if (infixExpression->token->tokenType == TokenType::GREATER_THAN) {
			emit(OpcodeType::OpGreaterThan);
			return ObjectType::BOOLEAN;
		}else if (infixExpression->token->tokenType == TokenType::LESS_EQUAL) {
			emit(OpcodeType::OpLessEqual);
			return ObjectType::BOOLEAN;
		}else if (infixExpression->token->tokenType == TokenType::GREATER_EQUAL) {
			emit(OpcodeType::OpGreaterEqual);
			return ObjectType::BOOLEAN;
		}
        else if (infixExpression->token->tokenType == TokenType::EQUAL) {
            emit(OpcodeType::OpEqual);
            return ObjectType::BOOLEAN;
        }
        else if (infixExpression->token->tokenType == TokenType::NOT_EQUAL) {
            emit(OpcodeType::OpNotEqual);
            return ObjectType::BOOLEAN;
        }
        else {
            throw invalid_argument("정의되지 않은 infix 연산자 입니다.");
        }
    }
    else if (IdentifierExpression* identifierExpression = dynamic_cast<IdentifierExpression*>(node)) {
        // 중요: 정의되지 않은 변수를 참조할 때 오류 발생해야 함
        Symbol symbol = symbolTable->Resolve(identifierExpression->name);
        loadSymbol(symbol);
        return symbol.type;
    }
    else if (IndexExpression* indexExpression = dynamic_cast<IndexExpression*>(node)) {
        ObjectType type = compile(indexExpression->left);
        compile(indexExpression->index);
        emit(OpcodeType::OpIndex);
        return type;
    }
	else if (FunctionExpression* functionExpression = dynamic_cast<FunctionExpression*>(node)) {
		compile(functionExpression->function);
		for (auto arg : functionExpression->arguments) {
			compile(arg);
		}
		emit(OpcodeType::OpCall, vector<int>{static_cast<int>(functionExpression->arguments.size())});

        // 중요: 함수의 리턴 값에 대한 형식을 리턴해야 함
        return ObjectType::INTEGER;
	}
    else if (ClassExpression* classExpression = dynamic_cast<ClassExpression*>(node)) {
        for (auto initalField : classExpression->statements) { // 여기서 좀 더 세부적인 처리가 필요?
            compile(initalField);
        }

        emit(OpcodeType::OpMakeClass, vector<int>{static_cast<int>(classExpression->statements.size())});

        return ObjectType::COMPILED_CLASS;
    }
    else {
        throw invalid_argument("알려지지 않은 오류 발생.");
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
    currentInstructions()[position] = newInstruction; // 기존 명령어 삭제 필요
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

void Compiler::letStatementTypeCheck(std::string name, ObjectType valueType) {
    if (name == "정수" && valueType == ObjectType::INTEGER) {}
    else if (name == "문자" && valueType == ObjectType::STRING) {}
    else if (classSet.find(name) != classSet.end()) {}
    else {
        throw invalid_argument("let statement의 타입과 값의 타입이 일치하지 않습니다.");
    }
}