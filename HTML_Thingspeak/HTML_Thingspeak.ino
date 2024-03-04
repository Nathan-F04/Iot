/*
The code below works as follows:
Before void setup the libraries for each component is added,
variables are named, and pins are set with each component having its own subsection for easier viewing
There are separate functions for the DHT11 temperature sensor, Servo motor,DC Motors and HC-SR04 ultrasonic sensor
Two functions interact with each other
The DHT11 function passes a temperature to the ultrasonic sensor
A Website displays the DHT11 temperature
The Forward motor functions is called in void loop as well as an ultrasonic sensor function
If the ultrasonic sensor gets a reading<20 the motors are stopped, the servo rotates to the left then right
taking a distance reading on both sides before deciding where to reverse and turn 
*/


//DHT11 info
/*!
 * @file readDHT11.ino
 * @brief DHT11 is used to read the temperature and humidity of the current environment. 
 *
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license     The MIT License (MIT)
 * @author [Wuxiao](xiao.wu@dfrobot.com)
 * @version  V1.0
 * @date  2018-09-14
 * @url https://github.com/DFRobot/DFRobot_DHT11
 */

//Ultrasonic sensor info
/*
Ultrasonic sensor pins:
	Vcc:+5VDC
	Trig:Trigger (INPUT) - GPIO 5
	Echo: Echo (OUTPUT) - GPIO 18
	The ECHO signal should be divided using a resistor divider to give a suitable 0 - 3 (5V Device)	(3V3 Device)
	
		e.g. HCSR04 __/\/\/\_____ESP32 GPIO 
				1K8  |
 				     | 3K3
				     |/\/\/\____GND
	GND: GND
*/

//DHT11
#include <DFRobot_DHT11.h>
DFRobot_DHT11 DHT;
#define DHT11_PIN 33

// This code is derived from the HelloServer Example
// in the (ESP32) WebServer library .
//
// It hosts a webpage which has one temperature reading to display.
// The webpage is always the same apart from the reading which would change.
// The getTemp() function simulates getting a temperature reading.
// homePage.h contains 2 constant string literals which is the two parts of the
// webpage that never change.
// handleRoot() builds up the webpage by adding as a C++ String:
// homePagePart1 + getTemp() +homePagePart2
// It then serves the webpage with the command:
// server.send(200, "text/html", message);
// Note the text is served as html.
//
// Replace the code in the homepage.h file with your own website HTML code.
//
// This example requires only an ESP32 and download cable. No other hardware is reuired.
// A wifi SSID and password is required.
// Written by: Natasha Rohan  12/3/23
//
#include "secrets.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include "homepage.h"
#include "ThingSpeak.h" // always include thingspeak header file after other header files and custom macros

char ssid[] = SECRET_SSID;   // your network SSID (name) 
char pass[] = SECRET_PASS;   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

//used for thingspeak data transfer
int number1 = 0;
int number2=0;

//variables for dht11 with thingspeak
float temp,humi;
WebServer server(80);

//Temperature function
String getTemp() {
  DHT.read(DHT11_PIN);
  String temp = String(DHT.temperature);
  return temp;
}
//Humidity function
String getHumi() {
  String humi = String(DHT.humidity);
  return humi;
}


void handleRoot() {
  String message = homePagePartDHT11 +getTemp()+ homePagePartDHT112+getHumi() + homePagePartGPS + homePagePartLink + homePagePartComponents;
  server.send(200, "text/html", message);
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/html", message);
}

void setup(void) {

  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);  // Initialize ThingSpeak
  WiFi.begin(ssid, pass);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);
  server.on("/inline", []() {
    server.send(200, "text/html", "this works as well");
  });
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");

  Serial.begin(115200);
}

void loop(void) {
  server.handleClient();
  delay(2);  //allow the cpu to switch to other tasks
    // Write to ThingSpeak. There are up to 8 fields in a channel, allowing you to store up to 8 different
  // pieces of information in a channel.  Here, we write to field 1.
  DHT.read(DHT11_PIN);
  temp=DHT.temperature;
  Serial.print("\nTemp:");
  Serial.print(temp);
  humi=DHT.humidity;
  Serial.print(" \nHumi:");
  Serial.print(humi);

  // set the fields with the values
  ThingSpeak.setField(1, temp);
  ThingSpeak.setField(2, humi);
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if(x == 200){
    Serial.println("\nChannel update successful.");
  }
  else{
    Serial.println("\nProblem updating channel. HTTP error code " + String(x));
  }

  delay(20000); // Wait 20 seconds to update the channel again
}