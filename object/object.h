#ifndef TOLELOM_OBJECT_H
#define TOLELOM_OBJECT_H

#include <utility>

#include "objectType.h"

class Object {
public:
    ObjectType type = {};

    virtual string print() = 0;
};

class Integer : public Object{
public:
    long long value{};

    Integer() = default;
    Integer(long long value) : value(value) { type = ObjectType::INTEGER; };

    string print() final {
        return to_string(value);
    }
};

class String : public Object {
public:
    string value{};

    String() = default;
    String(string s) : value(std::move(s)) { type = ObjectType::INTEGER; };

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

#endif //TOLELOM_OBJECT_H
