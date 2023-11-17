#ifndef TOLELOM_SYMBOLTABLE_H
#define TOLELOM_SYMBOLTABLE_H

#include <map>
#include <string>
#include "../object/objectType.h"
using namespace std;

using SymbolScope = string;

//class Scopes {
    const SymbolScope GlobalScope = "Global";
	const SymbolScope LocalScope = "Local";
	const SymbolScope BuiltinScope = "Builtin";
//};

class Symbol {
public:
    string name;
    int index;
    SymbolScope scope;
    ObjectType type;
};

class SymbolTable {
public:
	SymbolTable* outer{nullptr};

    Symbol Define(const string& name, const ObjectType type);
    Symbol Resolve(const string& name);
	Symbol DefineBuiltin(int index, const string& name);
	static SymbolTable* NewEnclosedSymbolTable(SymbolTable* outer);

    map<string, Symbol> store;
    int numberDefinitions;
private:
};


#endif //TOLELOM_SYMBOLTABLE_H
