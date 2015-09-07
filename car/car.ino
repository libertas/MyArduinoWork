#include <avr/wdt.h>
#include <IRremote.h>

int RECV_PIN = 9;

IRrecv irrecv(RECV_PIN);

decode_results results;

const int left1 = 5, left2 = 6, right1 = 7, right2 = 8;
const unsigned long goForward = 0xFFC03F
, goBack = 0xFF40BF
, turnLeft = 0xFF708F
, turnRight = 0xFF58A7;

int trig = 4, echo = 3;

inline double getDistance()
{
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  return pulseIn(echo, HIGH, 5000UL) / 58.8;
}

void setup()
{
  wdt_enable(WDTO_250MS);
  wdt_reset();
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
  pinMode(left1, OUTPUT);
  pinMode(left2, OUTPUT);
  pinMode(right1, OUTPUT);
  pinMode(right2, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(trig, OUTPUT);
}

void loop()
{
  wdt_reset();
  unsigned long irCode = 0;
  static unsigned long lastCode;
  static unsigned char errorCount;
  unsigned cm;

  if (irrecv.decode(&results))
  {
    irCode = results.value;
    Serial.println(irCode, HEX);
    irrecv.resume(); // Receive the next value
  }

  if(irCode == 0xffffffff)
  {
    irCode = lastCode;
  }

  cm = getDistance();
  if(cm > 0.0 && cm < 15.0)
  {
    irCode = goBack;
  }

  switch(irCode)
  {
  case goForward:
    digitalWrite(left1, HIGH);
    digitalWrite(left2, LOW);
    digitalWrite(right1, HIGH);
    digitalWrite(right2, LOW);
    errorCount = 0;
    lastCode = irCode;
    break;
  case goBack:
    digitalWrite(left1, LOW);
    digitalWrite(left2, HIGH);
    digitalWrite(right1, LOW);
    digitalWrite(right2, HIGH);
    errorCount = 0;
    lastCode = irCode;
    break;
  case turnLeft:
    digitalWrite(left1, LOW);
    digitalWrite(left2, LOW);
    digitalWrite(right1, HIGH);
    digitalWrite(right2, LOW);
    errorCount = 0;
    lastCode = irCode;
    break;
  case turnRight:
    digitalWrite(left1, HIGH);
    digitalWrite(left2, LOW);
    digitalWrite(right1, LOW);
    digitalWrite(right2, LOW);
    errorCount = 0;
    lastCode = irCode;
    break;
  default:
    errorCount++;
    if(errorCount < 5)
    {
      break;
    }
    errorCount = 0;
    lastCode = 0;
    digitalWrite(left1, LOW);
    digitalWrite(left2, LOW);
    digitalWrite(right1, LOW);
    digitalWrite(right2, LOW);
    break;
  }

  delay(50);
}

