#ifndef TOLELOM_OBJECT_H
#define TOLELOM_OBJECT_H

enum class ObjectType {
    INTEGER,
    BOOLEAN,
};

class Object {
public:
    virtual string print() = 0;
};

class Integer : public Object{
public:
    long long value;
    string print() final {
        return to_string(value);
    }
};

class Boolean : public Object {
public:
    bool value;
};

#endif //TOLELOM_OBJECT_H
