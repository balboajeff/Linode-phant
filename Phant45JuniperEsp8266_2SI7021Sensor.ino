/*
 *  This sketch sends data via HTTP GET requests to data.sparkfun.com service.
 *
 *  You need to get streamId and privateKey at data.sparkfun.com and paste them
 *  below. Or just customize this script to talk to other HTTP servers.
 *
 */

#include <ESP8266WiFi.h>
#include "SparkFun_Si7021_Breakout_Library.h"
#include <Wire.h>
//////////////////si7021 sensor
Weather sensor;
//Create Instance of HTU21D or SI7021 temp and humidity sensor and MPL3115A2 barrometric sensor
float humidity = 0;
float tempf = 0;
int power = 5;
int GND = 0;
/////////////////////end si7021
const char* ssid     ="fairpoint02125";
const char* password = "8023884840";
///////check line 55!!!!!!??????????http port//////////
const char* host = "45.33.90.132";
const char* streamId   = "vqgpa2y8W6hpY4xgyO1Dtq3gQkVe";//put your public key in the quotation marks
const char* privateKey = "rp85maMl3vhwqLNby9dWHYwz8VZb";//put your private key in the quotation marks
//below is an example feed
// //http://45.33.90.132:8080/input/vqgpa2y8W6hpY4xgyO1Dtq3gQkVe?private_key=rp85maMl3vhwqLNby9dWHYwz8VZb&humidity=10.05&temp=1.26
void setup() {
  Serial.begin(9600);
  delay(10);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  //SI7021
     pinMode(power, OUTPUT);
    pinMode(GND, OUTPUT);

    digitalWrite(power, HIGH);
    digitalWrite(GND, LOW);

    //Initialize the I2C sensors and ping them
    sensor.begin();

}

int value = 51;

void loop() {
  
  //++value;
getWeather();
  Serial.print("connecting to ");
  Serial.println(host);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 8080;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  
  // We now create a URI for the request
  //String url = "/input/vqgpa2y8W6hpY4xgyO1Dtq3gQkVe?private_key=rp85maMl3vhwqLNby9dWHYwz8VZb&humidity=ESP&temp=Test";
  String url = "/input/";
  url += streamId;
  url += "?private_key=";
  url += privateKey;
  url +="&humidity=";
  url += humidity;
  url += "&temp=";
  url += tempf;
 
  //http://45.33.90.132:8080/input/vqgpa2y8W6hpY4xgyO1Dtq3gQkVe?private_key=rp85maMl3vhwqLNby9dWHYwz8VZb&humidity=10.05&temp=1.26
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  int timeout = millis() + 5000;
  while (client.available() == 0) {
    if (timeout - millis() < 0) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
  
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  Serial.print("posted temp= ");
  Serial.println(tempf);
  Serial.print("posted humidity= ");
  Serial.println(humidity);
  Serial.println();
  Serial.println("closing connection");
 delay(25000);
}


//Create Instance of HTU21D or SI7021 temp and humidity sensor and MPL3115A2 barrometric sensor


//---------------------------------------------------------------

//---------------------------------------------------------------

//---------------------------------------------------------------
void getWeather()
{
  // Measure Relative Humidity from the HTU21D or Si7021
  humidity = sensor.getRH();

  // Measure Temperature from the HTU21D or Si7021
  tempf = sensor.getTempF();
  // Temperature is measured every time RH is requested.
  // It is faster, therefore, to read it from previous RH
  // measurement with getTemp() instead with readTemp()
}
//---------------------------------------------------------------
/*void printInfo()
{
//This function prints the weather data out to the default Serial Port

  Serial.print("Temp:");
  Serial.print(tempf);
  Serial.print("F, ");

  Serial.print("Humidity:");
  Serial.print(humidity);
  Serial.println("%");
}
*/

