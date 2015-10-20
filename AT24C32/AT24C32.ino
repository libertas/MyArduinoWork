#include <Wire.h>

#define AT24C32_ADDR B1010000

void setup()
{
  Serial.begin(9600);
  Wire.begin();
  Wire.beginTransmission(AT24C32_ADDR);
  Wire.write(byte(0x00));
  Wire.write(byte(0x00));
  Wire.write(byte(0xab));
  Wire.endTransmission();
  delay(1000);
}

void loop()
{
  Wire.beginTransmission(AT24C32_ADDR);
  Wire.write(byte(0x00));
  Wire.write(byte(0x00));
  Wire.endTransmission();
  
  Wire.beginTransmission(AT24C32_ADDR);
  Wire.endTransmission();
  
  Wire.requestFrom(AT24C32_ADDR, 1);
  Serial.println(Wire.read(), HEX);
  delay(1000);
}

