#include "utf8Converter.h"

vector<string> Utf8Converter::convert(const string &input) {
    vector<string> characters;
    this->input = input;

    for (int position = 0, length; position < this->input.size(); position += length) {
        length = getCharacterLength(position);
        characters.push_back(this->input.substr(position, length));
    }

    return characters;
}

int Utf8Converter::getCharacterLength(int position) {
    int length = 0;
    if (input[position] & 0x80) {
        length++;
        if (input[position] & 0x40) {
            length++;
            if (input[position] & 0x20) {
                length++;
                if (input[position] & 0x10) {
                    length++;
                }
            }
        }
    }
    return (length == 0) ? 1 : length;
}