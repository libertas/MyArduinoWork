#include <Wire.h>

#define DS1307_ADDR B1101000

void setup()
{
  Serial.begin(9600);
  Wire.begin();
  Wire.beginTransmission(DS1307_ADDR);
  Wire.write(byte(0x00));
  Wire.write(byte(0x00));
  Wire.endTransmission();
}

void loop()
{
  Wire.beginTransmission(DS1307_ADDR);
  Wire.write(byte(0x00));
  Wire.endTransmission();
  Wire.requestFrom(DS1307_ADDR, 1);
  Serial.println(Wire.read(), HEX);
  delay(1000);
}

