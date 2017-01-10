#include "jsonchild.h"

JsonChild::JsonChild(const bool &value)
{
    initialize();
    type = BOOL;
    boolValue = value;
}

JsonChild::JsonChild(const double &value)
{
    initialize();
    type = DOUBLE;
    doubleValue = value;
}

JsonChild::JsonChild(const int &value)
{
    initialize();
    type = INT;
    intValue = value;
}

JsonChild::JsonChild(JsonObject *value)
{
    initialize();
    type = JSON_OBJECT;
    jsonObject = value;
}

JsonChild::JsonChild(JsonArray *value)
{
    initialize();
    type = JSON_ARRAY;
    jsonArray = value;
}

JsonChild::JsonChild(const std::string &value)
{
    initialize();
    type = STRING;
    stringValue = value;
}

bool JsonChild::isBool(){
    return type == BOOL;
}

bool JsonChild::isDouble(){
    return type == DOUBLE;
}

bool JsonChild::isInt(){
    return type == INT;
}

bool JsonChild::isJsonArray(){
    return type == JSON_ARRAY;
}

bool JsonChild::isJsonObject(){
    return type == JSON_OBJECT;
}

bool JsonChild::isString(){
    return type == STRING;
}

bool JsonChild::getBool(){
    return boolValue;
}

int JsonChild::getInt(){
    return intValue;
}

double JsonChild::getDouble(){
    return doubleValue;
}

void JsonChild::setType(const JsonType &type){
    this->type = type;
}

JsonType JsonChild::getType(){
    return type;
}

std::string JsonChild::getString(){
    return stringValue;
}

JsonObject* JsonChild::getJsonObject(){
    return jsonObject;
}

JsonArray* JsonChild::getJsonArray(){
    return jsonArray;
}

std::string JsonChild::toString(){
    std::string value = "";
    if (type == BOOL){
        if (boolValue) return "true";
        return "false";
    }

    if (type == INT)
        return intToString(intValue);

    if (type == DOUBLE)
        return doubleToString(doubleValue);

    if (type == JSON_OBJECT && jsonObject != NULL)
        return jsonObject->toString();

    if (type == JSON_ARRAY && jsonArray != NULL)
        return jsonArray->toString();

    if (type == STRING)
        return "\"" + escapeSpecialCharacters(stringValue) + "\"";

    return value;
}

std::string JsonChild::intToString(const int &value){
    std::ostringstream stream;
    stream << value;
    return stream.str();
}

std::string JsonChild::doubleToString(const double &value){
    std::ostringstream stream;
    stream << value;
    return stream.str();
}

std::string JsonChild::escapeSpecialCharacters(std::string text){
    if (text.empty()) return text;

    //TODO for hexadecimal and unicode character
    std::string specialCharacters = "\a\b\e\f\n\r\t\v\"\\";
    std::string replacement = "abefnrtv\"\\";
    int sizeReplacement = replacement.size();
    for (unsigned int i = 0; i < text.size(); i++){
        for (int j = 0; j < sizeReplacement; j++){
            if (text[i] == specialCharacters[j]){
                text[i] = replacement[j];
                text.insert(i, "\\");
                i++;
                break;
            }
        }
    }

    return text;
}

void JsonChild::initialize(){
    jsonObject = NULL;
    jsonArray = NULL;
    type = UNKNOWN;
    boolValue = false;
    doubleValue = 0;
    int intValue = 0;
    stringValue = "";
}

JsonChild::~JsonChild(){
    if (jsonObject != NULL){
        delete jsonObject;
        jsonObject = NULL;
    }

    if (jsonObject != NULL){
        delete jsonArray;
        jsonArray = NULL;
    }
}
