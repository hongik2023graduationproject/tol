#include "codeTest.h"

void CodeTest::codeTest() {
    Code code;

    Instruction *testInstruction = code.makeInstruction(OpcodeType::OpConstant, vector<int>{65534});
    for (auto &it: *testInstruction) {
        cout << static_cast<int>(it) << ' ';
    }
    cout << endl;
}