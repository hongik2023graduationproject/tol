#ifndef TOLELOM_SYMBOLTABLE_H
#define TOLELOM_SYMBOLTABLE_H

#include <map>
#include <string>
using namespace std;

using SymbolScope = string;

//class Scopes {
    const SymbolScope GlobalScope = "Global";
//};

class Symbol {
public:
    string name;
    int index;
    SymbolScope scope;
};

class SymbolTable {
public:
    Symbol Define(string name);
    Symbol Resolve(string name);

    map<string, Symbol> store;
    int numberDefinitions;
private:
};


#endif //TOLELOM_SYMBOLTABLE_H
