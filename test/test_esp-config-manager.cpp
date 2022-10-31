#include <gtest/gtest.h>

#include "arduinojson.h"
#include "esp-config-manager.h"

namespace {
TEST(ConfigItemBool, Base) {
    ConfigItemBool ci;
    ci.init("tag", "label", true);
    ASSERT_EQ(ci.get(), true);
    ci.set(false);
    ASSERT_EQ(ci.get(), false);
}

TEST(ConfigItemBool, Base_No_Label) {
    ConfigItemBool ci;
    ci.init("tag", true);
    ASSERT_EQ(ci.get(), true);
    ci.set(false);
    ASSERT_EQ(ci.get(), false);
}

TEST(ConfigItemBool, FromJSON) {
    DynamicJsonDocument doc(512);

    deserializeJson(doc, "{\"cfg\":{\"tag\":true}}");

    JsonObject child_object = doc["cfg"];

    ConfigItemBool ci;
    ci.init("tag", "Label", false);

    ASSERT_EQ(ci.get(), false);
    ci.from_json(child_object);
    ASSERT_EQ(ci.get(), true);
}

TEST(ConfigItemBool, ToJSON) {
    String out;
    ConfigItemBool ci;
    ci.init("tag", "Label", true);
    DynamicJsonDocument doc(512);
    JsonObject child_object = doc.createNestedObject("cfg");
    ci.to_json(child_object);
    serializeJson(doc, out);
    ASSERT_STREQ(out.c_str(), "{\"cfg\":{\"tag\":true}}");
    out = "";
    ci.set(false);
    ci.to_json(child_object);
    serializeJson(doc, out);
    ASSERT_STREQ(out.c_str(), "{\"cfg\":{\"tag\":false}}");
}

TEST(ConfigItemInt, Base) {
    ConfigItemInt ci;
    ci.init("int", "label", 123);
    ASSERT_EQ(ci.get(), 123);
    ci.set(-10);
    ASSERT_EQ(ci.get(), -10);
}

TEST(ConfigItemInt, Base_No_Label) {
    ConfigItemInt ci;
    ci.init("int", 123);
    ASSERT_EQ(ci.get(), 123);
    ci.set(-10);
    ASSERT_EQ(ci.get(), -10);
}

TEST(ConfigItemInt, FromJSON) {
    DynamicJsonDocument doc(512);

    deserializeJson(doc, "{\"cfg\":{\"int\":56789}}");

    JsonObject child_object = doc["cfg"];

    ConfigItemInt ci;
    ci.init("int", "label", 12345);

    ASSERT_EQ(ci.get(), 12345);
    ci.from_json(child_object);
    ASSERT_EQ(ci.get(), 56789);
}

TEST(ConfigItemInt, FromJSON_negative) {
    DynamicJsonDocument doc(512);
    deserializeJson(doc, "{\"cfg\":{\"int\":-56789}}");

    JsonObject child_object = doc["cfg"];

    ConfigItemInt ci;
    ci.init("int", "label", 12345);

    ASSERT_EQ(ci.get(), 12345);
    ci.from_json(child_object);
    ASSERT_EQ(ci.get(), -56789);
}

TEST(ConfigItemInt, ToJSON) {
    String out;
    DynamicJsonDocument doc(512);
    JsonObject child_object = doc.createNestedObject("cfg");

    ConfigItemInt ci;
    ci.init("int", "label", 12345);
    ci.to_json(child_object);
    serializeJson(doc, out);
    ASSERT_STREQ(out.c_str(), "{\"cfg\":{\"int\":12345}}");
    out = "";
    ci.set(-12347);
    child_object = doc.createNestedObject("cfg");
    ci.to_json(child_object);
    serializeJson(doc, out);
    ASSERT_STREQ(out.c_str(), "{\"cfg\":{\"int\":-12347}}");
}

TEST(ConfigItemFloat, Base) {
    ConfigItemFloat ci;
    ci.init("float", "label", 123.02035);
    EXPECT_FLOAT_EQ(ci.get(), 123.02035);
    ci.set(-10.568);
    EXPECT_FLOAT_EQ(ci.get(), -10.568);
}

TEST(ConfigItemFloat, Base_No_Label) {
    ConfigItemFloat ci;
    ci.init("float", 123.02035);
    EXPECT_FLOAT_EQ(ci.get(), 123.02035);
    ci.set(-10.568);
    EXPECT_FLOAT_EQ(ci.get(), -10.568);
}

TEST(ConfigItemFloat, FromJSON) {
    DynamicJsonDocument doc(512);

    deserializeJson(doc, "{\"cfg\":{\"float\":56.789}}");

    JsonObject child_object = doc["cfg"];

    ConfigItemFloat ci;
    ci.init("float", "label", 12345.456);

    EXPECT_FLOAT_EQ(ci.get(), 12345.456);
    ci.from_json(child_object);
    EXPECT_FLOAT_EQ(ci.get(), 56.789001);
}

TEST(ConfigItemFloat, FromJSON_negative) {
    DynamicJsonDocument doc(512);
    deserializeJson(doc, "{\"cfg\":{\"float\":-56.789}}");

    JsonObject child_object = doc["cfg"];

    ConfigItemFloat ci;
    ci.init("float", "label", 123.45);

    EXPECT_FLOAT_EQ(ci.get(), 123.45);
    ci.from_json(child_object);
    EXPECT_FLOAT_EQ(ci.get(), -56.789);
}

TEST(ConfigItemFloat, ToJSON) {
    String out;
    DynamicJsonDocument doc(512);
    JsonObject child_object = doc.createNestedObject("cfg");

    ConfigItemFloat ci;
    ci.init("float", "label", 12.3456789);
    ci.to_json(child_object);
    serializeJson(doc, out);
    ASSERT_STREQ(out.c_str(), "{\"cfg\":{\"float\":12.34567928}}");
    out = "";
    ci.set(-98.765432);
    child_object = doc.createNestedObject("cfg");
    ci.to_json(child_object);
    serializeJson(doc, out);
    ASSERT_STREQ(out.c_str(), "{\"cfg\":{\"float\":-98.76543427}}");
}

TEST(ConfigItemString, Base) {
    ConfigItemString ci;
    ci.init("str", "label", "default Value");
    ASSERT_STREQ(ci.get(), "default Value");
    ci.set("string");
    ASSERT_STREQ(ci.get(), "string");
}
TEST(ConfigItemString, Base_No_Label) {
    ConfigItemString ci;
    ci.init("str", "default Value");
    ASSERT_STREQ(ci.get(), "default Value");
    ci.set("string");
    ASSERT_STREQ(ci.get(), "string");
}
TEST(ConfigItemString, FromJSON) {
    DynamicJsonDocument doc(512);
    deserializeJson(doc, "{\"cfg\":{\"str\":\"New String\"}}");
    JsonObject child_object = doc["cfg"];

    ConfigItemString ci;
    ci.init("str", "label", "default Value");

    ASSERT_STREQ(ci.get(), "default Value");
    ci.from_json(child_object);
    ASSERT_STREQ(ci.get(), "New String");
}

TEST(ConfigItemString, ToJSON) {
    String out;
    DynamicJsonDocument doc(512);
    JsonObject child_object = doc.createNestedObject("cfg");

    ConfigItemString ci;
    ci.init("str", "label", "Test String");

    ci.to_json(child_object);
    serializeJson(doc, out);
    ASSERT_STREQ(out.c_str(), "{\"cfg\":{\"str\":\"Test String\"}}");

    out = "";
    child_object = doc.createNestedObject("cfg");

    ci.set("New String");

    ci.to_json(child_object);
    serializeJson(doc, out);
    ASSERT_STREQ(out.c_str(), "{\"cfg\":{\"str\":\"New String\"}}");
}

TEST(ConfigItemIP, Base) {
    ConfigItemIP ci;
    ci.init("tag", "label", IPAddress(192, 168, 12, 34));
    ASSERT_EQ(ci.get(), IPAddress(192, 168, 12, 34));
    ci.set(IPAddress(192, 168, 34, 21));
    ASSERT_EQ(ci.get(), IPAddress(192, 168, 34, 21));
}
TEST(ConfigItemIP, Base_No_Label) {
    ConfigItemIP ci;
    ci.init("tag", IPAddress(192, 168, 12, 34));
    ASSERT_EQ(ci.get(), IPAddress(192, 168, 12, 34));
    ci.set(IPAddress(192, 168, 34, 21));
    ASSERT_EQ(ci.get(), IPAddress(192, 168, 34, 21));
}
TEST(ConfigItemIP, FromJSON) {
    DynamicJsonDocument doc(512);
    deserializeJson(doc, "{\"cfg\":{\"ip\":[127,0,15,46]}}");
    JsonObject child_object = doc["cfg"];

    ConfigItemIP ci;
    ci.init("ip", "label", IPAddress(192, 168, 12, 34));

    ASSERT_EQ(ci.get(), IPAddress(192, 168, 12, 34));
    ci.from_json(child_object);
    ASSERT_EQ(ci.get(), IPAddress(127, 0, 15, 46));
}

TEST(ConfigItemIP, ToJSON) {
    String out;
    DynamicJsonDocument doc(512);
    JsonObject child_object = doc.createNestedObject("cfg");

    ConfigItemIP ci;
    ci.init("tag", "label", IPAddress(192, 168, 12, 34));

    ci.to_json(child_object);
    serializeJson(doc, out);
    ASSERT_STREQ(out.c_str(), "{\"cfg\":{\"tag\":[192,168,12,34]}}");

    out = "";
    child_object = doc.createNestedObject("cfg");

    ci.set(IPAddress(18, 45, 62, 54));
    ci.to_json(child_object);
    serializeJson(doc, out);
    ASSERT_STREQ(out.c_str(), "{\"cfg\":{\"tag\":[18,45,62,54]}}");
}

TEST(ESPConfigManager, Create) {
    ESPConfigManager ec;
    EXPECT_EQ(ec._base_group, nullptr);
}
TEST(ConfigItem, Create) {
    ConfigItem ci;
    ci.setup("tag_item", "Label_Item");
    EXPECT_STREQ(ci._id, "tag_item");
    EXPECT_STREQ(ci._label, "Label_Item");
    EXPECT_EQ(ci._sibling, nullptr);
}

TEST(ConfigGroup, Create) {
    ConfigGroup cg;
    cg.setup("tag_grp", "Label_Group");
    EXPECT_STREQ(cg._id, "tag_grp");
    EXPECT_STREQ(cg._label, "Label_Group");
    EXPECT_EQ(cg._sibling, nullptr);
    EXPECT_EQ(cg._child, nullptr);
}

TEST(ESPConfigManager, add_group) {
    ESPConfigManager ec;
    EXPECT_EQ(ec._base_group, nullptr);
    ConfigGroup cg;
    cg.setup("tag_grp", "Label_Group");
    ec.add_group(&cg);
    EXPECT_EQ(ec._base_group, &cg);
}

TEST(ESPConfigManager, add_2_groups) {
    ESPConfigManager ec;
    EXPECT_EQ(ec._base_group, nullptr);
    ConfigGroup cg;
    cg.setup("tag_grp", "Label_Group");
    ConfigGroup cg2;
    cg2.setup("tag_grp2", "Label_Group2");
    ec.add_group(&cg);
    ec.add_group(&cg2);
    EXPECT_EQ(ec._base_group, &cg);
    EXPECT_EQ(cg._sibling, &cg2);
}

TEST(ConfigGroup, add_item) {
    ConfigGroup cg;
    cg.setup("tag_grp", "Label_Group");
    ConfigItemBool cb;
    cb.init("tag", "label", true);
    cg.add_child(&cb);
    EXPECT_EQ(cg._child, &cb);
    EXPECT_EQ(cb._sibling, nullptr);
}

TEST(ConfigGroup, add_2_items) {
    ConfigGroup cg;
    cg.setup("tag_grp", "Label_Group");
    ConfigItemBool cb;
    cb.init("tag", "label", true);
    ConfigItemBool cb2;
    cb2.init("tag", "label", true);
    cg.add_child(&cb);
    cg.add_child(&cb2);
    EXPECT_EQ(cg._child, &cb);
    EXPECT_EQ(cb._sibling, &cb2);
}

TEST(ConfigGroup, to_json) {
    String out;
    DynamicJsonDocument doc(512);
    JsonObject child_object = doc.createNestedObject("cfg");

    ConfigGroup cg;
    cg.setup("tag_grp", "Label_Group");
    cg.to_json(child_object);
    serializeJson(doc, out);
    ASSERT_STREQ(out.c_str(), "{\"cfg\":{}}");
}

TEST(ConfigGroup, to_json_with_child) {
    String out;
    DynamicJsonDocument doc(512);
    JsonObject child_object = doc.createNestedObject("cfg");

    ConfigGroup cg;
    cg.setup("tag_grp", "Label_Group");

    cg.to_json(child_object);
    serializeJson(doc, out);
    ASSERT_STREQ(out.c_str(), "{\"cfg\":{}}");

    out = "";
    ConfigItemBool ci;
    ci.init("tag", "Label", true);

    cg.add_child(&ci);
    cg.to_json(child_object);
    serializeJson(doc, out);
    ASSERT_STREQ(out.c_str(), "{\"cfg\":{\"tag_grp\":{\"tag\":true}}}");
}

TEST(ConfigGroup, from_json_with_child) {
    DynamicJsonDocument doc(512);
    deserializeJson(doc, "{\"cfg\":{\"tag_grp\":{\"tag\":true}}}");
    JsonObject child_object = doc["cfg"];

    ConfigGroup cg;
    cg.setup("tag_grp", "Label_Group");

    ConfigItemBool ci;
    ci.init("tag", "Label", false);

    cg.add_child(&ci);

    ASSERT_EQ(ci.get(), false);
    cg.from_json(child_object);
    ASSERT_EQ(ci.get(), true);
}

TEST(ConfigGroup, to_json_with_2_children) {
    String out;
    DynamicJsonDocument doc(512);
    JsonObject child_object = doc.createNestedObject("cfg");

    ConfigGroup cg;
    cg.setup("tag_grp", "Label_Group");

    cg.to_json(child_object);
    serializeJson(doc, out);
    ASSERT_STREQ(out.c_str(), "{\"cfg\":{}}");

    out = "";
    ConfigItemBool ci;
    ci.init("bool", "Label", true);
    ConfigItemInt cint;
    cint.init("int", "Label", 12345);

    cg.add_child(&ci);
    cg.add_child(&cint);
    cg.to_json(child_object);
    serializeJson(doc, out);
    ASSERT_STREQ(out.c_str(), "{\"cfg\":{\"tag_grp\":{\"bool\":true,\"int\":12345}}}");
}

TEST(ConfigGroup, from_json_with_2_children) {
    DynamicJsonDocument doc(512);
    deserializeJson(doc, "{\"cfg\":{\"tag_grp\":{\"bool\":false, \"int\":12345}}}");
    JsonObject child_object = doc["cfg"];

    ConfigGroup cg;
    cg.setup("tag_grp", "Label_Group");

    ConfigItemBool ci;
    ci.init("bool", "Label", true);
    ConfigItemInt cint;
    cint.init("int", "Label", -98765);

    cg.add_child(&ci);
    cg.add_child(&cint);
    ASSERT_EQ(ci.get(), true);
    ASSERT_EQ(cint.get(), -98765);
    cg.from_json(child_object);
    ASSERT_EQ(ci.get(), false);
    ASSERT_EQ(cint.get(), 12345);
}

TEST(ESPConfigManager, set_wifissid) {
    ESPConfigManager ec;
    ec.set_wifissid("wifissid");
    ASSERT_STREQ(ec.get_ssid(), "wifissid");
}

TEST(ESPConfigManager, set_wifikey) {
    ESPConfigManager ec;
    ec.set_wifikey("wifikey");
    ASSERT_STREQ(ec.get_key(), "wifikey");
}

TEST(ESPConfigManager, set_wifi) {
    ESPConfigManager ec;
    ec.set_wifi("wifissid", "wifikey");
    ASSERT_STREQ(ec.get_ssid(), "wifissid");
    ASSERT_STREQ(ec.get_key(), "wifikey");
}

TEST(ESPConfigManager, save_to_json) {
    String out;
    DynamicJsonDocument doc(512);
    ESPConfigManager ec;
    ec.set_wifi("wifissid", "wifikey");
    ec.save_to_json(doc);
    serializeJson(doc, out);
    ASSERT_STREQ(out.c_str(), "{\"wifi\":{\"ssid\":\"wifissid\",\"key\":\"wifikey\"}}");
}

TEST(ESPConfigManager, save_wifi_to_json) {
    String out;
    DynamicJsonDocument doc(512);
    ESPConfigManager ec;
    ec.set_wifi("wifissid", "wifikey");
    ec.save_wifi_json(doc);
    serializeJson(doc, out);
    ASSERT_STREQ(out.c_str(), "{\"wifi\":{\"ssid\":\"wifissid\",\"key\":\"wifikey\"}}");
}

TEST(ESPConfigManager, save_to_json_empty_wifi) {
    String out;
    DynamicJsonDocument doc(512);
    ESPConfigManager ec;
    ec.save_wifi_json(doc);
    serializeJson(doc, out);
    ASSERT_STREQ(out.c_str(), "null");
}

TEST(ESPConfigManager, save_to_json_with_empty_base_group) {
    String out;
    DynamicJsonDocument doc(512);
    ESPConfigManager ec;
    ConfigGroup cg;
    cg.setup("nw", "Network");
    ec.set_wifi("wifissid", "wifikey");
    ec.save_to_json(doc);
    serializeJson(doc, out);
    ASSERT_STREQ(out.c_str(), "{\"wifi\":{\"ssid\":\"wifissid\",\"key\":\"wifikey\"}}");
}

TEST(ESPConfigManager, save_to_json_with_base_group_with_2_items) {
    String out;
    DynamicJsonDocument doc(512);
    ESPConfigManager ec;
    ec.set_wifi("wifissid", "wifikey");

    ConfigGroup cg;
    cg.setup("nw", "Network");

    ConfigItemInt cint;
    cint.init("int", "Label", 12345);

    ConfigItemString cstr;
    cstr.init("str", "Label", "text");
    ec.add_group(&cg);
    cg.add_child(&cint);
    cg.add_child(&cstr);
    ec.save_to_json(doc);
    serializeJson(doc, out);
    ASSERT_STREQ(out.c_str(), "{\"wifi\":{\"ssid\":\"wifissid\",\"key\":\"wifikey\"},\"cfg\":{\"nw\":{\"int\":12345,\"str\":\"text\"}}}");
}

TEST(ESPConfigManager, save_to_json_with_2_base_groups) {
    String out;
    DynamicJsonDocument doc(1024);
    ESPConfigManager ec;
    ec.set_wifi("wifissid", "wifikey");

    ConfigGroup cg;
    cg.setup("nw");

    ConfigGroup cg_data;
    cg_data.setup("data");

    ConfigItemInt cint;
    cint.init("int", "Label", 12345);

    ConfigItemString cstr;
    cstr.init("str", "Label", "text");

    cg.add_child(&cint);
    cg_data.add_child(&cstr);
    ec.add_group(&cg);
    ec.add_group(&cg_data);
    ec.save_to_json(doc);
    serializeJson(doc, out);
    ASSERT_STREQ(out.c_str(), "{\"wifi\":{\"ssid\":\"wifissid\",\"key\":\"wifikey\"},\"cfg\":{\"nw\":{\"int\":12345},\"data\":{\"str\":\"text\"}}}");
}

TEST(ESPConfigManager, save_to_json_with_2_base_groups_no_wifi) {
    String out;
    DynamicJsonDocument doc(1024);
    ESPConfigManager ec;
    // ec.set_wifi("wifissid", "wifikey");

    ConfigGroup cg;
    cg.setup("nw");

    ConfigGroup cg_data;
    cg_data.setup("data");

    ConfigItemInt cint;
    cint.init("int", "Label", 12345);

    ConfigItemString cstr;
    cstr.init("str", "Label", "text");

    cg.add_child(&cint);
    cg_data.add_child(&cstr);
    ec.add_group(&cg);
    ec.add_group(&cg_data);
    ec.save_to_json(doc);
    serializeJson(doc, out);
    ASSERT_STREQ(out.c_str(), "{\"cfg\":{\"nw\":{\"int\":12345},\"data\":{\"str\":\"text\"}}}");
}

TEST(ESPConfigManager, save_and_load_config) {
    if (!LittleFS.begin()) {
        // LittleFS.format();
        Serial.println("An Error has occurred while mounting LittleFS");
        return;
    }

    LittleFS.remove("/esp_cfg.json");
    ESPConfigManager *esp_config = new ESPConfigManager();
    ConfigGroup *config_group_nw = new ConfigGroup();
    ConfigGroup *config_group_data = new ConfigGroup();
    ConfigItemIP *cip = new ConfigItemIP();
    ConfigItemInt *cint = new ConfigItemInt();
    ConfigItemString *cstr = new ConfigItemString();

    esp_config->set_wifi("wifissid", "wifikey");

    config_group_nw->setup("nw");
    config_group_data->setup("data");

    cip->init("ipaddress", IPAddress(192, 168, 10, 196));
    cint->init("int", 12345);   // Default value 12345
    cstr->init("str", "text");  // Default value "text"

    esp_config->add_group(config_group_nw);
    esp_config->add_group(config_group_data);

    config_group_nw->add_child(cint);
    config_group_nw->add_child(cstr);

    cint->set(108);         // Overwrite default value
    cstr->set("New Text");  // Overwrite default value
    esp_config->save();     // save

    // delete esp_config;
    // delete config_group_nw;
    // delete cint;
    // delete cstr;

    esp_config = new ESPConfigManager();
    config_group_nw = new ConfigGroup();
    config_group_data = new ConfigGroup();
    cint = new ConfigItemInt();
    cstr = new ConfigItemString();

    config_group_nw->setup("nw");
    cint->init("int", 12345);   // Default value 12345
    cstr->init("str", "text");  // Default value "text"

    esp_config->add_group(config_group_nw);
    config_group_nw->add_child(cint);
    config_group_nw->add_child(cstr);

    // ASSERT_STREQ(esp_config->get_ssid(), NULL);  // default value
    // ASSERT_STREQ(esp_config->get_key(), NULL);   // default value

    ASSERT_EQ(cint->get(), 12345);      // default value
    ASSERT_STREQ(cstr->get(), "text");  // default value
    esp_config->load();
    ASSERT_EQ(cint->get(), 108);            // new value
    ASSERT_STREQ(cstr->get(), "New Text");  // new value
}

}  // namespace

#if defined(ARDUINO)
#include <Arduino.h>

void setup() {
    // should be the same value as for the `test_speed` option in "platformio.ini"
    // default value is test_speed=115200
    Serial.begin(115200);

    ::testing::InitGoogleTest();
    // if you plan to use GMock, replace the line above with
    // ::testing::InitGoogleMock();
}

void loop() {
    // Run tests
    if (RUN_ALL_TESTS())
        ;

    // sleep for 1 sec
    delay(60000);
}

#else
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    // if you plan to use GMock, replace the line above with
    // ::testing::InitGoogleMock(&argc, argv);

    if (RUN_ALL_TESTS())
        ;

    // Always return zero-code and allow PlatformIO to parse results
    return 0;
}
#endif