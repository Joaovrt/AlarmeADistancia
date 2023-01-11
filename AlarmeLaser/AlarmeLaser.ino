#include <analogWrite.h>
#include "UbidotsEsp32Mqtt.h"
#define pinoSensorLuz  32
#define pinoBuzzer  13                                    
#define UBIDOTS_TOKEN  ""  
#define WIFI_SSID  ""      
#define WIFI_PASS  ""    
#define DEVICE_LABEL  ""  
#define VARIABLE_LABEL  "" 
#define PUBLISH_FREQUENCY  1000
unsigned long timer;
Ubidots ubidots(UBIDOTS_TOKEN);

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void setup()
{   
    Serial.begin(115200);
    pinMode(pinoBuzzer,OUTPUT);
    pinMode(pinoSensorLuz,INPUT);
    ubidots.connectToWifi(WIFI_SSID, WIFI_PASS);
    ubidots.setCallback(callback);
    ubidots.setup();
    ubidots.reconnect();
    timer = millis();
}
 
void loop()
{
  int valorLuz = analogRead(pinoSensorLuz);
  int value;         
  if(valorLuz<600)
  {       
    analogWrite(pinoBuzzer, 150);
    value=1;
  }
  else
  {
    analogWrite(pinoBuzzer, 0);
    value=0;
  }
  delay(10);
  if (!ubidots.connected())
  {
    ubidots.reconnect();
  }
  if (millis() - timer > PUBLISH_FREQUENCY) 
  {
    ubidots.add(VARIABLE_LABEL, value);
    ubidots.publish(DEVICE_LABEL);
    timer = millis();
  }
  ubidots.loop();                   
}
