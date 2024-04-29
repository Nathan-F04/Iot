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

//CHECK PINS BEFORE RUNNING 
//Ultrasonic 
const int TRIG_PIN = 5;	 //Trigger 
const int ECHO_PIN = 18; //Echo 
long duration; // used to measure time from transmition until echo returns
float  cm1,tempVal,airVal;


//DC MOTOR
//L293D
//Motor A if facing behnd breadboard Motor A is on the right
const int M1ON=25; //Works
const int MotorPin1  = 21;  // Pin 14 of L293 works
const int MotorPin2  = 19;  // Pin 10 of L293 works
//Motor B
const int M2ON=27;//Works
const int MotorPin3  = 0; // Pin  7 of L293 
const int MotorPin4  = 4;  // Pin  2 of L293 works

//DHT11
#include <DFRobot_DHT11.h>
DFRobot_DHT11 DHT;
#define DHT11_PIN 33//may have to change for gps

//Servo
#include <Servo.h>//Library
static const int servoPin = 2;//pin 15 doesn't work
Servo servo1;//Servo

 int j=0,LDist,RDist;

//Setup
void setup(){
  Serial.begin(115200);
  //Attach servo to pin
  servo1.attach(2);

  //Define inputs and outputs for Ultrasonic
  pinMode(TRIG_PIN,OUTPUT);
  pinMode(ECHO_PIN,INPUT);

  //Set Motor pins as outputs
  pinMode(M1ON, OUTPUT);
  pinMode(M2ON, OUTPUT);
  pinMode(MotorPin1, OUTPUT);
  pinMode(MotorPin2, OUTPUT);
  pinMode(MotorPin3, OUTPUT);
  pinMode(MotorPin4, OUTPUT);
} 
//Functions

//DC Motors
void Reverse()
{
  Serial.println("Reversing");
  digitalWrite(M1ON,HIGH);
  digitalWrite(MotorPin1,HIGH);
  digitalWrite(MotorPin2,LOW);
  digitalWrite(M2ON,HIGH);
  digitalWrite(MotorPin3,HIGH);
  digitalWrite(MotorPin4,LOW);
  delay(1000);
}

void Forward()
{
  Serial.println("Going Forward");
  digitalWrite(M1ON,HIGH);
  digitalWrite(M2ON,HIGH);
  digitalWrite(MotorPin1,LOW);
  digitalWrite(MotorPin2,HIGH);
  digitalWrite(MotorPin3,LOW);
  digitalWrite(MotorPin4,HIGH);
  delay(1000);
}

void Right()
{
  Serial.println("Going Right");
  digitalWrite(M1ON,HIGH);
  digitalWrite(M2ON,LOW);
  digitalWrite(MotorPin1,LOW);
  digitalWrite(MotorPin2,HIGH);
  digitalWrite(MotorPin3,LOW);
  digitalWrite(MotorPin4,LOW);
  delay(1000);
}

void Left()
{
  Serial.println("Going Left");
  digitalWrite(M1ON,LOW);
  digitalWrite(M2ON,HIGH);
  digitalWrite(MotorPin1,LOW);
  digitalWrite(MotorPin2,LOW);
  digitalWrite(MotorPin3,LOW);
  digitalWrite(MotorPin4,HIGH);
  delay(1000);
}

void Stop()
{
  Serial.println("Stopping");
  digitalWrite(M1ON,LOW);
  digitalWrite(M2ON,LOW);
  digitalWrite(MotorPin1,LOW);
  digitalWrite(MotorPin2,LOW);
  digitalWrite(MotorPin3,LOW);
  digitalWrite(MotorPin4,LOW);
  delay(1000);
}

//DHT11 Function
float temp(){
  DHT.read(DHT11_PIN);
  float temp= DHT.temperature; //Passing the temperature reading to a variable
  delay(1000);
  return temp; //Return tempertaure value to main for calulating speed of sound in air
}

//Line Sensor Function
void Sense()
{
  int r1=digitalRead(22);//Uses Pin 22
  int r2=digitalRead(23);//Uses Pin 23
  Serial.print("Sense1 is ");
  Serial.print(r1);
  Serial.print("\n");
  Serial.print("Sense2 is ");
  Serial.print(r2);
  Serial.print("\n");
}

