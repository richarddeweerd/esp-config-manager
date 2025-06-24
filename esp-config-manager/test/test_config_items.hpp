#include <unity.h>
#include "ArduinoJson.h"
#include "esp-config-items.h"

void test_config_item_bool_base_no_label(void)
{
    ConfigItemBool ci;
    ci.init("tag", true);
    TEST_ASSERT_EQUAL(ci.get(), true);
    ci.set(false);
    TEST_ASSERT_EQUAL(ci.get(), false);
}

void test_config_item_bool_base_fromJSON(void)
{
    DynamicJsonDocument doc(512);

    deserializeJson(doc, "{\"cfg\":{\"tag\":true}}");

    JsonObject child_object = doc["cfg"];

    ConfigItemBool ci;
    ci.init("tag", false);
    TEST_ASSERT_EQUAL(ci.get(), false);
    ci.item_from_json(child_object);
    TEST_ASSERT_EQUAL(ci.get(), true);
}

void test_config_item_bool_base_toJSON(void)
{
    String out;
    ConfigItemBool ci;
    ci.init("tag", true);
    DynamicJsonDocument doc(512);
    JsonObject child_object = doc.createNestedObject("cfg");
    ci.item_to_json(child_object);
    serializeJson(doc, out);
    TEST_ASSERT_EQUAL_STRING(out.c_str(), "{\"cfg\":{\"tag\":true}}");
    out = "";
    ci.set(false);
    ci.item_to_json(child_object);
    serializeJson(doc, out);
    TEST_ASSERT_EQUAL_STRING(out.c_str(), "{\"cfg\":{\"tag\":false}}");
}

void test_config_item_int_base(void)
{
    ConfigItemInt ci;
    ci.init("tag", true);
    TEST_ASSERT_EQUAL(ci.get(), true);
    ci.set(false);
    TEST_ASSERT_EQUAL(ci.get(), false);
}

void test_config_item_int_base_fromJSON(void)
{
    DynamicJsonDocument doc(512);

    deserializeJson(doc, "{\"cfg\":{\"tag\":123}}");

    JsonObject child_object = doc["cfg"];

    ConfigItemInt ci;
    ci.init("tag", 0);
    TEST_ASSERT_EQUAL(ci.get(), 0);
    ci.item_from_json(child_object);
    TEST_ASSERT_EQUAL(ci.get(), 123);
}

void test_config_item_int_base_toJSON(void)
{
    String out;
    ConfigItemInt ci;
    ci.init("tag", 123);
    DynamicJsonDocument doc(512);
    JsonObject child_object = doc.createNestedObject("cfg");
    ci.item_to_json(child_object);
    serializeJson(doc, out);
    TEST_ASSERT_EQUAL_STRING(out.c_str(), "{\"cfg\":{\"tag\":123}}");
    out = "";
    ci.set(456);
    ci.item_to_json(child_object);
    serializeJson(doc, out);
    TEST_ASSERT_EQUAL_STRING(out.c_str(), "{\"cfg\":{\"tag\":456}}");
}

void test_config_item_float_base(void)
{
    ConfigItemFloat ci;
    ci.init("tag", 1.23f);
    TEST_ASSERT_EQUAL_FLOAT(ci.get(), 1.23f);
    ci.set(4.56f);
    TEST_ASSERT_EQUAL_FLOAT(ci.get(), 4.56f);
}
void test_config_item_float_base_fromJSON(void)
{
    DynamicJsonDocument doc(512);

    deserializeJson(doc, "{\"cfg\":{\"tag\":7.89}}");

    JsonObject child_object = doc["cfg"];

    ConfigItemFloat ci;
    ci.init("tag", 0.0f);
    TEST_ASSERT_EQUAL_FLOAT(ci.get(), 0.0f);
    ci.item_from_json(child_object);
    TEST_ASSERT_EQUAL_FLOAT(ci.get(), 7.89f);
}
void test_config_item_float_base_toJSON(void)
{
    String out;
    ConfigItemFloat ci;
    ci.init("tag", 1.32f);
    DynamicJsonDocument doc(512);
    JsonObject child_object = doc.createNestedObject("cfg");
    ci.item_to_json(child_object);
    serializeJson(doc, out);
    TEST_ASSERT_EQUAL_STRING("{\"cfg\":{\"tag\":1.320000052}}", out.c_str());
    out = "";
    ci.set(4.56f);
    ci.item_to_json(child_object);
    serializeJson(doc, out);
    TEST_ASSERT_EQUAL_STRING("{\"cfg\":{\"tag\":4.559999943}}", out.c_str());
}

