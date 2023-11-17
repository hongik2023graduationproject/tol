#ifndef TOLELOM_BUILTINS_H
#define TOLELOM_BUILTINS_H

#include <string>
#include <iostream>
#include <map>
#include <sstream>
#include "../object/object.h"
using namespace std;

class Builtins{
public:
	Builtin* getBuiltinByName(string name);

	static Object* funcLen(vector<Object*> args);
	static Object* funcPuts(vector<Object*> args);
	static Object* funcGets(vector<Object*> args);
	static Object* funcNewl(vector<Object*> args);


	vector<Builtin> builtinList = {
			Builtin("길이", funcLen), // len함수 이름 추천좀..
			Builtin("출력한다", funcPuts),
			Builtin("입력한다", funcGets),
			Builtin("개행한다", funcNewl),


	};
};

#endif //TOLELOM_BUILTINS_H
