#include <gtest/gtest.h>

#include "arduinojson.h"
#include "esp32config.h"

namespace {
TEST(ConfigItemBool, Base) {
    ConfigItemBool ci;
    ci.init("tag", "label", true);
    ASSERT_EQ(ci.get(), true);
    ci.set(false);
    ASSERT_EQ(ci.get(), false);
}

// TEST(ConfigItemBool, FromJSON) {
//     ConfigItemBool ci("tag", false);
//     StaticJsonDocument<256> doc;
//     deserializeJson(doc, "{\"tag\":true}");
//     ASSERT_EQ(ci.get(), false);
//     ci.from_json(doc);
//     ASSERT_EQ(ci.get(), true);
// }

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

// TEST(ConfigItemInt, FromJSON) {
//     ConfigItemInt ci("tag", 123456);
//     StaticJsonDocument<256> doc;
//     deserializeJson(doc, "{\"tag\":108108}");
//     ASSERT_EQ(ci.get(), 123456);
//     ci.from_json(doc);
//     ASSERT_EQ(ci.get(), 108108);
// }
// TEST(ConfigItemInt, FromJSON_negative) {
//     ConfigItemInt ci("tag", 123456);
//     StaticJsonDocument<256> doc;
//     deserializeJson(doc, "{\"tag\":-108108}");
//     ASSERT_EQ(ci.get(), 123456);
//     ci.from_json(doc);
//     ASSERT_EQ(ci.get(), -108108);
// }

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

TEST(ConfigItemString, Base) {
    ConfigItemString ci;
    ci.init("str", "label", "default Value");
    ASSERT_STREQ(ci.get(), "default Value");
    ci.set("string");
    ASSERT_STREQ(ci.get(), "string");
}

// TEST(ConfigItemString, FromJSON) {
//     ConfigItemString ci("tag", "Test String");
//     StaticJsonDocument<256> doc;
//     deserializeJson(doc, "{\"tag\":\"New String\"}");
//     ASSERT_STREQ(ci.get(), "Test String");
//     ci.from_json(doc);
//     ASSERT_STREQ(ci.get(), "New String");
// }

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

// TEST(ConfigItemIP, FromJSON) {
//     ConfigItemIP ci("tag", IPAddress(192, 168, 12, 34));
//     StaticJsonDocument<256> doc;
//     deserializeJson(doc, "{\"tag\":[127,0,15,46]}");
//     ASSERT_EQ(ci.get(), IPAddress(192, 168, 12, 34));
//     ci.from_json(doc);
//     ASSERT_EQ(ci.get(), IPAddress(127, 0, 15, 46));
// }

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

TEST(ESP32Config, Create) {
    ESP32Config ec;
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

TEST(ESP32Config, add_group) {
    ESP32Config ec;
    EXPECT_EQ(ec._base_group, nullptr);
    ConfigGroup cg;
    cg.setup("tag_grp", "Label_Group");
    ec.add_group(&cg);
    EXPECT_EQ(ec._base_group, &cg);
}

TEST(ESP32Config, add_2_groups) {
    ESP32Config ec;
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

// TEST(ConfigGroup, add_2_items) {
// }

// TEST(ConfigItem, init) {
//     ESP32Config ec;
//     String out;
//     DynamicJsonDocument doc(512);

//     ConfigItem base_item;
//     base_item.init("name");
//     ASSERT_STREQ(base_item._id, "name");
//     JsonObject child_object = doc.createNestedObject("cfg");
//     base_item.to_json(child_object);
//     serializeJson(doc, out);
//     ASSERT_STREQ(out.c_str(), "{\"cfg\":{}}");
// }

// TEST(ConfigItem, add_child) {
//     //     // ESP32Config ec;
//     String out;
//     DynamicJsonDocument doc(512);

//     ConfigItem config_group;
//     config_group.init("grp");

//     ConfigItemBool base_item;
//     base_item.init("bool");

//     EXPECT_EQ(config_group._child, nullptr);

//     config_group.add_child(&base_item);
//     EXPECT_NE(config_group._child, nullptr);
//     ASSERT_STREQ(config_group._id, "grp");
//     JsonObject child_object = doc.createNestedObject("cfg");
//     config_group.to_json(child_object);
//     serializeJson(doc, out);
//     ASSERT_STREQ(out.c_str(), "{\"cfg\":{\"grp\":{\"bool\":true}}}");
// }

// TEST(ConfigGroup, init) {
//     // ESP32Config ec;
//     String out;
//     DynamicJsonDocument doc(512);

//     ConfigGroup config_group;
//     config_group.init("name");
//     ASSERT_STREQ(config_group._id, "name");
//     // ec.init(&config_group);
//     config_group.to_json(doc);
//     serializeJson(doc, out);
//     ASSERT_STREQ(out.c_str(), "{\"name\":{}}");
// }

// TEST(ConfigGroup, Add_Item) {
//     // ESP32Config ec;
//     ConfigGroup config_group;
//     ConfigItemBool();
//     ec.init(&config_group);
//     EXPECT_EQ(ec._base_group, &config_group);
// }

// TEST(ESP32Config, Add_2_items) {
//     ESP32Config ec;
//     ConfigGroup bci1;
//     ConfigGroup bci2;
//     ec.add_item(&bci1);
//     ec.add_item(&bci2);
//     EXPECT_EQ(ec._child, &bci1);
//     EXPECT_EQ(bci1._sibling, &bci2);
//     EXPECT_EQ(bci2._sibling, nullptr);
// }

// TEST(ESP32Config, Add_All_Diferent_items) {
//     ESP32Config ec;
//     ConfigItemBool ci_bool("bool", true);
//     ConfigItemInt ci_int("int", -108);
//     ConfigItemString ci_string("string", "String Text");
//     ec.add_item(&ci_bool);
//     ec.add_item(&ci_int);
//     ec.add_item(&ci_string);
//     EXPECT_EQ(ec._child, &ci_bool);
//     EXPECT_EQ(ci_bool._sibling, &ci_int);
//     EXPECT_EQ(ci_int._sibling, &ci_string);
//     EXPECT_EQ(ci_string._sibling, nullptr);
// }

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
    delay(1000);
}

#else
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    // if you plan to use GMock, replace the line above with
    // ::testing::InitGoogleMock(&argc, argv);

    if (RUN_ALL_TESTS())
        ;

    // Always return zero-code and allow PlatformIO to parse results
    return 0;
}
#endif