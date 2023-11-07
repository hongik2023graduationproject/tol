#include "virtualMachine.h"

void VirtualMachine::init(Bytecode& bytecode) {
    CompiledFunction* mainFunction = new CompiledFunction(bytecode.instructions);
    Frame* mainFrame = new Frame(mainFunction, 0);
    constants = bytecode.constants;
    stack.resize(StackSize);
    stackPointer = 0;
    globals.resize(GlobalsSize);
    frames.resize(MaxFrames);
    frameIndex = 0;
    pushFrame(mainFrame);
}


void VirtualMachine::run(Bytecode bytecode) {
    init(bytecode);

    while (currentFrame()->ip < (int)(currentFrame()->Instructions().size() -1)) { // ip = instruction pointer
        int& ip = currentFrame()->ip;
        ip++;
		vector<Instruction*> instructions = currentFrame()->Instructions();
		OpcodeType opcode = static_cast<OpcodeType>(int((*instructions[ip])[0]));

        if (opcode == OpcodeType::OpConstant) {
            int constIndex = endian.byteToInt(vector<byte>(instructions[ip]->begin() + 1, instructions[ip]->begin() + 5));
            push(constants[constIndex]);
        }
        else if (opcode == OpcodeType::OpAdd || opcode == OpcodeType::OpSub || opcode == OpcodeType::OpMul || opcode == OpcodeType::OpDiv) {
            executeBinaryOperation(opcode);
        }
        else if (opcode == OpcodeType::OpEqual || opcode == OpcodeType::OpNotEqual || opcode == OpcodeType::OpLessThan) {
            executeComparison(opcode);
        }
        else if (opcode == OpcodeType::OpPop) {
            pop();
        }
        else if (opcode == OpcodeType::OpTrue) {
            push(TRUE);
        }
        else if (opcode == OpcodeType::OpFalse) {
            push(FALSE);
        }
        else if (opcode == OpcodeType::OpBang) {
            executeBangOperator();
        }
        else if (opcode == OpcodeType::OpMinus) {
            executeMinusOperator();
        }
        else if (opcode == OpcodeType::OpSetGlobal) {
            int globalIndex = endian.byteToInt(vector<byte>(instructions[ip]->begin() + 1, instructions[ip]->begin() + 5));
            globals[globalIndex] = pop();
        }
        else if (opcode == OpcodeType::OpGetGlobal) {
            int globalIndex = endian.byteToInt(vector<byte>(instructions[ip]->begin() + 1, instructions[ip]->begin() + 5));
            push(globals[globalIndex]);
        }
        else if (opcode == OpcodeType::OpArray) {
            int numElements = endian.byteToInt(vector<byte>(instructions[ip]->begin() + 1, instructions[ip]->begin() + 5));

            Object* array = buildArray(stackPointer - numElements, stackPointer);
            stackPointer -= numElements;

            push(array);
        }
        else if (opcode == OpcodeType::OpIndex) {
            Object* index = pop();
            Object* left  = pop();
            executeIndexExpression(left, index);
        }
		else if (opcode == OpcodeType::OpJump) {
			int position = endian.byteToInt(vector<byte>(instructions[ip]->begin() + 1, instructions[ip]->begin() + 5));
			ip = position - 1;
		}
		else if (opcode == OpcodeType::OpJumpNotTruthy) {
			int position = endian.byteToInt(vector<byte>(instructions[ip]->begin() + 1, instructions[ip]->begin() + 5));

			Object* condition = pop();
			if(!isTruthy(condition)){
				ip = position - 1;
			}
		}
		else if (opcode == OpcodeType::OpCall) {
			int numArgs = endian.byteToInt(vector<byte>(instructions[ip]->begin() + 1, instructions[ip]->begin() + 5));
			executeCall(numArgs);
		}
		else if (opcode == OpcodeType::OpReturnValue) {
			auto returnValue = pop();

			auto frame = popFrame(); // pop function frame
			stackPointer = frame->basePointer - 1;

			push(returnValue);
		}
		else if (opcode == OpcodeType::OpReturn) {
			auto frame = popFrame();
			stackPointer = frame->basePointer - 1;
		}
		else if (opcode == OpcodeType::OpSetLocal) {
			int localIndex = endian.byteToInt(vector<byte>(instructions[ip]->begin() + 1, instructions[ip]->begin() + 5));
			auto frame = currentFrame();
			frame->ip++;

			stack[frame->basePointer + localIndex] = pop();
		}
		else if (opcode == OpcodeType::OpGetLocal) {
			int localIndex = endian.byteToInt(vector<byte>(instructions[ip]->begin() + 1, instructions[ip]->begin() + 5));
			auto frame = currentFrame();
			frame->ip++;

			push(stack[frame->basePointer + localIndex]);
		}
		else if (opcode == OpcodeType::OpGetBuiltin) {
			int builtinIndex = endian.byteToInt(vector<byte>(instructions[ip]->begin() + 1, instructions[ip]->begin() + 5));

			Builtin* definition = &(builtins.builtinList[builtinIndex]);

			push(definition);
		}
        else if (opcode == OpcodeType::OpMakeClass) {
            int numArgs = endian.byteToInt(vector<byte>(instructions[ip]->begin() + 1, instructions[ip]->begin() + 5));
            makeClass(numArgs);
        }
    }
}


