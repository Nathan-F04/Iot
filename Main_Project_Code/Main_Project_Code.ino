/*
The code below works as follows:
Before void setup the libraries for each component is added,
variables are named, and pins are set with each component having its own subsection for easier viewing
There are separate functions for the DHT11 temperature sensor, Servo motor,DC Motors and HC-SR04 ultrasonic sensor
Two functions interact with each other
The DHT11 function passes a temperature to the ultrasonic sensor
The functions are called in void loop which prints the temperature the DHT11 reads, moves the servo,drives the motors and then prints the distance the HC-SR04 reads
*/
//To do
//line sensors: order more, get em working again
//Research state machines see if they work
//look at Thinkspeak for gps widget
//Find gps code if not already gotten and figure out how it works

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
//CHECK PINS BEFORE RUNNING

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

//putting my webpage together
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
//Ultrasonic
const int TRIG_PIN = 5;   //Trigger
const int ECHO_PIN = 18;  //Echo
long duration;            // used to measure time from transmition until echo returns
float cm1, tempVal, airVal;


//DC MOTOR
//L293D
//Motor A if facing behnd breadboard Motor A is on the right
const int M1ON = 25;       //Works
const int MotorPin1 = 21;  // Pin 14 of L293 works
const int MotorPin2 = 19;  // Pin 10 of L293 works
//Motor B
const int M2ON = 27;      //Works
const int MotorPin3 = 0;  // Pin  7 of L293
const int MotorPin4 = 4;  // Pin  2 of L293 works

//SEN0017 pins
const int LSense = 23;
const int RSense = 24;

//DHT11
#include <DFRobot_DHT11.h>
DFRobot_DHT11 DHT;
#define DHT11_PIN 33


//Temperature function from html
String getTemp() {
  DHT.read(DHT11_PIN);
  String temp = String(DHT.temperature);
  return temp;
}
//Humidity function from html
String getHumi() {
  String humi = String(DHT.humidity);
  return humi;
}

int tempy(){
  DHT.read(DHT11_PIN);
  int temp= DHT.temperature;
  delay(1000);
  return temp;
}

//Servo
#include <Servo.h>              //Library
static const int servoPin = 2;  //pin 15 doesn't work
Servo servo1;                   //Servo

int j = 0, LDist, RDist;

//Setup
void setup() {
  Serial.begin(115200);

  //interrupt setup for line sensors
  //sets pins to high to start
  pinMode(LSense, INPUT_PULLUP);
  pinMode(RSense, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(23), Left, FALLING);
  attachInterrupt(digitalPinToInterrupt(22), Right, FALLING);

  //Attach servo to pin
  servo1.attach(2);

  //Define inputs and outputs for Ultrasonic
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  //Set Motor pins as outputs
  pinMode(M1ON, OUTPUT);
  pinMode(M2ON, OUTPUT);
  pinMode(MotorPin1, OUTPUT);
  pinMode(MotorPin2, OUTPUT);
  pinMode(MotorPin3, OUTPUT);
  pinMode(MotorPin4, OUTPUT);

  //Thingspeak and html setup code:
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
}
//Functions

//DC Motors
void Reverse() {
  Serial.println("Reversing");
  analogWrite(M1ON, 255);
  digitalWrite(MotorPin1, HIGH);
  digitalWrite(MotorPin2, LOW);
  analogWrite(M2ON, 255);
  digitalWrite(MotorPin3, HIGH);
  digitalWrite(MotorPin4, LOW);
  delay(1000);
}

void Forward() {
  Serial.println("Going Forward");
  analogWrite(M1ON, 255);
  analogWrite(M2ON, 255);
  digitalWrite(MotorPin1, LOW);
  digitalWrite(MotorPin2, HIGH);
  digitalWrite(MotorPin3, LOW);
  digitalWrite(MotorPin4, HIGH);
  delay(1000);
}

void Right() {
  Serial.println("Going Right");
  analogWrite(M1ON, 255);
  analogWrite(M2ON, 0);
  digitalWrite(MotorPin1, LOW);
  digitalWrite(MotorPin2, HIGH);
  digitalWrite(MotorPin3, LOW);
  digitalWrite(MotorPin4, LOW);
  delay(1000);
}

void Left() {
  Serial.println("Going Left");
  analogWrite(M1ON, 0);
  analogWrite(M2ON, 255);
  digitalWrite(MotorPin1, LOW);
  digitalWrite(MotorPin2, LOW);
  digitalWrite(MotorPin3, LOW);
  digitalWrite(MotorPin4, HIGH);
  delay(1000);
}

