#ifndef JSONOBJECT_H
#define JSONOBJECT_H

#include <string>
#include <map>

#include "jsonarray.h"
#include "jsonchild.h"

class JsonArray;
class JsonChild;

class JsonObject
{
public:
    JsonObject();
    ~JsonObject();

    bool has(const std::string &key);

    JsonChild* getJsonChild(const std::string &key);

    std::string toString(const bool &isToFormat = false);

    void put(const std::string &key, const bool &value);
    void put(const std::string &key, const std::string &value);
    void put(const std::string &key, const int &value);
    void put(const std::string &key, const double &value);
    void put(const std::string &key, JsonArray *value);
    void put(const std::string &key, JsonObject *value);
    void put(const std::string &key, JsonChild *value);
    void remove(const std::string key);

private:
    std::map<std::string, JsonChild*> jsonChilds;
};

#endif // JSONOBJECT_H
