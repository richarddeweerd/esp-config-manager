#include <unity.h>
#include "test_config_items.hpp"
#include "test_config_manager.hpp"

void setup()
{
    Serial.begin(115200);
    delay(1000);

    UNITY_BEGIN();
    RUN_TEST(test_config_item_bool_base_no_label);
    RUN_TEST(test_config_item_bool_base_fromJSON);
    RUN_TEST(test_config_item_bool_base_toJSON);
    RUN_TEST(test_config_item_int_base);
    RUN_TEST(test_config_item_int_base_fromJSON);
    RUN_TEST(test_config_item_int_base_toJSON);
    RUN_TEST(test_config_item_float_base);
    RUN_TEST(test_config_item_float_base_fromJSON);
    RUN_TEST(test_config_item_float_base_toJSON);
    RUN_TEST(test_config_item_string_base);
    RUN_TEST(test_config_item_string_base_fromJSON);
    RUN_TEST(test_config_item_string_base_toJSON);
    RUN_TEST(test_config_item_IP_base);
    RUN_TEST(test_config_item_IP_base_fromJSON);
    RUN_TEST(test_config_item_IP_base_toJSON);

    RUN_TEST(test_add_item);
    RUN_TEST(test_add_item_sibling);
    RUN_TEST(test_save_to_json);
    RUN_TEST(test_load_from_json);
    RUN_TEST(test_save_to_fs);
    RUN_TEST(test_load_from_fs);

    UNITY_END();
}

void loop() {}