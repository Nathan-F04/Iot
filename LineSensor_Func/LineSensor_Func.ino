///Arduino Sample Code
void setup()
{
  Serial.begin(115200);
}

void Sense()
{
  int r1=digitalRead(22);
  int r2=digitalRead(23);
  Serial.print("Sense1 is ");
  Serial.print(r1);
  Serial.print("\n");
  Serial.print("Sense2 is ");
  Serial.print(r2);
  Serial.print("\n");
}
void loop()
{
  /*Serial.println(digitalRead(23)); //print the data from the sensor
  Serial.println(digitalRead(22)); //print the data from the sensor
  delay(500);*/
  Sense();
  delay(500);
}