Object* VirtualMachine::stackTop() {
    if (stackPointer == 0) {
        throw invalid_argument("");
    }

    return stack[stackPointer - 1];
}

void VirtualMachine::push(Object *object) {
    if (stackPointer >= StackSize) {
        throw invalid_argument("");
    }

    stack[stackPointer] = object;
    stackPointer++;
}

Object* VirtualMachine::pop() {
    Object* object = stack[stackPointer - 1];
    stackPointer--;

    return object;
}

Object* VirtualMachine::lastPoppedElement() {
    return stack[stackPointer];
}

void VirtualMachine::executeBinaryOperation(OpcodeType opcode) {
    Object* right = pop();
    Object* left = pop();

    if (left->type == ObjectType::INTEGER && right->type == ObjectType::INTEGER) {
        return executeBinaryIntegerOperation(opcode, dynamic_cast<Integer*>(left), dynamic_cast<Integer*>(right));
    }
    else if (left->type == ObjectType::STRING && right->type == ObjectType::STRING) {
        return executeBinaryStringOperation(opcode, dynamic_cast<String*>(left), dynamic_cast<String*>(right));
    }
}

void VirtualMachine::executeBinaryIntegerOperation(OpcodeType opcode, Integer* left, Integer* right) {
    long long leftValue = left->value;
    long long rightValue = right->value;

    long long returnValue;
    switch (opcode) {
        case OpcodeType::OpAdd:
            returnValue = leftValue + rightValue;
            break;
        case OpcodeType::OpSub:
            returnValue = leftValue - rightValue;
            break;
        case OpcodeType::OpMul:
            returnValue = leftValue * rightValue;
            break;
        case OpcodeType::OpDiv:
            returnValue = leftValue / rightValue;
            break;
        defalut:
            throw invalid_argument("");
    }

    push(new Integer{returnValue});
}

void VirtualMachine::executeBinaryStringOperation(OpcodeType opcode, String *left, String *right) {
    if (opcode != OpcodeType::OpAdd) {
        throw invalid_argument("");
    }

    return push(new String{left->value + right->value});
}


void VirtualMachine::executeComparison(OpcodeType opcode) {
    Object* right = pop();
    Object* left = pop();

    if (left->type == ObjectType::INTEGER && right->type == ObjectType::INTEGER) {
        return executeIntegerComparison(opcode, dynamic_cast<Integer*>(left), dynamic_cast<Integer*>(right));
    }

    // boolean끼리 연산
    switch (opcode) {
        case OpcodeType::OpEqual:
            return push(nativeBoolToBooleanObject(left == right));
        case OpcodeType::OpNotEqual:
            return push(nativeBoolToBooleanObject(left != right));
        default:
            throw invalid_argument("");
    }
}

void VirtualMachine::executeIntegerComparison(OpcodeType opcode, Integer *left, Integer *right) {
    long long leftValue = left->value;
    long long rightValue = right->value;

    switch (opcode) {
        case OpcodeType::OpEqual:
            return push(nativeBoolToBooleanObject(leftValue == rightValue));
        case OpcodeType::OpNotEqual:
            return push(nativeBoolToBooleanObject(leftValue != rightValue));
        case OpcodeType::OpLessThan:
            return push(nativeBoolToBooleanObject(leftValue < rightValue));
        default:
            throw invalid_argument("");
    }
}

