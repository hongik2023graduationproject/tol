#include "symbolTable.h"


Symbol SymbolTable::Define(std::string name) {
    Symbol symbol;
    symbol.name = name;
    symbol.index = numberDefinitions;
    symbol.scope = GlobalScope;

    store[name] = symbol;
    numberDefinitions++;
    return symbol;
}

Symbol SymbolTable::Resolve(std::string name) {
    if (store.find(name) == store.end()) {
        throw invalid_argument("");
    }

    return store[name];
}