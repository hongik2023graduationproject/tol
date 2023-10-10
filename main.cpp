#include <iostream>
#include "repl/repl.h"
#include "endian/endian.h"
#include "code/code.h"



int main() {
    Endian endian;
    cout << "Endian Type: " << endian.checkBigEndianComputer() << endl;

    byte t = static_cast<byte>(5);

    Instruction* testInstruction = makeInstruction(OpcodeType::OpConstant, vector<int>{65534});
    for (auto& it : *testInstruction) {
        cout << static_cast<int>(it) << ' ';
    }
    cout << endl;

    Repl repl;
//    repl.lexerTest();
//    repl.parserTest();
//    repl.run();
    return 0;
}