//Servo Function
void SerR(){
    for(int posDegrees = 90; posDegrees <= 180; posDegrees++) {//For loop to rotate 1 direction
        servo1.write(posDegrees);
        Serial.println(posDegrees);
        delay(20);
    }

    for(int posDegrees = 180; posDegrees >= 90; posDegrees--) {//For loop to rotate other direction
        servo1.write(posDegrees);
        Serial.println(posDegrees);
        delay(20);
    }
  }

  //Servo Function
void SerL(){
    for(int posDegrees = 90; posDegrees >= 0; posDegrees--) {//For loop to rotate 1 direction
        servo1.write(posDegrees);
        Serial.println(posDegrees);
        delay(20);
    }

    for(int posDegrees = 0; posDegrees <= 90; posDegrees++) {//For loop to rotate other direction
        servo1.write(posDegrees);
        Serial.println(posDegrees);
        delay(20);
    }
  }


  //Ultrasonic Function
  void Usensor(){
  //The sensor is triggered by a HIGH pulse of 10 or more microseconds
  //Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(TRIG_PIN,LOW);
  delay(5);
  digitalWrite(TRIG_PIN,HIGH);
  delay(10);
  digitalWrite(TRIG_PIN,LOW);

  //Read the signal from the sensor: a HIGH pulse whose
  //duration is the time (in microseconds)
  // of the ping to the reception of its echo off an object
  duration = pulseIn(ECHO_PIN,HIGH);

  //Calling DHT11 function and storing it in tempVal
  tempVal=temp();
  //Calculating the speed of sound in air using the value from the DHT11
  //Formula: 331.5+ 0.6T where T is temprature in degrees celcius
  airVal=331.5+(0.6*tempVal);
  //Calculating distance
  cm1=(duration/2)*(airVal/10000);

  //Printing distance from HC-SR04
  Serial.print(cm1);
  Serial.print("cm:");
  Serial.println();

   if(cm1<20){
    j=1;
  }
  else{
    j=0;
  }
}


