#include <Arduino.h>

#include "LittleFS.h"
#include "esp-config-manager.h"

ESPConfigManager config_manager;
ConfigItemBool ci_bool;
ConfigItemInt ci_int;
ConfigItemFloat ci_float;
ConfigItemString ci_string;
ConfigItemIP ci_ip;

void print_config_to_serial()
{
    File file = LittleFS.open("/esp_cfg.json", "r");
    if (!file)
    {
        Serial.println("Failed to open file for reading");
        return;
    }

    Serial.println("File Contents:");
    while (file.available())
    {
        Serial.write(file.read());
    }
    file.close();
    Serial.println();
}

void print_values()
{
    Serial.print("Value ci_bool: ");
    Serial.println(ci_bool.get());
    Serial.print("Value ci_int: ");
    Serial.println(ci_int.get());
    Serial.print("Value ci_float: ");
    Serial.println(ci_float.get());
    Serial.print("Value ci_string: ");
    Serial.println(ci_string.get());
    Serial.print("Value ci_op: ");
    Serial.println(ci_ip.get());
}

void setup()
{
    Serial.begin(115200);
    Serial.println();
    Serial.println("Booting...");

    if (!LittleFS.begin())
    {
        // LittleFS.format();
        Serial.println("An Error has occurred while mounting LittleFS");
        return;
    }


    ci_bool.init("bool", true);
    ci_int.init("int", -1256);
    ci_float.init("float", -15.486);
    ci_string.init("string", "Text string");
    ci_ip.init("ip_address", IPAddress(192, 168, 208, 109));

    config_manager.add_item(&ci_bool);
    config_manager.add_item(&ci_int);
    config_manager.add_item(&ci_float);
    config_manager.add_item(&ci_string);
    config_manager.add_item(&ci_ip);

    config_manager.save();
    Serial.println("Default values:");
    print_values();
    print_config_to_serial();
    // set different values
    Serial.println("------------------------------------");
    Serial.println("Setting new values:");
    ci_bool.set(false);
    ci_int.set(9564);
    ci_float.set(3.1415);
    ci_string.set("New Text string");
    ci_ip.set(IPAddress(127, 0, 0, 1));
    print_values();

    Serial.println("Save new values:");
    config_manager.save();
    print_config_to_serial();
    Serial.println("------------------------------------");
    Serial.println("Setting new values again:");
    ci_bool.set(false);
    ci_int.set(56335);
    ci_float.set(9.56415);
    ci_string.set("New Text string again!");
    ci_ip.set(IPAddress(123, 45, 67, 89));
    print_values();
    Serial.println("------------------------------------");
    Serial.println("Clear settings:");
    Serial.println("Reboot to set settings back to default");
    Serial.println("------------------------------------");
    config_manager.clear();
    print_config_to_serial();
}

void loop()
{
    delay(1000);
}