void Stop() {
  Serial.println("Stopping");
  analogWrite(M1ON, 0);
  analogWrite(M2ON, 0);
  digitalWrite(MotorPin1, LOW);
  digitalWrite(MotorPin2, LOW);
  digitalWrite(MotorPin3, LOW);
  digitalWrite(MotorPin4, LOW);
  delay(1000);
}

//Line Sensor Function
void Sense() {
  int r1 = digitalRead(22);  //Uses Pin 22
  int r2 = digitalRead(23);  //Uses Pin 23
  Serial.print("Sense1 is ");
  Serial.print(r1);
  Serial.print("\n");
  Serial.print("Sense2 is ");
  Serial.print(r2);
  Serial.print("\n");
}

//Servo Function
void SerR() {
  for (int posDegrees = 90; posDegrees <= 180; posDegrees++) {  //For loop to rotate 1 direction
    servo1.write(posDegrees);
    Serial.println(posDegrees);
    delay(20);
  }

  for (int posDegrees = 180; posDegrees >= 90; posDegrees--) {  //For loop to rotate other direction
    servo1.write(posDegrees);
    Serial.println(posDegrees);
    delay(20);
  }
}

//Servo Function
void SerL() {
  for (int posDegrees = 90; posDegrees >= 0; posDegrees--) {  //For loop to rotate 1 direction
    servo1.write(posDegrees);
    Serial.println(posDegrees);
    delay(20);
  }

  for (int posDegrees = 0; posDegrees <= 90; posDegrees++) {  //For loop to rotate other direction
    servo1.write(posDegrees);
    Serial.println(posDegrees);
    delay(20);
  }
}


//Ultrasonic Function
void Usensor() {
  //The sensor is triggered by a HIGH pulse of 10 or more microseconds
  //Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(TRIG_PIN, LOW);
  delay(5);
  digitalWrite(TRIG_PIN, HIGH);
  delay(10);
  digitalWrite(TRIG_PIN, LOW);

  //Read the signal from the sensor: a HIGH pulse whose
  //duration is the time (in microseconds)
  // of the ping to the reception of its echo off an object
  duration = pulseIn(ECHO_PIN, HIGH);

  //Calling DHT11 function and storing it in tempVal
  tempVal = tempy();
  //Calculating the speed of sound in air using the value from the DHT11
  //Formula: 331.5+ 0.6T where T is temprature in degrees celcius
  airVal = 331.5 + (0.6 * tempVal);
  //Calculating distance
  cm1 = (duration / 2) * (airVal / 10000);

  //Printing distance from HC-SR04
  Serial.print(cm1);
  Serial.print("cm:");
  Serial.println();

  if (cm1 < 20) {
    j = 1;
  } else {
    j = 0;
  }
}



//Void Loop
void loop() {

  //DHT11 Function call; added to ultrasonic but for testing purposes leave in main to see temperature value
  //int a=temp();
  //delay(1000);
  //Serial.print("\nTemp:");
  //Serial.print(a);
  //Serial.print("\n");
  //delay(1000);

  //Servo Function call
  //Serstart();//Call func
  //delay(1000);


  //Ultrasonic Function call
  //Usensor();
  //delay(1000);

  //Motor Function call(only forward for testing)
  //Forward();
  //delay(1000);

  //Line Sensor Function Call 1 is white
  // Sense();
  //delay(1000);


  /*if(j=1){
    Stop();
    SerL();
    Usensor();
    LDist=cm1;
    delay(500);
    SerR();
    Usensor();
    RDist=cm1;
    delay(500);
    if(RDist>LDist){
      Reverse();
      Right();
      j=0;
    }
    else{
      Reverse();
      Left();
      j=0;
    }*/
  //Thingspeak and webserver loop code below:
  server.handleClient();
  delay(2);  //allow the cpu to switch to other tasks
    // Write to ThingSpeak. There are up to 8 fields in a channel, allowing you to store up to 8 different
  // pieces of information in a channel.  Here, we write to field 1.
  DHT.read(DHT11_PIN);
  temp = DHT.temperature;
  Serial.print("\nTemp:");
  Serial.print(temp);
  humi = DHT.humidity;
  Serial.print(" \nHumi:");
  Serial.print(humi);

  // set the fields with the values
  ThingSpeak.setField(1, temp);
  ThingSpeak.setField(2, humi);
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if (x == 200) {
    Serial.println("\nChannel update successful.");
  } else {
    Serial.println("\nProblem updating channel. HTTP error code " + String(x));
  }

  delay(20000);  // Wait 20 seconds to update the channel again
}
