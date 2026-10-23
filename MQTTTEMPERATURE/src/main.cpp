#include <Arduino.h>
#include <math.h>
#include <WiFi.h>
#include <HomeAssistantMQTT.h>
#include <PubSubClient.h>

const char* ssid = "moerings_IoT";
const char* wifipassword = "1nth3a1rt0n19ht";

const char* serverip = "192.168.68.85";
const int serverport = 1883;
const char* username = "esp32-2";
const char* password = "esp322password";

int Beta = 6500;
int Rvalue = 10000;



 //Function for reading temperature

float readtemp(int Pin, float seriesresistance, float beta){   

int val = analogRead(Pin);   //read adc pin value

float resistance = seriesresistance * (4095.0 / val - 1.0);  //get resistance

//steinhart-hart math
float temp = resistance / seriesresistance;   
temp = log(temp);
temp /= beta; 
temp += 1.0 /  (25 + 273.15);
temp = 1.0 / temp;
temp -= 273.15;
return temp;
}



void HAMQTT_Callback(String item, String payload) {

}


WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

HomeAssistantMQTTDevice sensor1("sensor", "Temp 1", "temperature1");
HomeAssistantMQTTDevice sensor2("sensor", "Temp 2", "temperature2");
HomeAssistantMQTTDevice sensor3("sensor", "Temp 3", "temperature3");
HomeAssistantMQTTDevice sensor4("sensor", "Temp 4", "temperature4");
HomeAssistantMQTTDevice sensor5("sensor", "Temp 5", "temperature5");
HomeAssistantMQTTDevice sensor6("sensor", "Temp 6", "temperature6");


void configureSensors() {
    sensor1.setConfig("device_class", "temperature");
    sensor1.setConfig("unit_of_measurement", "°C");
    sensor1.setConfig("state_class", "measurement");

    sensor2.setConfig("device_class", "temperature");
    sensor2.setConfig("unit_of_measurement", "°C");
    sensor2.setConfig("state_class", "measurement");

    sensor3.setConfig("device_class", "temperature");
    sensor3.setConfig("unit_of_measurement", "°C");
    sensor3.setConfig("state_class", "measurement");

    sensor4.setConfig("device_class", "temperature");
    sensor4.setConfig("unit_of_measurement", "°C");
    sensor4.setConfig("state_class", "measurement");

    sensor5.setConfig("device_class", "temperature");
    sensor5.setConfig("unit_of_measurement", "°C");
    sensor5.setConfig("state_class", "measurement");

    sensor6.setConfig("device_class", "temperature");
    sensor6.setConfig("unit_of_measurement", "°C");
    sensor6.setConfig("state_class", "measurement");
}

void reconnectMQTT() {
    while (!mqttClient.connected()) {
        Serial.print("Connecting to MQTT... ");

        String clientId = "ESP32-" +
                          String((uint32_t)ESP.getEfuseMac(), HEX);

        if (mqttClient.connect(clientId.c_str(), username, password)) {
            Serial.println("connected");
        } else {
            Serial.print("failed, state = ");
            Serial.println(mqttClient.state());
            delay(5000);
        }
    }
}


void setup() {
Serial.begin(115200);  //Start serial
WiFi.begin(ssid, wifipassword);
while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}

if (mqttClient.setBufferSize(1024)) {
        Serial.println("MQTT buffer set to 1024 bytes");
    } else {
        Serial.println("Failed to resize MQTT buffer");
    }

mqttClient.setServer("192.168.68.85", 1883);
configureSensors();
}


void loop() {

if (!mqttClient.connected()){
    reconnectMQTT();
}

mqttClient.loop();


//    Name           Function Pin Resistor Beta
float temperature1 = readtemp (33, Rvalue, Beta);
float temperature2 = readtemp (32, Rvalue, Beta);
float temperature3 = readtemp (35, Rvalue, Beta);
float temperature4 = readtemp (34, Rvalue, Beta);
float temperature5 = readtemp (36, Rvalue, Beta);
float temperature6 = readtemp (39, Rvalue, Beta);

sensor1.setState(String(temperature1, 2));
sensor2.setState(String(temperature2, 2));
sensor3.setState(String(temperature3, 2));
sensor4.setState(String(temperature4, 2));
sensor5.setState(String(temperature5, 2));
sensor6.setState(String(temperature6, 2));

sensor1.loop(mqttClient);
sensor2.loop(mqttClient);
sensor3.loop(mqttClient);
sensor4.loop(mqttClient);
sensor5.loop(mqttClient);
sensor6.loop(mqttClient);

delay(3000);
}

void publishmqttconfig() {

}
//(192.168.68.85, 1883, "ESP32", "esp32-2", "esp322password");




