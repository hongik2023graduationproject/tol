#include "repl/repl.h"

int main() {
    Repl repl;
//    repl.lexerTest();
//    repl.parserTest();
//    repl.runWithEvaluator();
    repl.runWithVM();

    return 0;
}

