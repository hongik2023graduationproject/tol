#ifndef TOLELOM_OBJECT_H
#define TOLELOM_OBJECT_H

enum class ObjectType {
    INTEGER,
    BOOLEAN,
};

class Object {
public:
    ObjectType type;
    virtual string print() = 0;
};

class Integer : public Object{
public:
    ObjectType type = ObjectType::INTEGER;
    long long value;
    string print() final {
        return to_string(value);
    }
};

class Boolean : public Object {
public:
    ObjectType type = ObjectType::BOOLEAN;
    bool value;
    Boolean() {};
    Boolean(bool value) : value(value) {};
    string print() final {
        return to_string(value);
    }
};

#endif //TOLELOM_OBJECT_H
