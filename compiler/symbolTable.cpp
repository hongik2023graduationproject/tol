#include "symbolTable.h"


Symbol SymbolTable::Define(const std::string& name, const ObjectType type) {
    Symbol symbol;
    symbol.name = name;
    symbol.index = numberDefinitions;
    symbol.scope = outer == nullptr ? GlobalScope : LocalScope;
    symbol.type = type;

    store[name] = symbol;
    numberDefinitions++;
    return symbol;
}

Symbol SymbolTable::Resolve(const std::string& name) {
    if (store.find(name) == store.end()) {
		if (outer != nullptr){
			return outer->Resolve(name);
		}
		else throw invalid_argument("");
    }

    return store[name];
}

SymbolTable *SymbolTable::NewEnclosedSymbolTable(SymbolTable* outer) {
	SymbolTable* symbolTable = new SymbolTable;
	symbolTable->outer = outer;
	return symbolTable;
}

Symbol SymbolTable::DefineBuiltin(int index, const string &name) {
	Symbol symbol{.name = name, .index = index, .scope = BuiltinScope};
	store[name] = symbol;
	return symbol;
}

