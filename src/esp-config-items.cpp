#include "esp-config-items.h"

ConfigItemBase::ConfigItemBase(void) {}
ConfigItemBase::~ConfigItemBase(void) {}

void ConfigItemBase::setup(const char *id, const char *label) {
    _id = id;
    _label = label;
}
void ConfigItemBase::setup(const char *id) {
    _id = id;
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

void ConfigItemBool::init(const char *id, bool defaultValue) {
    setup(id);
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
void ConfigItemInt::init(const char *id, int defaultValue) {
    setup(id);
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
void ConfigItemString::init(const char *id, const char *defaultValue) {
    setup(id);
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

void ConfigItemIP::init(const char *id, IPAddress ip) {
    setup(id);
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