Boolean* VirtualMachine::nativeBoolToBooleanObject(bool input) {
    if (input)
        return TRUE;
    else
        return FALSE;
}

void VirtualMachine::executeBangOperator() {
    Object* operand = pop();

    if (operand == TRUE) {
        return push(FALSE);
    }
    else if (operand == FALSE) {
        return push(TRUE);
    }
    else { // ???
        return push(FALSE);
    }
}

void VirtualMachine::executeMinusOperator() {
    Object* operand = pop();

    if (operand->type == ObjectType::INTEGER) {
        long long value = -dynamic_cast<Integer*>(operand)->value;
        return push(new Integer(value));
    }
    else {
        throw invalid_argument("");
    }
}


Object* VirtualMachine::buildArray(int startIndex, int endIndex) {
    int numElements = endIndex - startIndex;
    vector<Object*> elements(numElements);

    for (int i = startIndex; i < endIndex; ++i) {
        elements[i - startIndex] = stack[i];
    }

    return new Array{elements};
}

void VirtualMachine::executeIndexExpression(Object *left, Object *index) {
    if (left->type == ObjectType::ARRAY && index->type == ObjectType::INTEGER) {
        return executeArrayIndex(dynamic_cast<Array*>(left), dynamic_cast<Integer*>(index));
    }
    else {
        throw invalid_argument("index operator not supported");
    }
}

void VirtualMachine::executeArrayIndex(Array* left, Integer* index) {
    int m = (int)left->elements.size() - 1;
    long long i = index->value;
    if (i < 0 || m < i) {
        throw invalid_argument("");
    }

    push(left->elements[i]);
}

bool VirtualMachine::isTruthy(Object *obj) {
	if(Boolean* boolean = dynamic_cast<Boolean*>(obj)){
		return boolean->value;
	}
	else{
		return true;
	}
}

Frame *VirtualMachine::currentFrame() {
	return frames[frameIndex - 1];
}

void VirtualMachine::pushFrame(Frame *frame) {
	frames[frameIndex++] = frame;
}

Frame *VirtualMachine::popFrame() {
	frameIndex--;
	return frames[frameIndex];
}

void VirtualMachine::callFunction(CompiledFunction* function,int numArgs){
	if(numArgs != function->numParameters){
		// 인수 개수 오류
		throw(("인수의 개수가 맞지 않습니다. 매개변수 : "
			   + to_string(function->numParameters) + ", 인수 : " + to_string(numArgs)));
	}

	Frame* frame = new Frame(function, stackPointer - numArgs);
	pushFrame(frame);
	stackPointer = frame->basePointer + function->numLocals;

	return;
}

void VirtualMachine::callBuiltin(Builtin *builtin, int numArgs) {
	vector<Object*> args;
	for(int i = stackPointer - numArgs; i < stackPointer; i++){
		args.push_back(stack[i]);
	}
	Object* result = builtin->fn(args);
	if(result != nullptr){
		push(result);
	}
}

void VirtualMachine::executeCall(int numArgs) {
	Object* callee = stack[stackPointer - 1 - numArgs];
	if(CompiledFunction* function = dynamic_cast<CompiledFunction*>(callee)) {
		callFunction(function, numArgs);
	}
	else if(Builtin* builtin = dynamic_cast<Builtin*>(callee)) {
		callBuiltin(builtin, numArgs);
	}
	else{
		throw(invalid_argument("함수가 아닌 것을 호출하고 있습니다."));
	}
}

void VirtualMachine::makeClass(int numArgs) {
    Object* callee = stack[stackPointer - 1 - numArgs];
    if (CompiledClass* classObj = dynamic_cast<CompiledClass*>(callee)) {

    } else {
        throw(invalid_argument("클래스가 아닌 것을 호출하고 있습니다."));
    }
}








// Frame

vector<Instruction *> Frame::Instructions() {
	return function->instructions;
}

Frame::Frame(CompiledFunction *fn, int basePointer) {
	this->function = fn;
	this->basePointer = basePointer;
	this->ip = -1;
}
