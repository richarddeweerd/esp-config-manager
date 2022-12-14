#ifndef ESP_CONFIG_ITEMS_H_
#define ESP_CONFIG_ITEMS_H_
#include <Arduino.h>

#include "arduinojson.h"

class ConfigItemBase {
   public:
    ConfigItemBase(void);
    ~ConfigItemBase(void);

    void setup(const char *id, const char *label);
    void setup(const char *id);
    void add_sibling(ConfigItemBase *sibling);
    ConfigItemBase *_sibling = nullptr;
    const char *_id;
    const char *_label;
    virtual void to_json(JsonObject &doc);
    virtual void from_json(JsonObject &doc);
    virtual void item_from_json(JsonObject &doc);
    virtual void item_to_json(JsonObject &doc);
};

class ConfigItem : public ConfigItemBase {
   public:
    ConfigItem(void);
    ~ConfigItem(void);

    void item_from_json(JsonObject &doc);
    virtual void item_to_json(JsonObject &doc);
    // virtual void to_json(JsonObject &doc);
};

class ConfigGroup : public ConfigItemBase {
   public:
    ConfigGroup(void);
    ~ConfigGroup(void);

    ConfigItem *_child = nullptr;

    void add_child(ConfigItem *child);

    void to_json(JsonObject &doc);

    void from_json(JsonObject &doc);
};

class ConfigItemBool : public ConfigItem {
   public:
    ConfigItemBool();
    void init(const char *id, const char *label, bool defaultValue);
    void init(const char *id, bool defaultValue);

    void set(bool val) { _value = val; };
    bool get() { return _value; };
    void item_from_json(JsonObject &doc);
    void item_to_json(JsonObject &doc);

   private:
    bool _value;
};

class ConfigItemInt : public ConfigItem {
   public:
    ConfigItemInt();
    void init(const char *id, const char *label, int defaultValue);
    void init(const char *id, int defaultValue);
    void set(int val) { _value = val; };

    int get() { return _value; };
    void item_from_json(JsonObject &doc);
    void item_to_json(JsonObject &doc);

   private:
    int _value;
};

class ConfigItemFloat : public ConfigItem {
   public:
    ConfigItemFloat();
    void init(const char *id, const char *label, float defaultValue);
    void init(const char *id, float defaultValue);
    void set(float val) { _value = val; };

    float get() { return _value; };
    void item_from_json(JsonObject &doc);
    void item_to_json(JsonObject &doc);

   private:
    float _value;
};

class ConfigItemString : public ConfigItem {
   public:
    ConfigItemString();
    void init(const char *id, const char *label, const char *defaultValue);
    void init(const char *id, const char *defaultValue);

    void set(const char *new_value);
    const char *get() { return _value; };
    void item_from_json(JsonObject &doc);
    void item_to_json(JsonObject &doc);

   private:
    char *_value;
};

class ConfigItemIP : public ConfigItem {
   public:
    ConfigItemIP();
    void init(const char *id, const char *label, IPAddress ip);
    void init(const char *id, IPAddress ip);

    void set(IPAddress val) { _value = val; };

    IPAddress get() { return _value; };
    void item_from_json(JsonObject &doc);
    void item_to_json(JsonObject &doc);

   private:
    IPAddress _value;
};
#endif