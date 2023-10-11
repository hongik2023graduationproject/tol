#ifndef TOLELOM_VIRTUALMACHINE_H
#define TOLELOM_VIRTUALMACHINE_H

#include "../lexer/lexer.h"
#include "../parser/parser.h"
#include "../object/object.h"

class VirtualMachine {
public:
    void testIntegerObject(long long expected, Object* object);

private:
    Lexer lexer;
    Parser parser;
};


#endif //TOLELOM_VIRTUALMACHINE_H
