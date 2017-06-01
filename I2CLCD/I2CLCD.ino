
#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {
  
  lcd.begin(16, 2);
  
  Serial.begin(115200);
}

void loop() {
  
  lcd.noCursor();
  delay(500);
  
  lcd.cursor();
  delay(500);
}

void updateLCD(char c) {
  static int count = 0;
  
  lcd.setCursor(count % 16, count / 16);
  lcd.write(c);
  
  count = (count + 1) % 32;
  Serial.println(count);
}

void serialEvent() {
  while (Serial.available()) {
    
    char c = (char)Serial.read(); 
    
    updateLCD(c);
  }
}
