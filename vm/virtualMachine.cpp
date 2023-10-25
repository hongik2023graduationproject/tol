#include "virtualMachine.h"

void VirtualMachine::testIntegerObject(long long expected, Object* object) {
    if (Integer* integer = dynamic_cast<Integer*>(object)) {
        if (integer->value != expected) {
            throw invalid_argument("");
        }
        // ok

    }
    else {
        throw invalid_argument("");
    }
}

void VirtualMachine::run(Bytecode bytecode) {
    // init

	CompiledFunction* mainFunction = new CompiledFunction;// (instructions); // 왜 생성자가 안먹히지?
	mainFunction->instructions = bytecode.instructions;
	Frame* mainFrame = new Frame{.function = mainFunction};
	constants = bytecode.constants;
    stack.resize(StackSize);
    globals.resize(GlobalsSize);
	frames.resize(MaxFrames);
    stackPointer = 0;
	frameIndex = 0;
	pushFrame(mainFrame);


    while (currentFrame()->ip < (currentFrame()->Instructions().size() -1)) { // ip = instruction pointer
        int& ip = currentFrame()->ip;
		vector<Instruction *> instructions = currentFrame()->Instructions();
		ip++;
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
			if (CompiledFunction* function = dynamic_cast<CompiledFunction*>(stackTop())){
				Frame* frame = new Frame{.function = function};
				pushFrame(frame);
			}
			else {
				// throw(, "Calling non-function");
				return;
			}
		}
		else if (opcode == OpcodeType::OpReturnValue) {
			auto returnValue = pop();
			popFrame(); // pop function frame
			pop(); // pop CompiledFunction

			push(returnValue);
		}
		else if (opcode == OpcodeType::OpReturn) {
			popFrame();
			pop();
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
    vector<Object*> elements(endIndex - startIndex);

    for (int i = startIndex; i < endIndex; ++i) {
        elements[i - startIndex] = stack[i];
    }

    return new Array{elements};
}

void VirtualMachine::executeIndexExpression(Object *left, Object *index) {
    if (left->type == ObjectType::ARRAY && index->type == ObjectType::INTEGER) {
        return executeArrayIndex(static_cast<Array*>(left), static_cast<Integer*>(index));
    }
    else {
        throw invalid_argument("");
    }
}

void VirtualMachine::executeArrayIndex(Array* left, Integer* index) {
    int m = left->elements.size() - 1;
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

vector<Instruction *> Frame::Instructions() {
	return function->instructions;
}
