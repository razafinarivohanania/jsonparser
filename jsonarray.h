#ifndef JSONARRAY_H
#define JSONARRAY_H

#include <string>
#include <vector>
#include <iostream>

#include "jsonobject.h"
#include "jsonchild.h"

typedef unsigned int u_int;

class JsonObject;
class JsonChild;

class JsonArray
{
public:
    JsonArray();
    ~JsonArray();

    bool has(const u_int &index);

    JsonChild* getJsonChild(const u_int &index);

    void add(const bool &value);
    void add(const double &value);
    void add(const int &value);
    void add(const std::string &value);
    void add(JsonArray *value);
    void add(JsonChild *value);
    void add(JsonObject *value);
    void remove(const u_int &index);

    std::string toString(const bool &isToFormat = false);

private:
    std::vector<JsonChild*> jsonChilds;
};

#endif // JSONARRAY_H
