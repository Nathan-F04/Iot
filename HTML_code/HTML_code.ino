
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
#include <DFRobot_DHT11.h>
#include "ThingSpeak.h"  // always include thingspeak header file after other header files and custom macros
DFRobot_DHT11 DHT;
#define DHT11_PIN 33
#include <Adafruit_GPS.h>

// what's the name of the hardware serial port?
#define GPSSerial Serial2

// Connect to the GPS on the hardware port
Adafruit_GPS GPS(&GPSSerial);

// Set GPSECHO to 'false' to turn off echoing the GPS data to the Serial console
// Set to 'true' if you want to debug and listen to the raw GPS sentences
#define GPSECHO false
#define REPORTING_PERIOD_MS 20000
uint32_t timer = millis();
uint32_t tsLastReport = 0;

char ssid[] = SECRET_SSID;  // your network SSID (name)
char pass[] = SECRET_PASS;  // your network password


int keyIndex = 0;  // your network key Index number (needed only for WEP)
WiFiClient client;

unsigned long myChannelNumber = SECRET_CH_ID;
const char* myWriteAPIKey = SECRET_WRITE_APIKEY;
// Initialize our values
int number1 = 0;
int number2 = 0;
String myStatus = "";
float temp, humi;

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

String getLat() {
  String lat = String(GPS.latitude);
  return lat;
}

String getLong() {
  String longi = String(GPS.longitude);
  return longi;
}


void handleRoot() {
  String MapLink = "https://www.google.com/maps?q=";
  MapLink += String(GPS.latitude, 6);
  MapLink += ",";
  MapLink += String(GPS.longitude, 6);
  String message = homePagePartDHT11 + getTemp() + homePagePartDHT112 + getHumi() + homePagePartGPS + getLat() + getLong() + "<a href='" + MapLink + "'>" + homePagePartLink + "</a>" + homePagePartComponents;  //+ getTemp() + homePagePart2+ getHumi()+homePagePart3;
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

  //while (!Serial);  // uncomment to have the sketch wait until Serial is ready

  // connect at 115200 so we can read the GPS fast enough and echo without dropping chars
  // also spit it out
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
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
}

void loop(void) {
   server.handleClient();
  DHT.read(DHT11_PIN);
  temp = DHT.temperature;
  Serial.print("\ntemp");
  Serial.print(temp);
  humi = DHT.humidity;
  Serial.print("\nhumi");
  Serial.print(humi);

  // set the fields with the values
  ThingSpeak.setField(1, temp);
  ThingSpeak.setField(2, humi);
  // write to the ThingSpeak channel


  if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
    ThingSpeak.setStatus(myStatus);
    int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
    tsLastReport = millis();
  }
  /*int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }*/
  delay(20000);
}