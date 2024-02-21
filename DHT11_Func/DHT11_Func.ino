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
 
#include <DFRobot_DHT11.h>
DFRobot_DHT11 DHT;
#define DHT11_PIN 33

void setup(){
  Serial.begin(115200);
}

int temp(){
  DHT.read(DHT11_PIN);
  int temp= DHT.temperature;
  delay(1000);
  return temp;
}

int humi(){
  DHT.read(DHT11_PIN);
  int humi= DHT.humidity;
  delay(1000);
  return humi;
}

void loop(){
  Serial.print("\n----:");
  int a=temp();
  delay(1000);
  Serial.print("\nTemp:");
  Serial.print(a);
   int b=humi();
  delay(1000);
  Serial.print("\nHumi:");
  Serial.print(b);
  Serial.print("\n----:");
}

