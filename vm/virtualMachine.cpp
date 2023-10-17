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
    instructions = bytecode.instructions;
    constants = bytecode.constants;
	boolTrue = new Boolean;
	boolFalse = new Boolean;
	boolTrue->value = true;
	boolFalse->value = false;
    stack.resize(StackSize);
    stackPointer = 0;

    for (int ip = 0; ip < int(instructions.size()); ++ip) { // ip = instruction pointer
        OpcodeType opcode = static_cast<OpcodeType>(int((*instructions[ip])[0]));

        if (opcode == OpcodeType::OpConstant) {
            int constIndex = endian.byteToInt(vector<byte>(instructions[ip]->begin() + 1, instructions[ip]->begin() + 5));
            push(constants[constIndex]);
        }
        else if (opcode == OpcodeType::OpAdd || opcode == OpcodeType::OpSub
		|| opcode == OpcodeType::OpMul || opcode == OpcodeType::OpDiv) {
			executeBinaryOperation(opcode);
//            Object* right = pop();
//            Object* left = pop();
//
//            Integer* rightInteger = dynamic_cast<Integer*>(right);
//            Integer* leftInteger = dynamic_cast<Integer*>(left);
//
//            Integer* resultInteger = new Integer;
//            resultInteger->value = leftInteger->value + rightInteger->value;
//            push(resultInteger);
        }
		else if (opcode == OpcodeType::OpPop) {
			pop();
		}
		else if (opcode == OpcodeType::OpTrue) {
			push(boolTrue);
		}
		else if (opcode == OpcodeType::OpFalse) {
			push(boolFalse);
		}
		else if (opcode == OpcodeType::OpEqual || opcode == OpcodeType::OpNotEqual
		|| opcode == OpcodeType::OpGreaterThan){
			executeComparison(opcode);
		}
    }
}


Object* VirtualMachine::stackTop() {
    if (stackPointer == 0) {
        throw invalid_argument("");
    }

    return stack[stackPointer - 1];
}

//Object* VirtualMachine::lastPoppedStackElement(){
//	return stack[stackPointer];
//}

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

void VirtualMachine::executeBinaryOperation(OpcodeType op) {
	Object* right = pop();
	Object* left = pop();

	ObjectType rightType = right->type;
	ObjectType leftType = left->type;

	if(leftType == ObjectType::INTEGER && rightType == ObjectType::INTEGER){
		executeBinaryIntegerOperation(op, left, right);
	}
}

void VirtualMachine::executeBinaryIntegerOperation(OpcodeType op, Object *left, Object *right) {
	Integer* rightInteger = dynamic_cast<Integer*>(right);
	Integer* leftInteger = dynamic_cast<Integer*>(left);

	Integer* resultInteger = new Integer;
	if(op == OpcodeType::OpAdd){
		resultInteger->value = leftInteger->value + rightInteger->value;
	}
	else if(op == OpcodeType::OpSub){
		resultInteger->value = leftInteger->value - rightInteger->value;
	}
	else if(op == OpcodeType::OpMul){
		resultInteger->value = leftInteger->value * rightInteger->value;
	}
	else if(op == OpcodeType::OpDiv){
		resultInteger->value = leftInteger->value / rightInteger->value;
	}
	else{
		// throw( ); <- 어떤 에러 넣어야 하나?
	}
	push(resultInteger);
}

void VirtualMachine::executeComparison(OpcodeType opcode) {
	Object* right = pop();
	Object* left = pop();

	if(left->type == ObjectType::INTEGER && right->type == ObjectType::INTEGER) {
		executeIntegerComparison(opcode, left, right);
	}
	else{
		if (opcode == OpcodeType::OpEqual){
			push(nativeBoolToBooleanObject(right == left));
		}
		else if (opcode == OpcodeType::OpNotEqual){
			push(nativeBoolToBooleanObject(right != left));
		}
		else{
			// throw();
		}
	}
}

void VirtualMachine::executeIntegerComparison(OpcodeType opcode, Object *left, Object *right) {
	int leftValue = dynamic_cast<Integer*>(left)->value;
	int rightValue = dynamic_cast<Integer*>(right)->value;

	if (opcode == OpcodeType::OpEqual){
		push(nativeBoolToBooleanObject(rightValue == leftValue));
	}
	else if (opcode == OpcodeType::OpNotEqual){
		push(nativeBoolToBooleanObject(rightValue != leftValue));
	}
	else if (opcode == OpcodeType::OpGreaterThan){
		push(nativeBoolToBooleanObject(rightValue > leftValue));
	}
	else {
		// throw();
	}
}

Boolean *VirtualMachine::nativeBoolToBooleanObject(bool input) {
	if(input){
		return boolTrue;
	}
	return boolFalse;
}

