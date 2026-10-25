#include <Arduino.h>
#include <math.h>
#include <WiFi.h>
#include <HomeAssistantMQTT.h>
#include <PubSubClient.h>

#include "temperature.h"
#include "mqtt.h"

//Wifi name and password
const char* ssid = "yourwifissid";
const char* wifipassword = "yourwifipassword";

//homeassistant server ip, port and username and password are all found in mqtt.cpp
//temperature math function is found in temperature.cpp
//when a adc measures outside of range it will return -404 eg, when no sensor is connected
//betas can be remotely configured in homeassistant by using a helper and automation script which you can find in homeassistantscripts.yaml

void setup() {
Serial.begin(115200);  //Start serial

//Wifi connection
WiFi.begin(ssid, wifipassword);
while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}
setupMQTT();
}

void loop() {

    //mqtt connection
if (!mqttClient.connected()){
    reconnectMQTT();
}
mqttClient.loop();

//configure sensor pins, resistances and betas (betas are found in mqtt.cpp)
//    Name           Function Pin Resistor Beta
float temperature1 = readtemp (33, 10000, rbetas[0]); //sensors currently use the 10k beta variable 
float temperature2 = readtemp (32, 10000, rbetas[0]); //if you dont want to use the remote betas from homeassistant
float temperature3 = readtemp (35, 10000, rbetas[0]); //just switch "rbetas[0}" for a different value eg, 3950
float temperature4 = readtemp (34, 10000, rbetas[0]);
float temperature5 = readtemp (36, 10000, rbetas[0]);
float temperature6 = readtemp (39, 10000, rbetas[0]);

//array for using the variables in for loops
float temperatures [6] = {
temperature1, temperature2, temperature3,
temperature4, temperature5, temperature6
};

// set sensor values
for (int i = 0; i < 6; i++)
{
sensors[i]->setState(String(temperatures[i], 2));
delay(1);
}

// loop sensors
for (int i = 0; i < 6; i++)
{
sensors[i]->loop(mqttClient);
delay(1);
}

//try to reconnect wifi when connection is lost
if (WiFi.status() != WL_CONNECTED)
{
WiFi.reconnect();
Serial.print("Wifi reconnecting");
}

delay(3000);  // do loop every 3 seconds
}
