#ifndef TOLELOM_ENDIAN_H
#define TOLELOM_ENDIAN_H

#include <vector>
#include <cstdint>
#include <cstring>
#include <algorithm>

using namespace std;

class Endian {
public:
    bool checkBigEndianComputer();
    vector<byte> intToByte(int value);
    int byteToInt(vector<byte> bytes);
private:
    bool isBigEndianComputer;
};


#endif //TOLELOM_ENDIAN_H
