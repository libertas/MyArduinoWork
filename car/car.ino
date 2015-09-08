#include <avr/wdt.h>
#include <IRremote.h>

int RECV_PIN = 8;

IRrecv irrecv(RECV_PIN);

decode_results results;

const int left1 = 9, left2 = 5, right1 = 10, right2 = 6;
const unsigned long goForward = 0xFFC03F
, goBack = 0xFF40BF
, turnLeft = 0xFF708F
, turnRight = 0xFF58A7
, turnOnHorn = 0xFFE01F;

const int trig = 4, echo = 3;
const int horn = 14;

inline double getDistance()
{
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  return pulseIn(echo, HIGH, 5000UL) / 58.8;
}

void beeBee()
{
  unsigned int i;
  for(i=0; i<200; i++)
  {
    digitalWrite(horn, HIGH);
    delay(1);
    digitalWrite(horn, LOW);
    delay(1);
  }
  delay(300);
}

void setup()
{
  wdt_enable(WDTO_2S);
  wdt_reset();
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
  pinMode(left1, OUTPUT);
  pinMode(left2, OUTPUT);
  pinMode(right1, OUTPUT);
  pinMode(right2, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(trig, OUTPUT);
  pinMode(horn, OUTPUT);
}

void loop()
{
  wdt_reset();
  unsigned long irCode = 0;
  static unsigned long lastCode;
  static unsigned char errorCount;
  double cm;
  unsigned char flagHorn = 0;
  unsigned char spd;

  if (irrecv.decode(&results))
  {
    irCode = results.value;
    Serial.println(irCode, HEX);
    irrecv.resume(); // Receive the next value
  }

  if(irCode == 0xffffffff)
  {
    irCode = lastCode;
    spd = 255;
  }
  else
  {
    spd = 150;
  }

  cm = getDistance();
  if(cm > 0.0 && cm < 7.0)
  {
    irCode = goBack;
    flagHorn = 1;
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
    analogWrite(left2, spd);
    digitalWrite(right1, LOW);
    analogWrite(right2, spd);
    errorCount = 0;
    lastCode = irCode;
    break;
  case turnLeft:
    digitalWrite(left1, LOW);
    digitalWrite(left2, LOW);
    analogWrite(right1, spd);
    digitalWrite(right2, LOW);
    errorCount = 0;
    lastCode = irCode;
    break;
  case turnRight:
    analogWrite(left1, spd);
    digitalWrite(left2, LOW);
    digitalWrite(right1, LOW);
    digitalWrite(right2, LOW);
    errorCount = 0;
    lastCode = irCode;
    break;
  case turnOnHorn:
    flagHorn = 1;
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

  if(flagHorn)
  {
    beeBee();
  }

  delay(50);
}

