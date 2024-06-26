#include <Servo.h>//Library
static const int servoPin = 2;//pin 15 doesn't work
Servo servo1;//Servo

void Ser(){
    for(int posDegrees = 0; posDegrees <= 180; posDegrees++) {//For loop to rotate 1 direction
        servo1.write(posDegrees);
        Serial.println(posDegrees);
        delay(20);
    }

    for(int posDegrees = 180; posDegrees >= 0; posDegrees--) {//For loop to rotate other direction
        servo1.write(posDegrees);
        Serial.println(posDegrees);
        delay(20);
    }
  }

  //Servo Function
void SerR() {
  for (int posDegrees = 90; posDegrees >= 0; posDegrees--) {  //For loop to rotate 1 direction
    servo1.write(posDegrees);
    Serial.println(posDegrees);
    delay(20);
  }

  /*for (int posDegrees = 0; posDegrees <= 90; posDegrees++) {  //For loop to rotate other direction
    servo1.write(posDegrees);
    Serial.println(posDegrees);
    delay(20);
  }*/
}

void SerL() {
  for (int posDegrees = 90; posDegrees <= 180; posDegrees++) {  //For loop to rotate 1 direction
    servo1.write(posDegrees);
    Serial.println(posDegrees);
    delay(20);
  }

  /*for (int posDegrees = 180; posDegrees >= 90; posDegrees--) {  //For loop to rotate other direction
    servo1.write(posDegrees);
    Serial.println(posDegrees);
    delay(20);
  }*/
}

void setup(){
  Serial.begin(115200);
  servo1.attach(2);//attach servo to pin
}

void loop(){
 SerL();//Call func
 delay(1000);
 SerR();
 //delay(1000);
}