//Void Loop
void loop(){

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
  /*Forward();
  delay(1000);

  //Line Sensor Function Call 1 is white
  // Sense();
  //delay(1000);


  if(j=1){
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
    }
  }
VAL = 2000;

//Setup
void setup() {
  Serial.begin(115200);

  //GPS setup info below:
  //while (!Serial);  // uncomment to have the sketch wait until Serial is ready

  // connect at 115200 so we can read the GPS fast enough and echo without dropping chars
  // also spit it out
  Serial.begin(115200);
  Serial.println("Adafruit GPS library basic parsing test!");

  // 9600 NMEA is the default baud rate for Adafruit MTK GPS's- some use 4800
  GPS.begin(9600);
  // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  // uncomment this line to turn on only the "minimum recommended" data
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
  // For parsing data, we don't suggest using anything but either RMC only or RMC+GGA since
  // the parser doesn't care about other sentences at this time
  // Set the update rate
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);  // 1 Hz update rate
  // For the parsing code to work nicely and have time to sort thru the data, and
  // print it out we don't suggest using anything higher than 1 Hz

  // Request updates on antenna status, comment out to keep quiet
  GPS.sendCommand(PGCMD_ANTENNA);

  delay(1000);

  // Ask for firmware version
  GPSSerial.println(PMTK_Q_RELEASE);

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
  //Serial.println("Going Right");
  analogWrite(M1ON, 255);
  analogWrite(M2ON, 0);
  digitalWrite(MotorPin1, LOW);
  digitalWrite(MotorPin2, HIGH);
  digitalWrite(MotorPin3, LOW);
  digitalWrite(MotorPin4, LOW);
  //delay(1000);
}

void Left() {
  Serial.println("Going Left");
  analogWrite(M1ON, 0);
  analogWrite(M2ON, 255);
  digitalWrite(MotorPin1, LOW);
  digitalWrite(MotorPin2, LOW);
  digitalWrite(MotorPin3, LOW);
  digitalWrite(MotorPin4, HIGH);
  //delay(1000);
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
  
  //Servo Function call
  SerL();//Call func
  delay(1000);

  //Ultrasonic Function call
  Usensor();
  delay(1000);

  //Motor Function call(only forward for testing)
  Forward();
  delay(1000);

  //Line Sensor Function Call 1 is white
  Sense();
  delay(1000);

  unsigned long currentMillis = millis();

  // read data from the GPS in the 'main loop'
  char c = GPS.read();
  // if you want to debug, this is a good time to do it!
  if (GPSECHO)
    if (c) Serial.print(c);
  // if a sentence is received, we can check the checksum, parse it...
  if (GPS.newNMEAreceived()) {
    // a tricky thing here is if we print the NMEA sentence, or data
    // we end up not listening and catching other sentences!
    // so be very wary if using OUTPUT_ALLDATA and trying to print out data
    Serial.print(GPS.lastNMEA());    // this also sets the newNMEAreceived() flag to false
    if (!GPS.parse(GPS.lastNMEA()))  // this also sets the newNMEAreceived() flag to false
      return;                        // we can fail to parse a sentence in which case we should just wait for another
  }

  // approximately every 2 seconds or so, print out the current stats
  if (millis() - timer > 2000) {
    timer = millis();  // reset the timer
    Serial.print("\nTime: ");
    if (GPS.hour < 10) { Serial.print('0'); }
    Serial.print(GPS.hour, DEC);
    Serial.print(':');
    if (GPS.minute < 10) { Serial.print('0'); }
    Serial.print(GPS.minute, DEC);
    Serial.print(':');
    if (GPS.seconds < 10) { Serial.print('0'); }
    Serial.print(GPS.seconds, DEC);
    Serial.print('.');
    if (GPS.milliseconds < 10) {
      Serial.print("00");
    } else if (GPS.milliseconds > 9 && GPS.milliseconds < 100) {
      Serial.print("0");
    }
    Serial.println(GPS.milliseconds);
    Serial.print("Date: ");
    Serial.print(GPS.day, DEC);
    Serial.print('/');
    Serial.print(GPS.month, DEC);
    Serial.print("/20");
    Serial.println(GPS.year, DEC);
    Serial.print("Fix: ");
    Serial.print((int)GPS.fix);
    Serial.print(" quality: ");
    Serial.println((int)GPS.fixquality);
    if (GPS.fix) {
      Serial.print("Location: ");
      Serial.print(GPS.latitude, 4);
      Serial.print(GPS.lat);
      float fl, fl2;
      int i = 0;
      double lat, longi;
      fl = GPS.longitude / 100;
      i = fl;
      fl2 = fl - i;
      fl2 = fl2 / 0.6;
      lat = fl2 + i;
      Serial.print(", ");
      Serial.print(GPS.longitude, 4);
      Serial.println(GPS.lon);
      Serial.print(i);
      Serial.print(",fl:");
      Serial.print(lat, 6);
      Serial.print("\n");
      fl = GPS.latitude / 100;
      i = fl;
      fl2 = fl - i;
      fl2 = fl2 / 0.6;
      longi = fl2 + i;
      Serial.print(i);
      Serial.print(",fl:");
      Serial.print(longi, 6);
      Serial.print("\n");
      Serial.print("Satellites: ");
      Serial.println((int)GPS.satellites);
      Serial.print("Antenna status: ");
      Serial.println((int)GPS.antenna);
    }
  }

  /*Serial.println("Testing speed");
  digitalWrite(MotorPin1, HIGH);
  digitalWrite(MotorPin2, LOW);
  digitalWrite(MotorPin3, HIGH);
  digitalWrite(MotorPin4, LOW);
  int j=50;
  analogWrite(M1ON, j);
  analogWrite(M2ON, j);
  delay(200);
  j=100;
  analogWrite(M1ON, j);
  analogWrite(M2ON, j);
  delay(200);
  j=150;
  analogWrite(M1ON, j);
  analogWrite(M2ON, j);
  delay(200);
  j=200;
  analogWrite(M1ON, j);
  analogWrite(M2ON, j);
  delay(200);
  delay(1000);*/


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
  //delay(2);  //allow the cpu to switch to other tasks
  if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
    //Thingspeak.setStatus(myStatus);
    int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
    if (x == 200) {
      Serial.println("\nChannel update successful.");
    } else {
      Serial.println("\nProblem updating channel. HTTP error code " + String(x));
    }
    tsLastReport = millis();
  }
  // Write to ThingSpeak. There are up to 8 fields in a channel, allowing you to store up to 8 different
  // pieces of information in a channel.  Here, we write to field 1.
  if (currentMillis - previousMillis >= INTERVAL) {
    //save the last time you read the sensors
    previousMillis = currentMillis;
    DHT.read(DHT11_PIN);
    temp = DHT.temperature;
    Serial.print("\nTemp:");
    Serial.print(temp);
    humi = DHT.humidity;
    Serial.print(" \nHumi:");
    Serial.print(humi);
  }

  // set the fields with the values
  ThingSpeak.setField(1, temp);
  ThingSpeak.setField(2, humi);

}
