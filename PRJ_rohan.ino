#include <DHT.h>  // Including library for dht
#include <ESP8266WiFi.h>//for wifi integration
#include <ThingSpeak.h>//for website
#include <Arduino.h>//for using Arduino Ide on ESP
#include<Wire.h>//for ADC
#include<Adafruit_ADS1X15.h>//ADC
#include "wifi_info.h"//for wifi 
Adafruit_ADS1015 ads;//

const char * Wapi_key = "HFODZADZSFLFCQCG";//webserver
const char* server = "api.thingspeak.com";//webserver
unsigned long ChnlNum = 2007314;//webserver

#define DHTPIN D5//defning pin 5 to DHT
#define Type DHT11//which type of DHT

DHT HT(DHTPIN, Type);//dht fuction
float t, h;
//------------------------------------------------------
int dry = 1024;
int wet = 465;
WiFiClient client;//starting wifi communucation

void gas()
{
int sensorValue = analogRead(A0);
  Serial.print("The   amount of CO2 (in PPM): ");
  Serial.println(sensorValue);
  ThingSpeak.setField(3,sensorValue);//upload info to thingspeak
  delay(500);
}
void tempHumid()
{
  h = HT.readHumidity();//ht is object (h is variable) it will read humidity gives value to ht
  t = HT.readTemperature();

  if (isnan(h) || isnan(t))//if neither temp or humid read
  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  ThingSpeak.setField(1, t);
  ThingSpeak.setField(2, h);


  Serial.print("Temperature");
  Serial.println(t);
  Serial.print("Humidity");
  Serial.println(h);

}


void setup() {
  Wire.begin(D2,D1);//starting I2C communication 
   HT.begin();//
	Serial.begin(9600);
	wifi_connect(); //connecting wifi frm esp 
  delay(10);
  
  ThingSpeak.begin(client);//client is server
  if(!ads.begin())//for adc
  {
 Serial.println("Failed to initialze ADS");
 while(1);
  }
 
}
void loop() 
{
   tempHumid();
   gas();
   int16_t sensorVal,LI;//16 bit Analog value
  
delay(500);
 
  sensorVal=ads.readADC_SingleEnded(1);//Read and display single valued output
  delay(200);
  int percentage = map(sensorVal, wet, dry, 100, 0);//our value btw wet and dry
  Serial.print(percentage);
  Serial.println("%");
  ThingSpeak.setField(4,percentage);
  delay (50);
  LI=ads.readADC_SingleEnded(2);
  Serial.print("Light Intensity");
  Serial.println(LI);
  ThingSpeak.setField(5,LI);
  
  ThingSpeak.writeFields(ChnlNum, Wapi_key);
  delay(100);
  // ---------------

  client.stop();//Stopping the signal from the server

  Serial.println("Waiting...");
  delay(15000);
 }
 
