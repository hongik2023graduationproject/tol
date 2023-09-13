#include <iostream>
#include "repl/repl.h"

int main() {
    Repl repl;
//    repl.lexerTest();
//    repl.parserTest();
    repl.run();
    return 0;
}
