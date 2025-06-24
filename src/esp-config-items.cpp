#include "esp-config-items.h"

void ConfigItem::setup(const char *id)
{
    _id = id;
}

void ConfigItem::add_sibling(ConfigItem *sibling)
{
    ConfigItem *current = this;
    while (current->_sibling != nullptr)
    {
        current = current->_sibling;
    }
    current->_sibling = sibling;
}

void ConfigItemBool::init(char const *id, bool defaultValue)
{
    setup(id);
    _value = defaultValue;
}

void ConfigItemBool::item_from_json(JsonObject &doc)
{
    _value = doc[get_id()];
}

void ConfigItemBool::item_to_json(JsonObject &doc)
{
    doc[get_id()] = _value;
}

void ConfigItemInt::init(char const *id, int defaultValue)
{
    setup(id);
    _value = defaultValue;
}
void ConfigItemInt::item_from_json(JsonObject &doc)
{
    _value = doc[get_id()];
}
void ConfigItemInt::item_to_json(JsonObject &doc)
{
    doc[get_id()] = _value;
}

void ConfigItemFloat::init(char const *id, float defaultValue)
{
    setup(id);
    _value = defaultValue;
}
void ConfigItemFloat::item_from_json(JsonObject &doc)
{
    _value = doc[get_id()];
}
void ConfigItemFloat::item_to_json(JsonObject &doc)
{
    doc[get_id()] = _value;
}

void ConfigItemString::init(char const *id, const char *defaultValue)
{
    setup(id);
    set(defaultValue);
}
void ConfigItemString::set(const char *new_value)
{
    int _length = strlen(new_value);
    _value = new char[_length + 1];
    memset(_value, 0, _length + 1); // explicit null
    if (new_value != NULL)
    {
        strncpy(_value, new_value, _length);
    }
}

void ConfigItemString::item_from_json(JsonObject &doc)
{
    const char *value = doc[get_id()];
    if (value != nullptr)
    {
        set(value);
    }
}
void ConfigItemString::item_to_json(JsonObject &doc)
{
    if (_value != nullptr)
    {
        doc[get_id()] = _value;
    }
}

void ConfigItemIP::init(char const *id, IPAddress defaultValue)
{
    setup(id);
    _value = defaultValue;
}

void ConfigItemIP::item_from_json(JsonObject &doc)
{
    JsonArray array = doc[get_id()];
    int i = 0;
    for (JsonVariant value : array)
    {
        _value[i] = value.as<int>();
        i++;
    }
}

void ConfigItemIP::item_to_json(JsonObject &doc)
{
    JsonArray ip = doc.createNestedArray(get_id());
    for (int i = 0; i < 4; i++)
    {
        ip.add(_value[i]);
    }
}