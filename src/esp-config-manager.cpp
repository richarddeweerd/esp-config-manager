#include "esp-config-manager.h"

#include "LittleFS.h"

ConfigItemBase::ConfigItemBase(void) {}
ConfigItemBase::~ConfigItemBase(void) {}


void ConfigItemBase::setup(const char *id, const char *label) {
    _id = id;
    _label = label;
}

void ConfigItemBase::add_sibling(ConfigItemBase *sibling) {
    ConfigItemBase *current = this;
    while (current->_sibling != nullptr) {
        current = current->_sibling;
    }
    current->_sibling = sibling;
}

void ConfigItemBase::item_from_json(JsonObject &doc) {}
void ConfigItemBase::item_to_json(JsonObject &doc) {}

void ConfigItemBase::from_json(JsonObject &doc) {
    if (doc.containsKey(_id)) {
        item_from_json(doc);
    }
    if (_sibling != nullptr) {
        _sibling->from_json(doc);
    }
}

void ConfigItemBase::to_json(JsonObject &doc) {
    item_to_json(doc);
    if (_sibling != nullptr) {
        _sibling->to_json(doc);
    }
}

ConfigItem::ConfigItem(void) {}

ConfigItem::~ConfigItem(void) {}

void ConfigItem::item_from_json(JsonObject &doc) {}

void ConfigItem::item_to_json(JsonObject &doc) {}

ConfigGroup::ConfigGroup(void) {}

ConfigGroup::~ConfigGroup(void) {}

void ConfigGroup::add_child(ConfigItem *child) {
    if (_child == nullptr) {
        _child = child;
        return;
    }
    _child->add_sibling(child);
}

void ConfigGroup::to_json(JsonObject &doc) {
    if (_child != nullptr) {
        JsonObject child_object = doc.createNestedObject(_id);
        _child->to_json(child_object);
    }
}

void ConfigGroup::from_json(JsonObject &doc) {
    if (_child != nullptr) {
        if (doc.containsKey(_id)) {
            JsonObject child_object = doc[_id];
            _child->from_json(child_object);
        }
    }
    if (_sibling != nullptr) {
        _sibling->from_json(doc);
    }
}

ConfigItemBool::ConfigItemBool() {}

void ConfigItemBool::init(const char *id, const char *label, bool defaultValue) {
    setup(id, label);
    _value = defaultValue;
}

void ConfigItemBool::item_from_json(JsonObject &doc) {
    _value = doc[_id];
}

void ConfigItemBool::item_to_json(JsonObject &doc) {
    doc[_id] = _value;
}

ConfigItemInt::ConfigItemInt() {}
void ConfigItemInt::init(const char *id, const char *label, int defaultValue) {
    setup(id, label);
    _value = defaultValue;
}

void ConfigItemInt::item_from_json(JsonObject &doc) {
    _value = doc[_id];
}

void ConfigItemInt::item_to_json(JsonObject &doc) {
    doc[_id] = _value;
}

ConfigItemString::ConfigItemString() {}

void ConfigItemString::init(const char *id, const char *label, const char *defaultValue) {
    setup(id, label);
    set(defaultValue);
}

void ConfigItemString::set(const char *new_value) {
    int _length = strlen(new_value);
    _value = new char[_length + 1];
    memset(_value, 0, _length + 1);  // explicit null
    if (new_value != NULL) {
        strncpy(_value, new_value, _length);
    }
}

void ConfigItemString::item_from_json(JsonObject &doc) {
    set(doc[_id]);
}

void ConfigItemString::item_to_json(JsonObject &doc) {
    doc[_id] = _value;
}

ConfigItemIP::ConfigItemIP() {}
void ConfigItemIP::init(const char *id, const char *label, IPAddress ip) {
    setup(id, label);
    set(ip);
}

void ConfigItemIP::item_from_json(JsonObject &doc) {
    JsonArray array = doc[_id];
    int i = 0;
    for (JsonVariant value : array) {
        _value[i] = value.as<int>();
        i++;
    }
}

void ConfigItemIP::item_to_json(JsonObject &doc) {
    JsonArray ip = doc.createNestedArray(_id);
    for (int i = 0; i < 4; i++) {
        ip.add(_value[i]);
    }
}

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
