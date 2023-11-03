#ifndef TOLELOM_BUILTINS_H
#define TOLELOM_BUILTINS_H

#include <string>
#include <iostream>
#include <map>
#include "../object/object.h"
using namespace std;

class Builtins{
public:
	Builtin* getBuiltinByName(string name);

	static Object* funcLen(vector<Object*> args);
	static Object* funcPuts(vector<Object*> args);


	vector<Builtin> builtinList = {
			Builtin("len", funcLen),
			Builtin("출력한다", funcPuts),

	};
};

#endif //TOLELOM_BUILTINS_H
