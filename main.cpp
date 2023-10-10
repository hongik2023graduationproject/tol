#include <iostream>
#include "repl/repl.h"
#include "compiler/compiler.h"
#include "code/codeTest.h"



int main() {
    Compiler compiler;
    cout << "Endian Type: " << compiler.endian.checkBigEndianComputer() << endl;

    CodeTest codeTest;
    codeTest.codeTest();

    Repl repl;
//    repl.lexerTest();
//    repl.parserTest();
//    repl.run();
    return 0;
}

