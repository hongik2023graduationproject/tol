#ifndef TOLELOM_OBJECT_H
#define TOLELOM_OBJECT_H

#include <vector>
#include <utility>

#include "objectType.h"
using namespace std;

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
	string name;
    BuiltinFunction fn;

	Builtin() {type = ObjectType::BUILTIN_FUNCTION;};
	Builtin(string name, BuiltinFunction fn) {
		this->name = name;
		this->fn = fn;
		type = ObjectType::BUILTIN_FUNCTION;
	};

    string print() {
        return "builtin function";
    }
};


class Array : public Object {
public:
    vector<Object*> elements;

    Array() = default;
    Array(vector<Object*> elements) : elements(elements) {};

    string print() {
        string s = "[";
        for (auto element : elements) {
            s += element->print() + ",";
        }
        s += "]";

        return s;
    }
};


class CompiledFunction : public Object {
public:
	vector<vector<byte>*> instructions;
	int numLocals; // count of local variables
	int numParameters; // count of parameters

	CompiledFunction() {type = ObjectType::COMPILED_FUNCTION;};
	CompiledFunction(vector<vector<byte>*> instructions) : instructions(std::move(instructions))
		{type = ObjectType::COMPILED_FUNCTION;};

	string print() {
		return "CompiledFunction";
	}
};

class CompiledClass : public Object {
public:
    vector<vector<byte>*> instructions;
    int numLocalDefine;
    string name;

    string print() {
        return "Compiled Class: " + name;
    }
};

#endif //TOLELOM_OBJECT_H
