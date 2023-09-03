#ifndef TOLELOM_UTF8CONVERTER_H
#define TOLELOM_UTF8CONVERTER_H


#include <string>
#include <vector>

using namespace std;

class Utf8Converter {
public:
    vector<string> convert(const string &input);

private:
    string input;
    int getCharacterLength(int position);
};



#endif //TOLELOM_UTF8CONVERTER_H
