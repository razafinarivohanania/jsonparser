#ifndef JSONCHILD_H
#define JSONCHILD_H

#include <sstream>
#include <iostream>

#include "jsontype.h"
#include "jsonarray.h"
#include "jsonobject.h"

typedef unsigned short u_short;

class JsonObject;
class JsonArray;

class JsonChild
{
public:
    JsonChild(const bool &value);
    JsonChild(const double &value);
    JsonChild(const int &value);
    JsonChild(const std::string &value);
    JsonChild(JsonArray *value);
    JsonChild(JsonObject *value);

    ~JsonChild();

    bool getBool();
    bool isBool();
    bool isDouble();
    bool isInt();
    bool isJsonArray();
    bool isJsonObject();
    bool isString();

    double getDouble();

    int getInt();

    JsonArray* getJsonArray();
    JsonObject* getJsonObject();
    JsonType getType();

    std::string getString();
    std::string toString();

    void setType(const JsonType &type);

    static std::string intToString(const int &value);
private:
    bool boolValue;
    double doubleValue;
    int intValue;

    JsonArray *jsonArray;
    JsonObject *jsonObject;
    JsonType type;

    std::string stringValue;
    std::string escapeSpecialCharacters(std::string text);
    std::string doubleToString(const double &value);

    void initialize();

    static const char DOUBLE_QUOT = '"';
    static const char BACK_SLASH = '\\';
};

#endif // JSONCHILD_H
