#ifndef ESP_CONFIG_ITEMS_H_
#define ESP_CONFIG_ITEMS_H_
#include <Arduino.h>

#include "ArduinoJson.h"

class ConfigItem
{
public:
    ConfigItem(void) {};

    void setup(const char *id);
    void add_sibling(ConfigItem *sibling);

    // void from_json(JsonObject &doc);
    virtual void item_from_json(JsonObject &doc);
    virtual void item_to_json(JsonObject &doc);
    const char *get_id() const { return _id; }
    const char *get_label() const { return _label; }
    ConfigItem *get_sibling() const { return _sibling; }

private:
    ConfigItem *_sibling = nullptr;

    const char *_id;
    const char *_label;
};

class ConfigItemBool : public ConfigItem
{
public:
    void init(char const *id, bool defaultValue);
    void set(bool val) { _value = val; };
    bool get() { return _value; };
    void item_from_json(JsonObject &doc);
    void item_to_json(JsonObject &doc);

private:
    bool _value;
};

class ConfigItemInt : public ConfigItem
{
public:
    void init(char const *id, int defaultValue);
    void set(int val) { _value = val; };
    int get() { return _value; };
    void item_from_json(JsonObject &doc);
    void item_to_json(JsonObject &doc);

private:
    int _value;
};

class ConfigItemFloat : public ConfigItem
{
public:
    void init(char const *id, float defaultValue);
    void set(float val) { _value = val; };
    float get() { return _value; };
    void item_from_json(JsonObject &doc);
    void item_to_json(JsonObject &doc);

private:
    float _value;
};

class ConfigItemString : public ConfigItem
{
public:
    void init(char const *id, const char *defaultValue);
    void set(const char *new_value);
    const char *get() { return _value; };
    void item_from_json(JsonObject &doc);
    void item_to_json(JsonObject &doc);

private:
    char *_value;
};

class ConfigItemIP : public ConfigItem
{
public:
    void init(char const *id, IPAddress defaultValue);
    void set(IPAddress val) { _value = val; };
    IPAddress get() { return _value; };
    void item_from_json(JsonObject &doc);
    void item_to_json(JsonObject &doc);

private:
    IPAddress _value;
};

#endif // ESP_CONFIG_ITEMS_H_
