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
private:
    bool isBigEndianComputer;
};

bool Endian::checkBigEndianComputer() {
    uint32_t value = 1;
    uint8_t* bytes = reinterpret_cast<uint8_t*>(&value);

    (bytes[0] == 1) ? isBigEndianComputer = false : isBigEndianComputer = true;
    return isBigEndianComputer;
}

vector<byte> Endian::intToByte(int value) {
    vector<byte> byteArray(4);
    std::memcpy(byteArray.data(), &value, sizeof(int));

    if (!isBigEndianComputer)
        reverse(byteArray.begin(), byteArray.end());

//    // 바이트 배열 출력 테스트용
//    for (int i = 0; i < sizeof(int); i++) {
//        std::cout << std::hex << static_cast<int>(byteArray[i]) << " ";
//    }
//    std::cout << std::endl;

    return byteArray;
}

#endif //TOLELOM_ENDIAN_H
