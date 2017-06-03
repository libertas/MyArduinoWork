
#include <LiquidCrystal.h>
#include <Wire.h>

#define I2CLCD_ADDRESS 0x12

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
int count = 0;

char data[2][16] = {0};

void setup() {
  
  lcd.begin(16, 2);
  
  Wire.begin(I2CLCD_ADDRESS);
  Wire.onReceive(receiveEvent);
  
  Serial.begin(115200);
}

void loop() {
  static int refreshTime = 0;
  static int cursorTime = 0;
  static bool cursorState = 1;
  
  if(millis() - cursorTime > 500) {
    cursorTime = millis();
    if(cursorState) {
      cursorState = false;
      lcd.noCursor();
    } else {
      cursorState = true;
      lcd.cursor();
    }
  }
  
  if(millis() - refreshTime > 100) {
    refreshTime = millis();
    lcd.setCursor(0, 0);
    lcd.print(data[0]);
    lcd.setCursor(0, 1);
    lcd.print(data[1]);
    
  }
}

void updateLCD(char c) {
  data[count / 16][count % 16] = c;
  
  count = (count + 1) % 32;
}

void receiveEvent(int howMany)
{
  while(0 < Wire.available())
  {
    char c = Wire.read();
    updateLCD(c);
  }
}

void serialEvent() {
  while (Serial.available()) {
    
    char c = (char)Serial.read(); 
    
    updateLCD(c);
    Serial.println(count);
  }
}
