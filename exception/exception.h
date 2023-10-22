#ifndef TOLELOM_EXCEPTION_H
#define TOLELOM_EXCEPTION_H

#include <iostream>
#include <exception>
#include <string>
#include <format>

using namespace std;


class LexerException : exception {
public:
    virtual void print() const = 0;
};


class notFoundEndOfString : public LexerException {
public:
    notFoundEndOfString(long long line) : line(line) {};

    void print() const {
        cout << line << "번 줄: 입력된 문자열의 끝을 알 수 없습니다." << endl;
    }
private:
    long long line;
};



#endif //TOLELOM_EXCEPTION_H
