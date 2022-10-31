#include "esp-config-manager.h"

ESPConfigManager::ESPConfigManager(void) {
    _ssid = new char[1];
    memset(_ssid, 0, 1);
    _key = new char[1];
    memset(_key, 0, 1);
}

ESPConfigManager::~ESPConfigManager(void) {}

void ESPConfigManager::add_group(ConfigGroup *child) {
    if (_base_group == nullptr) {
        _base_group = child;
        return;
    }
    _base_group->add_sibling(child);
}

void ESPConfigManager::set_wifissid(const char *ssid) {
    int _length = strlen(ssid);
    _ssid = new char[_length + 1];
    memset(_ssid, 0, _length + 1);  // explicit null
    if (ssid != NULL) {
        strncpy(_ssid, ssid, _length);
    }
    // _ssid_set = true;
}

void ESPConfigManager::set_wifikey(const char *key) {
    int _length = strlen(key);
    _key = new char[_length + 1];
    memset(_key, 0, _length + 1);  // explicit null
    if (key != NULL) {
        strncpy(_key, key, _length);
    }
    // _key_set = true;
}
void ESPConfigManager::set_wifi(const char *ssid, const char *key) {
    set_wifissid(ssid);
    set_wifikey(key);
}

void ESPConfigManager::load(void) {
    if (_base_group == nullptr) {
        return;
    }
    if (!LittleFS.exists(F("/esp_cfg.json"))) {
        return;
    }

    DynamicJsonDocument doc(2048);

    File dataFile = LittleFS.open(F("/esp_cfg.json"), "r");
    if (!dataFile) {
        Serial.println("Failed to open config file for reading");
        return;
    }
    deserializeJson(doc, dataFile);

    ConfigGroup *current = _base_group;
    if (doc.containsKey(F("wifi"))) {
    }

    if (doc.containsKey(F("cfg"))) {
        JsonObject json_object = doc[F("cfg")];
        if (json_object.containsKey(current->_id)) {
            serializeJsonPretty(json_object, Serial);
            current->from_json(json_object);
        }
    }
}

void ESPConfigManager::save_wifi_json(JsonDocument &doc) {
    if ((strlen(_ssid) == 0) && (strlen(_key) == 0)) {
        return;
    }
    JsonObject child_object = doc.createNestedObject(F("wifi"));
    if (strlen(_ssid) > 0) {
        child_object[F("ssid")] = get_ssid();
    }
    if (strlen(_key) > 0) {
        child_object[F("key")] = get_key();
    }
}

void ESPConfigManager::save_to_json(JsonDocument &doc) {
    save_wifi_json(doc);
    if (_base_group == nullptr) {
        return;
    }
    JsonObject child_object = doc.createNestedObject(F("cfg"));
    _base_group->to_json(child_object);
}

void ESPConfigManager::save(void) {
    DynamicJsonDocument doc(2048);
    save_to_json(doc);
    File dataFile = LittleFS.open(F("/esp_cfg.json"), "w");
    if (!dataFile) {
        Serial.println("Failed to open config file for writing");
    }
    serializeJson(doc, dataFile);
    // serializeJsonPretty(doc, Serial);
}

void ESPConfigManager::clear(void) {
    LittleFS.remove(F("/esp_cfg.json"));
}
