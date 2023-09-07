#ifndef TOLELOM_ENVIRONMENT_H
#define TOLELOM_ENVIRONMENT_H

#include <map>
#include <string>
#include "object.h"

using namespace std;

class Environment {
public:
    map<string, Object*> store;
    Object* get(string name) {
        return store[name];
    }
    Object* set(string name , Object* object) {
        store[name] = object;
        return object;
    }
};


#endif //TOLELOM_ENVIRONMENT_H
