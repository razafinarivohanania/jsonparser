#include "jsonobject.h"

JsonObject::JsonObject()
{
}

void JsonObject::put(const std::string &key, const double &value){
    if (key.empty()) return;
    jsonChilds[key] = new JsonChild(value);
}

JsonChild* JsonObject::getJsonChild(const std::string &key){
    if (jsonChilds.empty()) return NULL;
    return jsonChilds[key];
}

void JsonObject::put(const std::string &key, const int &value){
    if (key.empty()) return;
    jsonChilds[key] = new JsonChild(value);
}

void JsonObject::put(const std::string &key, JsonArray *value){
    if (key.empty()) return;
    jsonChilds[key] = new JsonChild(value);
}

void JsonObject::put(const std::string &key, JsonObject *value){
    if (key.empty()) return;
    jsonChilds[key] = new JsonChild(value);
}

void JsonObject::put(const std::string &key, const std::string &value){
    if (key.empty()) return;
    jsonChilds[key] = new JsonChild(value);
}

void JsonObject::put(const std::string &key, const bool &value){
    if (key.empty()) return;
    jsonChilds[key] = new JsonChild(value);
}

void JsonObject::put(const std::string &key, JsonChild *value){
    JsonType type = value->getType();
    if (type == BOOL){
        put(key, value->getBool());
        return;
    }
    if (type == DOUBLE){
        put(key, value->getDouble());
        return;
    }
    if (type == INT){
        put(key, value->getInt());
        return;
    }
    if (type == STRING){
        put(key, value->getString());
        return;
    }
    if (type == JSON_OBJECT){
        JsonObject *jsonObject = value->getJsonObject();
        put(key, jsonObject);
        return;
    }

    if (type == JSON_ARRAY){
        JsonArray *jsonArray = value->getJsonArray();
        put(key, jsonArray);
    }
}

bool JsonObject::has(const std::string &key){
    return false;
}

std::string JsonObject::toString(const bool &isToFormat){
    std::string value;
    if (jsonChilds.empty()) return value;

    value = "{";
    std::map<std::string, JsonChild*>::iterator it;
    for (it = jsonChilds.begin(); it != jsonChilds.end(); it++){
        std::string key = it->first;
        if (key.empty()) continue;

        JsonChild *jsonChild = it->second;
        if (jsonChild == NULL) continue;
        value += "\"" + key + "\":" + jsonChild->toString() + ",";
    }
    value = value.erase(value.size() - 1);
    value += "}";

    return value;
}

void JsonObject::remove(const std::string key){
    if (key.empty()) return;

    JsonChild *jsonChild = jsonChilds[key];
    if (jsonChild != NULL) {
        delete jsonChild;
        jsonChild = NULL;
    }

    jsonChilds.erase(key);
}

JsonObject::~JsonObject(){
    if (jsonChilds.empty()) return;

    std::map<std::string, JsonChild*>::iterator it;
    for (it = jsonChilds.begin(); it != jsonChilds.end(); it++){
        JsonChild *jsonChild = it->second;
        if (jsonChild == NULL) continue;
        delete jsonChild;
        jsonChild = NULL;
    }

    jsonChilds.clear();
}
