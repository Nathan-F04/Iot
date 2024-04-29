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

//Setup
void setup() {
  Serial.begin(115200);
  attachInterrupt(digitalPinToInterrupt(23), Right, FALLING);
  attachInterrupt(digitalPinToInterrupt(22), Left, FALLING);


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
void Forward() {
  Serial.println("Going Forward");
  digitalWrite(M1ON,HIGH);
  digitalWrite(MotorPin1, HIGH);
  digitalWrite(MotorPin2, LOW);
  digitalWrite(M2ON, HIGH);
  digitalWrite(MotorPin3, HIGH);
  digitalWrite(MotorPin4, LOW);
  delay(1000);
}

void Reverse() {
  Serial.println("Reversing");
  digitalWrite(M1ON, HIGH);
  digitalWrite(M2ON, HIGH);
  digitalWrite(MotorPin1, LOW);
  digitalWrite(MotorPin2, HIGH);
  digitalWrite(MotorPin3, LOW);
  digitalWrite(MotorPin4, HIGH);
  delay(1000);
}

void Left() {
  //Serial.println("Going Left");
  digitalWrite(M1ON, LOW);
  digitalWrite(M2ON, HIGH);
  digitalWrite(MotorPin1, LOW);
  digitalWrite(MotorPin2, LOW);
  digitalWrite(MotorPin3, HIGH);
  digitalWrite(MotorPin4, LOW);
  for (int i = 0; i < 10; i++) delayMicroseconds(15000);
}

void Right() {
  Serial.println("Going Right");
  /*
  digitalWrite(M1ON, HIGH);
  digitalWrite(M2ON, LOW);
  digitalWrite(MotorPin1, HIGH);
  digitalWrite(MotorPin2, LOW);
  digitalWrite(MotorPin3, LOW);
  digitalWrite(MotorPin4, LOW);
  for (int i = 0; i < 10; i++) delayMicroseconds(15000);*/
}

void Stop() {
  Serial.println("Stopping");
  digitalWrite(M1ON, LOW);
  digitalWrite(M2ON, LOW);
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
  Serial.print("Left is ");  //left from behind
  Serial.print(r1);
  Serial.print("\n");
  Serial.print("Right is ");
  Serial.print(r2);
  Serial.print("\n");
}
//Void Loop
void loop() {
  //Motor Function call(only forward for testing)
  Forward();
  delay(1000);

  //Line Sensor Function Call 1 is white
  //Sense();
}
