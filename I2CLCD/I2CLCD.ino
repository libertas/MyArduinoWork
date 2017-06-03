
#include <EEPROM.h>
#include <LiquidCrystal.h>
#include <Wire.h>

#define I2CLCD_ADDRESS 0x12
#define EEPROM_SIZE 1024
#define EEPROM_BLOCK_SIZE 33

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
int count = 0;
int eepromaddr;

char data[2][16] = {0};

void setup() {
  
  lcd.begin(16, 2);
  
  eepromaddr = EEPROM.read(0);
  if((eepromaddr + 1) * EEPROM_BLOCK_SIZE + 1 > EEPROM_SIZE) {
    eepromaddr = 0;
    EEPROM.write(0, eepromaddr);
  }
  for(int i = 0; i < 32; i++) {
    char c = EEPROM.read(eepromaddr * EEPROM_BLOCK_SIZE + 2 + i);
    ((char*)data)[i] = c;
  }
  
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
  int eepromcount = EEPROM.read(eepromaddr * EEPROM_BLOCK_SIZE + 1);
  if(eepromcount == 0) {
    eepromaddr++;
    if((eepromaddr + 1) * EEPROM_BLOCK_SIZE + 1 > EEPROM_SIZE) {
      eepromaddr = 0;
    }
    EEPROM.write(eepromaddr * EEPROM_BLOCK_SIZE + 1, 0xff);
    EEPROM.write(0, eepromaddr);
  }
  if(count == 0) {
    eepromcount = EEPROM.read(eepromaddr * EEPROM_BLOCK_SIZE + 1);
    EEPROM.write(eepromaddr * EEPROM_BLOCK_SIZE + 1, --eepromcount);
  }
  EEPROM.write(eepromaddr * EEPROM_BLOCK_SIZE + 2 + count, c);
  
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
