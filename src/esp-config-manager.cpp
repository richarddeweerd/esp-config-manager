#include "esp-config-manager.h"

ESP32Config::ESP32Config(void) {}

ESP32Config::~ESP32Config(void) {}

void ESP32Config::add_group(ConfigGroup *child) {
    if (_base_group == nullptr) {
        _base_group = child;
        return;
    }
    _base_group->add_sibling(child);
}

void ESP32Config::set_wifissid(const char *ssid) {
    int _length = strlen(ssid);
    _ssid = new char[_length + 1];
    memset(_ssid, 0, _length + 1);  // explicit null
    if (ssid != NULL) {
        strncpy(_ssid, ssid, _length);
    }
}

void ESP32Config::set_wifikey(const char *key) {
    int _length = strlen(key);
    _key = new char[_length + 1];
    memset(_key, 0, _length + 1);  // explicit null
    if (key != NULL) {
        strncpy(_key, key, _length);
    }
}
void ESP32Config::set_wifi(const char *ssid, const char *key) {
    set_wifissid(ssid);
    set_wifikey(key);
}

void ESP32Config::load(void) {
    if (_base_group == nullptr) {
        return;
    }
    DynamicJsonDocument doc(2048);

    File dataFile = LittleFS.open("/esp_cfg.json", "r");
    if (!dataFile) {
        Serial.println("Failed to open config file for writing");
    }
    deserializeJson(doc, dataFile);
    ConfigGroup *current = _base_group;
    if (doc.containsKey(current->_id)) {
        JsonObject json_object = doc["cfg"];
        current->from_json(json_object);
    }
}
void ESP32Config::save_to_json(JsonDocument &doc) {
    JsonObject child_object = doc.createNestedObject(F("wifi"));
    child_object[F("ssid")] = _ssid;
    child_object[F("key")] = _key;
    if (_base_group == nullptr) {
        return;
    }
    child_object = doc.createNestedObject(F("cfg"));
    _base_group->to_json(child_object);
}

void ESP32Config::save(void) {
    // ConfigGroup *current = _child;
    DynamicJsonDocument doc(2048);
    save_to_json(doc);
    // save
    // current->to_json(doc);
    // while (current->_sibling != nullptr) {
    //     current = current->_sibling;
    //     current->to_json(doc);
    // }
    // File dataFile = LittleFS.open("/esp_cfg.json", "w");
    // if (!dataFile) {
    //     Serial.println("Failed to open config file for writing");
    // }
    // serializeJson(doc, dataFile);
    // serializeJsonPretty(doc, Serial);
}
