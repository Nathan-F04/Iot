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

const int TRIG_PIN = 5;	 //Trigger
const int ECHO_PIN = 18; //Echo 
long duration; // used to measure time from transmition until echo returns
float cms, inches;

void setup(){
//Serial Port begin
Serial.begin (115200);
//Define inputs and outputs
pinMode(TRIG_PIN,OUTPUT);
pinMode(ECHO_PIN,INPUT);
}

void Sensor(){
  //The sensot is triggered by a HIGH pulse of 10 or more microseconds
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

//Convert the time into a distance
cms = (duration/2)*0.0343;	//Divide by 29.1 or multiply by 0.0343

Serial.print(cms);
Serial.print("cm");
Serial.println();
delay(250);
}
void loop(){
  Sensor();
}
