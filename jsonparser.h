#ifndef JSONPARSER_H
#define JSONPARSER_H

#include <string>
#include <vector>
#include <iostream>

#include "jsonchild.h"
#include "jsonobject.h"
#include "jsonarray.h"
#include "jsontype.h"

class JsonParser
{
public:
    JsonParser(const std::string &rawJson);
    ~JsonParser();

    bool hasError();

    JsonChild getJson();

    std::string getError();

private:
    int digitsSize;
    int rawJsonSize;

    JsonChild* jsonChild;

    std::string digits;
    std::string error;
    std::string rawJson;

    std::vector<JsonChild*> jsonChilds;

    bool isDigit(const char &character);
    bool isEscapedQuot(const std::string &rawJson, const int &cursor);

    int getIndexQuot(const std::string &rawJson, const int &cursor);

    std::string getPosition(const int &cursor);

    void gotoFirstJsonCharacter(int &cursor);
    void skipSpaces(int &cursor);

    JsonChild* getValue(int &cursor);
    JsonChild* parseToJsonArray(int &cursor);
    JsonChild* parseToJsonObject(int &cursor);
    JsonChild* parseToNumberString(int &cursor);

    std::string getKey(int &cursor);
    std::string parseToString(int &cursor, const std::string &context);

    void parse();
    void removeJsonChilds();
    void saveValue(JsonObject *jsonObject, const std::string &key, JsonChild *jsonChild);

    static const char BACK_SLASH = '\\';
    static const char DOT = '.';
    static const char DOUBLE_QUOT = '"';     
};

#endif // JSONPARSER_H
