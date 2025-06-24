#include <unity.h>
#include "ArduinoJson.h"
#include "esp-config-manager.h"

void test_add_item(void)
{
    ESPConfigManager manager;
    ConfigItemBool item1;
    item1.init("item1", true);

    manager.add_item(&item1);

    TEST_ASSERT_EQUAL(item1.get_id(), "item1");
    TEST_ASSERT_EQUAL(manager.count_items(), 1);
}

void test_add_item_sibling(void)
{
    ESPConfigManager manager;
    ConfigItemBool item1;
    item1.init("item1", true);
    ConfigItemBool item2;
    item2.init("item2", false);

    manager.add_item(&item1);
    manager.add_item(&item2);

    TEST_ASSERT_EQUAL(item1.get_id(), "item1");
    TEST_ASSERT_EQUAL(item2.get_id(), "item2");
    TEST_ASSERT_EQUAL(manager.count_items(), 2);
}

void test_save_to_json(void)
{
    ESPConfigManager manager;
    ConfigItemBool item1;
    item1.init("item1", true);
    ConfigItemInt item2;
    item2.init("item2", 42);

    manager.add_item(&item1);
    manager.add_item(&item2);

    DynamicJsonDocument doc(512);
    manager.save_to_json(doc);

    String output;
    serializeJson(doc, output);
    TEST_ASSERT_EQUAL(2, manager.count_items());
    TEST_ASSERT_EQUAL_STRING("{\"cfg\":{\"item1\":true,\"item2\":42}}", output.c_str());
}

void test_load_from_json(void)
{

    DynamicJsonDocument doc(512);
    deserializeJson(doc, "{\"cfg\":{\"item1\":true,\"item2\":93}}");

    ESPConfigManager manager;
    ConfigItemBool item1;
    item1.init("item1", false);
    ConfigItemInt item2;
    item2.init("item2", 18);
    manager.add_item(&item1);
    manager.add_item(&item2);

    manager.load_from_json(doc);
    TEST_ASSERT_EQUAL(true, item1.get());
    TEST_ASSERT_EQUAL(93, item2.get());
}

void test_save_to_fs(void)
{
    ESPConfigManager manager;
    ConfigItemBool item1;
    item1.init("item1", true);
    ConfigItemInt item2;
    item2.init("item2", 42);

    manager.add_item(&item1);
    manager.add_item(&item2);

    // Initialize LittleFS
    if (!LittleFS.begin())
    {
        Serial.println("Failed to mount LittleFS");
        TEST_FAIL_MESSAGE("LittleFS mount failed");
    }

    // Save to file
    manager.save();

    DynamicJsonDocument doc(2048);

    File dataFile = LittleFS.open(F("/esp_cfg.json"), "r");
    if (!dataFile)
    {
        Serial.println("Failed to open config file for reading");
        TEST_FAIL_MESSAGE("Failed to open config file for reading");
    }
    deserializeJson(doc, dataFile);

    dataFile.close();
    String output;
    serializeJson(doc, output);
    LittleFS.end();
    TEST_ASSERT_EQUAL_STRING("{\"cfg\":{\"item1\":true,\"item2\":42}}", output.c_str());
}

void test_load_from_fs(void)
{
    ESPConfigManager manager;
    ConfigItemBool item1;
    item1.init("item1", false);
    ConfigItemInt item2;
    item2.init("item2", 95);

    manager.add_item(&item1);
    manager.add_item(&item2);

    // Initialize LittleFS
    if (!LittleFS.begin())
    {
        Serial.println("Failed to mount LittleFS");
        TEST_FAIL_MESSAGE("LittleFS mount failed");
    }

    // Load from file
    TEST_ASSERT_EQUAL(item1.get(), false);
    TEST_ASSERT_EQUAL(item2.get(), 95);

    manager.load();

    TEST_ASSERT_EQUAL(item1.get(), true);
    TEST_ASSERT_EQUAL(item2.get(), 42);

    LittleFS.end();
}
