#ifndef ESP_CONFIG_MANAGER_H_
#define ESP_CONFIG_MANAGER_H_
#include <Arduino.h>

#include "LittleFS.h"
#include "ArduinoJson.h"
#include "esp-config-items.h"

class ESPConfigManager
{
public:
    ESPConfigManager(void) {};
    void add_item(ConfigItem *child);
    int count_items(void);

    void save_to_json(JsonDocument &doc);
    void load_from_json(JsonDocument &doc);

    void load(void);
    void save(void);
    void clear(void);

private:
    ConfigItem *_base_item = nullptr;
};

#endif