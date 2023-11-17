#include "builtins.h"

Object *Builtins::funcLen(const vector<Object *> args) {
	auto arg = args[0];
	if(Array* arr = dynamic_cast<Array*>(arg)){
		Integer * len = new Integer;
		len->value = arr->elements.size();
		return len;
	}
	else if(String* str = dynamic_cast<String*>(arg)){
		Integer * len = new Integer;
		len->value = str->value.length();
		return len;
	}
	return nullptr; // 오류 처리할 것
}



Builtin *Builtins::getBuiltinByName(string name) {
	for(auto it = builtinList.begin(); it != builtinList.end(); it++){
		return &(*it);
	}
	return nullptr;
}

Object *Builtins::funcPuts(vector<Object *> args) {
	for(auto obj : args){
		if(Integer* integer = dynamic_cast<Integer*>(obj)){
			cout << integer->value;
		}
		else if(String* str = dynamic_cast<String*>(obj)){
			cout << str->value;
		}
		else{
			throw(invalid_argument("출력한다. : 올뱌르지 않은 인수입니다."));
		}
	}

	return nullptr;
}

Object *Builtins::funcGets(vector<Object *> args) {
	string input;

	std::getline(std::cin, input);

	std::istringstream iss(input);

	// 정수 또는 실수로 변환을 시도
	int integerResult;
	float floatResult;

	// 정수로 변환 시도
	iss >> integerResult;

	// 변환 성공했으면 정수 객체 반환
	if (!iss.fail()) {
		Integer* integer = new Integer;
		integer->value = integerResult;
		return integer;
	}

	// 실수로 변환 시도
//	iss >> floatResult;
//	if(!iss.fail()){
//
//	}

	// string 객체 반환
	auto str = new String;
	str->value = input;
	return str;
}

Object *Builtins::funcNewl(vector<Object *> args) {
	cout << '\n';
	return nullptr;
}
