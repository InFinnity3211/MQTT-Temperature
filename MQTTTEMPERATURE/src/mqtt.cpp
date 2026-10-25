#include <Arduino.h>
#include <math.h>
#include <WiFi.h>
#include <HomeAssistantMQTT.h>
#include <PubSubClient.h>
#include <cstring>
#include <cstdlib>

#include "mqtt.h"

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

//mqtt user password and username
const char* username = "yourusername";
const char* password = "yourpassword";

//server ip and password
char* serverip = "yourserverip";  //also add it at the bottom of this code at mqttClient.setServer
int   serverport = 1883;

float Beta      = 3950.0;
float Beta100k  = 3950.0;
//float betaname = defaultvalue;

//rename "temp 1" if you have multiple esp's running to not add different values to 1 entity
HomeAssistantMQTTDevice sensor1("sensor", "Temp 1", "temperature-1");
HomeAssistantMQTTDevice sensor2("sensor", "Temp 2", "temperature-2");
HomeAssistantMQTTDevice sensor3("sensor", "Temp 3", "temperature-3");
HomeAssistantMQTTDevice sensor4("sensor", "Temp 4", "temperature-4");
HomeAssistantMQTTDevice sensor5("sensor", "Temp 5", "temperature-5");
HomeAssistantMQTTDevice sensor6("sensor", "Temp 6", "temperatur2-6");

HomeAssistantMQTTDevice* sensors[6] = {
&sensor1, &sensor2, &sensor3, 
&sensor4, &sensor5, &sensor6
};
//beta variables for remote beta changing also declare them above
float rbetas[2] = {
Beta, Beta100k
};

const char* topics[2] = { //mqtt helper topics
"esp32/beta10k", "esp32/beta100k"
};


//Sensor configuration
void configureSensors() {
    
for (int i = 0; i < 6; i++)
{
sensors[i]->setConfig("state_class", "measurement");
sensors[i]->setConfig("device_class", "temperature");
sensors[i]->setConfig("unit_of_measurement", "°C");
delay(1);
}
}
    

//Mqtt connection
void reconnectMQTT() {
    while (!mqttClient.connected()) {
        Serial.print("Connecting to MQTT... ");

        String clientId = "ESP32-" +
                          String((uint32_t)ESP.getEfuseMac(), HEX);

        if (mqttClient.connect(clientId.c_str(), username, password)) {
            Serial.println("connected");
            for (int i = 0; i < 2; i++) {
            mqttClient.subscribe(topics[i]);  
            }
        } else {
            Serial.print("failed, state = ");
            Serial.println(mqttClient.state());
            delay(5000);
        }
    }

}


//mqtt remote beta changing 
void mqttCallback(char* topic, byte* payload, unsigned int length) {
  for (int i = 0; i < 2; i++)
  {
    if (strcmp(topic, topics[i]) == 0) {
        char message[20];

        memcpy(message, payload, length);
        message[length] = '\0';

        rbetas[i] = atof(message);
        Serial.print(rbetas[i]);
    }
  }
}

void setupMQTT() {
// Increase buffer size for larger packets
if (mqttClient.setBufferSize(1024)) {
        Serial.println("MQTT buffer set to 1024 bytes");
    } else {
        Serial.println("Failed to resize MQTT buffer");
    }

    mqttClient.setServer("yourserverip", 1883);
    mqttClient.setCallback(mqttCallback);
configureSensors();
}