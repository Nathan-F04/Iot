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
#define DHT11_PIN 17//may have to change for gps

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
