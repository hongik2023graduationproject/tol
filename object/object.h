#ifndef TOLELOM_OBJECT_H
#define TOLELOM_OBJECT_H

#include <utility>

#include "objectType.h"

class Object {
public:
    ObjectType type = ObjectType::RETURN_VALUE_OBJECT;

    virtual string print() = 0;
};

class Integer : public Object{
public:
    ObjectType type = ObjectType::INTEGER;
    long long value{};

    Integer() = default;
    Integer(long long value) : value(value) {};

    string print() final {
        return to_string(value);
    }
};

class String : public Object {
public:
    ObjectType type = ObjectType::STRING;
    string value{};

    String() = default;
    String(string s) : value(std::move(s)) {};

    string print() final {
        return value;
    }
};

class Boolean : public Object {
public:
    ObjectType type = ObjectType::BOOLEAN;
    bool value{};

    Boolean() = default;
    explicit Boolean(bool value) : value(value) {};

    string print() final {
        return to_string(value);
    }
};

class ReturnValue : public Object {
public:
    ObjectType type = ObjectType::RETURN_VALUE_OBJECT;
    Object* value{};

    string print() final {
        return value->print();
    }
};

#endif //TOLELOM_OBJECT_H