void test_config_item_string_base(void)
{
    ConfigItemString ci;
    ci.init("tag", "Hello");
    TEST_ASSERT_EQUAL_STRING(ci.get(), "Hello");
    ci.set("World");
    TEST_ASSERT_EQUAL_STRING(ci.get(), "World");
}

void test_config_item_string_base_fromJSON(void)
{
    DynamicJsonDocument doc(512);

    deserializeJson(doc, "{\"cfg\":{\"tag\":\"Test String\"}}");

    JsonObject child_object = doc["cfg"];

    ConfigItemString ci;
    ci.init("tag", "Default");
    TEST_ASSERT_EQUAL_STRING(ci.get(), "Default");
    ci.item_from_json(child_object);
    TEST_ASSERT_EQUAL_STRING(ci.get(), "Test String");
}
void test_config_item_string_base_toJSON(void)
{
    String out;
    ConfigItemString ci;
    ci.init("tag", "Initial Value");
    DynamicJsonDocument doc(512);
    JsonObject child_object = doc.createNestedObject("cfg");
    ci.item_to_json(child_object);
    serializeJson(doc, out);
    TEST_ASSERT_EQUAL_STRING(out.c_str(), "{\"cfg\":{\"tag\":\"Initial Value\"}}");
    out = "";
    ci.set("Updated Value");
    ci.item_to_json(child_object);
    serializeJson(doc, out);
    TEST_ASSERT_EQUAL_STRING(out.c_str(), "{\"cfg\":{\"tag\":\"Updated Value\"}}");
}

void test_config_item_IP_base(void)
{
    ConfigItemIP ci;
    ci.init("tag", IPAddress(192, 168, 1, 1));
    TEST_ASSERT_EQUAL(ci.get(), IPAddress(192, 168, 1, 1));
    ci.set(IPAddress(10, 0, 0, 1));
    TEST_ASSERT_EQUAL(ci.get(), IPAddress(10, 0, 0, 1));
}

void test_config_item_IP_base_fromJSON(void)
{
    DynamicJsonDocument doc(512);
    deserializeJson(doc, "{\"cfg\":{\"tag\":[192,168,10,10]}}");
    JsonObject child_object = doc["cfg"];
    ConfigItemIP ci;
    ci.init("tag", IPAddress(0, 0, 0, 0));
    TEST_ASSERT_EQUAL(ci.get(), IPAddress(0, 0, 0, 0));
    ci.item_from_json(child_object);
    TEST_ASSERT_EQUAL(ci.get(), IPAddress(192, 168, 10, 10));
}

void test_config_item_IP_base_toJSON(void)
{
    String out;
    ConfigItemIP ci;
    ci.init("tag", IPAddress(192, 168, 1, 1));
    DynamicJsonDocument doc(512);
    JsonObject child_object = doc.createNestedObject("cfg");
    ci.item_to_json(child_object);
    serializeJson(doc, out);
    TEST_ASSERT_EQUAL_STRING("{\"cfg\":{\"tag\":[192,168,1,1]}}", out.c_str());

    out = "";
    ci.set(IPAddress(10, 0, 0, 1));
    ci.item_to_json(child_object);
    serializeJson(doc, out);
    TEST_ASSERT_EQUAL_STRING("{\"cfg\":{\"tag\":[10,0,0,1]}}", out.c_str());
}
