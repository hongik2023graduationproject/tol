#include "builtins.h"

Object *Builtins::funcLen(const vector<Object *> args) {
	return nullptr;
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
