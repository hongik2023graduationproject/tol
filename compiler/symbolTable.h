#ifndef TOLELOM_SYMBOLTABLE_H
#define TOLELOM_SYMBOLTABLE_H

#include <map>
#include <string>
using namespace std;

using SymbolScope = string;

//class Scopes {
    const SymbolScope GlobalScope = "Global";
	const SymbolScope LocalScope = "Local";
//};

class Symbol {
public:
    string name;
    int index;
    SymbolScope scope;
};

class SymbolTable {
public:
	SymbolTable* outer{nullptr};

    Symbol Define(const string& name);
    Symbol Resolve(const string& name);
	static SymbolTable* NewEnclosedSymbolTable(SymbolTable* outer);

    map<string, Symbol> store;
    int numberDefinitions;
private:
};


#endif //TOLELOM_SYMBOLTABLE_H
