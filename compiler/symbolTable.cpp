#include "symbolTable.h"


Symbol SymbolTable::Define(std::string name) {
    Symbol symbol;
    symbol.name = name;
    symbol.index = numberDefinitions;
    symbol.scope = outer == nullptr ? GlobalScope : LocalScope;

    store[name] = symbol;
    numberDefinitions++;
    return symbol;
}

Symbol SymbolTable::Resolve(std::string name) {
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

