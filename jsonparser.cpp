#include "jsonparser.h"

JsonParser::JsonParser(const std::string &rawJson)
{
    this->rawJson = rawJson;
    error = "";
    digits = "0123456789";
    digitsSize = digits.size();
    rawJsonSize = rawJson.size();
    parse();
    if (hasError())
        removeJsonChilds();
}

void JsonParser::parse(){
    int cursor = 0;

    gotoFirstJsonCharacter(cursor);

    if (hasError()) return;

    if (rawJson[cursor] == '{')
        jsonChild = parseToJsonObject(cursor);
    else
        jsonChild = parseToJsonArray(cursor);
}

JsonChild JsonParser::getJson(){
    return *jsonChild;
}

JsonChild* JsonParser::parseToJsonArray(int &cursor){
    cursor++;
    if (cursor >= rawJsonSize){
        error = "No json array found" + getPosition(cursor - 1);
        return NULL;
    }

    JsonArray *jsonArray = new JsonArray;

    while(true){
        JsonChild *value = getValue(cursor);
        if (hasError()) return NULL;

        skipSpaces(cursor);

        if (cursor >= rawJsonSize){
            error = "Out of range cursor on json array treatment" + getPosition(cursor);
            return NULL;
        }

        if (rawJson[cursor] == ',' || rawJson[cursor] == ']'){
            jsonArray->add(value);
            cursor++;

            if (rawJson[cursor - 1] == ',') continue;

            JsonChild *jsonChild = new JsonChild(jsonArray);
            jsonChilds.push_back(jsonChild);
            return jsonChild;
        }

        break;
    }

    std::string c = "" + rawJson[cursor];
    error = "Illegal character \"" + c + "\"" + getPosition(cursor);
    return NULL;
}

JsonChild* JsonParser::parseToJsonObject(int &cursor){
    cursor++;
    if (cursor >= rawJsonSize){
        error = "No json object found" + getPosition(cursor - 1);
        return NULL;
    }

    JsonObject *jsonObject = new JsonObject;

    while(true){
        std::string key = getKey(cursor);
        if (hasError()) return NULL;

        skipSpaces(cursor);
        if (rawJson[cursor] != ':'){
            error = "No separator \":\" found" + getPosition(cursor);
            return NULL;
        }

        cursor++;

        JsonChild *value = getValue(cursor);
        if (hasError()) return NULL;

        skipSpaces(cursor);

        if (cursor >= rawJsonSize){
            error = "Out of range cursor on json object treatment" + getPosition(cursor);
            return NULL;
        }

        if (rawJson[cursor] == ',' || rawJson[cursor] == '}'){
            jsonObject->put(key, value);
            cursor++;

            if (rawJson[cursor - 1] == ',') continue;

            JsonChild *jsonChild = new JsonChild(jsonObject);
            jsonChilds.push_back(jsonChild);
            return jsonChild;
        }

        break;
    }

    std::string c = "" + rawJson[cursor];
    error = "Illegal character \"" + c + "\"" + getPosition(cursor);
    return NULL;
}

void JsonParser::saveValue(JsonObject *jsonObject, const std::string &key, JsonChild *jsonChild){
    if (jsonObject == NULL || jsonChild == NULL || key.empty()) return;

    JsonType type = jsonChild->getType();
    if (type == BOOL){
        jsonObject->put(key, jsonChild->getBool());
        return;
    }

    if (type == INT){
        jsonObject->put(key, jsonChild->getInt());
        return;
    }

    if (type == DOUBLE){
        jsonObject->put(key, jsonChild->getDouble());
        return;
    }

    if (type == JSON_ARRAY){
        JsonArray *jsonArray = jsonChild->getJsonArray();
        jsonObject->put(key, jsonArray);
        return;
    }

    if (type == JSON_OBJECT){
        JsonObject *jsonObjectValue = jsonChild->getJsonObject();
        jsonObject->put(key, jsonObjectValue);
        return;
    }

    if (type == STRING)
        jsonObject->put(key, jsonChild->getString());
}

JsonChild* JsonParser::getValue(int &cursor){
    skipSpaces(cursor);

    if (cursor >= rawJsonSize){
        error = "No value of key on json object found" + getPosition(cursor - 1);
        return NULL;
    }

    if (rawJson[cursor] == '{')
        return parseToJsonObject(cursor);

    if (rawJson[cursor] == '[')
        return parseToJsonArray(cursor);

    if (rawJson[cursor] == '"'){
        std::string value = parseToString(cursor, "value");
        if (hasError()) return NULL;

        JsonChild *jsonChild = new JsonChild(value);
        jsonChilds.push_back(jsonChild);
        return jsonChild;
    }

    if (isDigit(rawJson[cursor])){
        JsonChild *jsonChild = parseToNumberString(cursor);
        if (hasError()) return NULL;
        return jsonChild;
    }

    if (rawJson.find("true", cursor) == cursor){
        JsonChild *jsonChild = new JsonChild(true);
        jsonChilds.push_back(jsonChild);
        cursor += 4;
        return jsonChild;
    }

    if (rawJson.find("false", cursor) == cursor){
        JsonChild *jsonChild = new JsonChild(false);
        jsonChilds.push_back(jsonChild);
        cursor += 5;
        return jsonChild;
    }

    if (rawJson.find("null", cursor) == cursor){
        std::string empty = "";
        JsonChild *jsonChild = new JsonChild(empty);
        jsonChilds.push_back(jsonChild);
        cursor += 4;
        return jsonChild;
    }

    std::string c = "" + rawJson[cursor];
    error = "Illegal character \"" + c + "\"" + getPosition(cursor);
    return NULL;
}

