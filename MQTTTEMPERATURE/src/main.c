#include <WiFi.h>
#include <math.h>
#include <HomeAssistantMQTT.h>
#include <Arduino.h>

const char* ssid = "Your_SSID";
const char* password = "Your_PASSWORD";


void setup() {
serialbegin(115200);

wifibegin (ssid, password);

while (wifi)
{
    /* code */
}

}


void loop
() {
    // Your loop code here
}