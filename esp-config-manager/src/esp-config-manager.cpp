#include "esp-config-manager.h"

void ESPConfigManager::add_item(ConfigItem *child)
{
    if (_base_item == nullptr)
    {
        _base_item = child;
        return;
    }
    _base_item->add_sibling(child);
}

int ESPConfigManager::count_items(void)
{
    ConfigItem *current = _base_item;

    int total_count = 0;
    while (current != nullptr)
    {
        total_count++;
        current = current->get_sibling();
    }
    return total_count;
}

void ESPConfigManager::load(void)
{
    if (_base_item == nullptr)
    {
        return;
    }

    if (!LittleFS.exists(F("/esp_cfg.json")))
    {
        return;
    }

    DynamicJsonDocument doc(2048);

    File dataFile = LittleFS.open(F("/esp_cfg.json"), "r");
    if (!dataFile)
    {
        Serial.println("Failed to open config file for reading");
        return;
    }
    deserializeJson(doc, dataFile);

    // ConfigGroup *current = _base_group;
    load_from_json(doc);
}

void ESPConfigManager::load_from_json(JsonDocument &doc)
{
    ConfigItem *current = _base_item;

    if (doc.containsKey(F("cfg")))
    {

        JsonObject json_object = doc[F("cfg")];

        while (current != nullptr)
        {
            current->item_from_json(json_object);
            current = current->get_sibling();
        }
    }
}

void ESPConfigManager::save_to_json(JsonDocument &doc)

{
    ConfigItem *current = _base_item;
    JsonObject child_object = doc.createNestedObject(F("cfg"));

    while (current != nullptr)
    {
        current->item_to_json(child_object);
        current = current->get_sibling();
    }
}

void ESPConfigManager::save(void)
{
    DynamicJsonDocument doc(2048);
    save_to_json(doc);
    File dataFile = LittleFS.open(F("/esp_cfg.json"), "w");
    if (!dataFile)
    {
        Serial.println("Failed to open config file for writing");
    }
    serializeJson(doc, dataFile);
}

void ESPConfigManager::clear(void)
{
    LittleFS.remove(F("/esp_cfg.json"));
}