JsonChild* JsonParser::parseToNumberString(int &cursor){
    int indexLastNumber = -1;
    bool hasDot = false;
    for (int i = cursor; i < rawJsonSize; i++){
        if (isDigit(rawJson[i])){
            indexLastNumber = i;
            continue;
        }
        if (rawJson[i] == DOT){
            if (hasDot){
                error = "Number format error" + getPosition(i);
                return  NULL;
            }
            hasDot = true;
            continue;
        }
        break;
    }

    if (indexLastNumber == 1){
        error = "No number format found" + getPosition(cursor);
        return  NULL;
    }

    int sizeNumber = indexLastNumber + 1 - cursor;
    std::string stringNumber = rawJson.substr(cursor, sizeNumber);

    cursor += sizeNumber;

    JsonChild *jsonChild = NULL;
    if (hasDot)
        jsonChild = new JsonChild(0.0);
    else
        jsonChild = new JsonChild(0);

    jsonChilds.push_back(jsonChild);
    return jsonChild;
}

bool JsonParser::isDigit(const char &character){
    for (int i = 0; i < digitsSize; i++){
        if (character == digits[i])
            return true;
    }
    return false;
}

std::string JsonParser::getKey(int &cursor){
    return parseToString(cursor, "key");
}

std::string JsonParser::parseToString(int &cursor, const std::string &context){
    skipSpaces(cursor);

    if (cursor >= rawJsonSize){
        error = "No " + context + " found" + getPosition(cursor - 1);
        return "";
    }

    if (rawJson[cursor] != DOUBLE_QUOT){
        error = context + " not begin with double quot" + getPosition(cursor - 1);
        return "";
    }

    cursor++;

    int indexSecondQuot = getIndexQuot(rawJson, cursor);
    if (indexSecondQuot == -1){
        error = "End of " + context + "not found" + getPosition(cursor - 1);
        return "";
    }

    int sizeValue = indexSecondQuot - cursor;
    std::string value = "";
    if (sizeValue > 0)
        value = rawJson.substr(cursor, sizeValue);

    cursor += sizeValue + 1;
    return value;
}

int JsonParser::getIndexQuot(const std::string &rawJson, const int &cursor){
    for (int i = cursor; i < rawJsonSize; i++){
        if (rawJson[i] != DOUBLE_QUOT) continue;
        if (isEscapedQuot(rawJson, i)) continue;
        return i;
    }
    return -1;
}

bool JsonParser::isEscapedQuot(const std::string &rawJson, const int &cursor){
    int backSlashSize = 0;
    for (int i = cursor - 1; i >= 0; i--){
        if (rawJson[i] != BACK_SLASH) break;
        backSlashSize++;
    }

    return backSlashSize % 2 != 0;
}

bool JsonParser::hasError(){
    return !error.empty();
}

std::string JsonParser::getError(){
    return error;
}

void JsonParser::gotoFirstJsonCharacter(int &cursor){
    if (rawJson.empty()){
        error = "Raw json is empty";
        return;
    }

    skipSpaces(cursor);

    if (cursor >= rawJsonSize){
        error = "No format json found" + getPosition(cursor - 1);
        return;
    }

    if (rawJson[cursor] == '{' || rawJson[cursor] == '[')
        return;

    error = "No format json found" + getPosition(cursor);
}

void JsonParser::skipSpaces(int &cursor){
    for (int i = cursor; i < rawJsonSize; i++){
        char c = rawJson[i];
        if (c == ' ' || c == '\n' || c == '\r' || c == '\t')
            cursor++;
        else
            break;
    }
}

std::string JsonParser::getPosition(const int &cursor){
    return " at " + JsonChild::intToString(cursor) + " position";
}

void JsonParser::removeJsonChilds(){
    jsonChilds.push_back(jsonChild);
    int size = jsonChilds.size();
    for (int i = 0; i < size; i++){
        JsonChild *jsonChild = jsonChilds.at(i);
        delete jsonChild;
        jsonChild = NULL;
    }
    jsonChilds.clear();
}

JsonParser::~JsonParser(){
    removeJsonChilds();
}
