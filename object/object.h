#ifndef TOLELOM_OBJECT_H
#define TOLELOM_OBJECT_H

#include <utility>

#include "objectType.h"


class Object;

using BuiltinFunction = Object* (*)(vector<Object*> args);

class Object {
public:
    ObjectType type = {};

    virtual string print() = 0;
};

class Integer : public Object{
public:
    long long value{};

    Integer() { type = ObjectType::INTEGER; };
    Integer(long long value) : value(value) { type = ObjectType::INTEGER; };

    string print() final {
        return to_string(value);
    }
};

class String : public Object {
public:
    string value{};

    String() { type = ObjectType::STRING; };
    String(string s) : value(std::move(s)) { type = ObjectType::STRING; };

    string print() final {
        return value;
    }
};

class Boolean : public Object {
public:
    bool value{};

    Boolean() = default;
    explicit Boolean(bool value) : value(value) { type = ObjectType::BOOLEAN; };

    string print() final {
        return to_string(value);
    }
};

class ReturnValue : public Object {
public:
    Object* value{};

    string print() final {
        return value->print();
    }
};



class Builtin : public Object {
public:
    BuiltinFunction fn;

    string print() {
        return "builtin function";
    }
};

#endif //TOLELOM_OBJECT_H
