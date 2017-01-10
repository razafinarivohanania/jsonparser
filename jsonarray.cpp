#include "jsonarray.h"

JsonArray::JsonArray()
{
}

bool JsonArray::has(const u_int &index){
    if (jsonChilds.empty()) return false;
    return index < jsonChilds.size();
}

void JsonArray::add(const bool &value){
    jsonChilds.push_back(new JsonChild(value));
}

void JsonArray::add(const double &value){
    jsonChilds.push_back(new JsonChild(value));
}

void JsonArray::add(const int &value){
    jsonChilds.push_back(new JsonChild(value));
}

void JsonArray::add(JsonArray *value){
    jsonChilds.push_back(new JsonChild(value));
}

void JsonArray::add(JsonObject *value){
    jsonChilds.push_back(new JsonChild(value));
}

void JsonArray::add(const std::string &value){
    jsonChilds.push_back(new JsonChild(value));
}

void JsonArray::add(JsonChild *value){
    jsonChilds.push_back(value);
}

void JsonArray::remove(const u_int &index){
    if (jsonChilds.empty()) return;
    int size = jsonChilds.size();
    if ((int) index >= size) return;

    std::vector<JsonChild*> newJsonChilds;
    for (int i = 0; i < size; i++){
        JsonChild *jsonChild = jsonChilds.at(i);
        if (i == (int) index){
            delete jsonChild;
            jsonChild = NULL;
        } else {
            newJsonChilds.push_back(jsonChild);
        }
    }

    jsonChilds.clear();
    jsonChilds = newJsonChilds;
}

std::string JsonArray::toString(const bool &isToFormat){
    if (jsonChilds.empty()) return "";
    int size = jsonChilds.size();
    std::string value = "[";
    for (int i = 0; i < size; i++){
        JsonChild *jsonChild = jsonChilds.at(i);
        value += jsonChild->toString();
        if (i != size - 1) value += ",";
    }
    value += "]";
    return value;
}

JsonChild* JsonArray::getJsonChild(const u_int &index){
    if (!has(index)) return NULL;
    return jsonChilds.at(index);
}

JsonArray::~JsonArray(){
    if (jsonChilds.empty()) return;

    int size = jsonChilds.size();
    for (int i = 0; i < size; i++){
        JsonChild* jsonChild = jsonChilds.at(i);
        delete jsonChild;
        jsonChild = NULL;
    }

    jsonChilds.clear();
}
