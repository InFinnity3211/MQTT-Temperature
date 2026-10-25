
#ifndef MQTT_H
#define MQTT_H

#include <Arduino.h>
#include <PubSubClient.h>
#include <HomeAssistantMQTT.h>

extern float rbetas[2];

extern PubSubClient mqttClient;

extern HomeAssistantMQTTDevice sensor1;
extern HomeAssistantMQTTDevice sensor2;
extern HomeAssistantMQTTDevice sensor3;
extern HomeAssistantMQTTDevice sensor4;
extern HomeAssistantMQTTDevice sensor5;
extern HomeAssistantMQTTDevice sensor6;

extern HomeAssistantMQTTDevice* sensors[6];

void configureSensors();
void reconnectMQTT();
void mqttCallback(char* topic, byte* payload, unsigned int length);
void setupMQTT();
#endif
