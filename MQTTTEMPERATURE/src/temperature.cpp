
#include <Arduino.h>
#include <math.h>
#include "temperature.h"


float readtemp(int Pin, float seriesresistance, float beta){   

int val = analogRead(Pin);   //read adc pin value

if (val <=0 || val >= 4095) {
return -404;
}

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