#ifndef ESP_CONFIG_MANAGER_H_
#define ESP_CONFIG_MANAGER_H_
#include <Arduino.h>

#include "LittleFS.h"
#include "arduinojson.h"
#include "esp-config-items.h"

class ESP32Config {
   public:
    ConfigGroup *_base_group = nullptr;

    ESP32Config(void);
    ~ESP32Config(void);
    void add_group(ConfigGroup *child);
    void set_wifissid(const char *ssid);
    void set_wifikey(const char *key);
    void set_wifi(const char *ssid, const char *key);
    void save(void);
    void save_to_json(JsonDocument &doc);
    void load(void);

    const char *get_ssid() { return _ssid; };
    const char *get_key() { return _key; };
    char *_ssid;
    char *_key;
};

#endif