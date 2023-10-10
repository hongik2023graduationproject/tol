#include <iostream>
#include "repl/repl.h"
#include "endian/endian.h"




int main() {
    Endian endian;
    cout << endian.checkBigEndianComputer() << endl;

    Repl repl;
//    repl.lexerTest();
//    repl.parserTest();
//    repl.run();
    return 0;
}

