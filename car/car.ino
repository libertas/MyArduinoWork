#include <IRremote.h>

int RECV_PIN = 9;

IRrecv irrecv(RECV_PIN);

decode_results results;

const int left1 = 5, left2 = 6, right1 = 7, right2 = 8;
const unsigned long goForward = 0xFF18E7, goBack = 0xFF4AB5, turnLeft = 0xFF10EF, turnRight = 0xFF5AA5, stopAll = 0xFF38C7;

void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
}

void loop()
{
  unsigned long irCode = 0, lastCode = 0;
  if (irrecv.decode(&results))
  {
    irCode = results.value;
    Serial.println(irCode, HEX);
    irrecv.resume(); // Receive the next value
  }

  if(irCode != lastCode)
  {
    switch(irCode)
    {
    case goForward:
      digitalWrite(left1, HIGH);
      digitalWrite(left2, LOW);
      digitalWrite(right1, HIGH);
      digitalWrite(right2, LOW);
      break;
    case goBack:
      digitalWrite(left1, LOW);
      digitalWrite(left2, HIGH);
      digitalWrite(right1, LOW);
      digitalWrite(right2, HIGH);
      break;
    case turnLeft:
      digitalWrite(left1, LOW);
      digitalWrite(left2, LOW);
      digitalWrite(right1, HIGH);
      digitalWrite(right2, LOW);
      break;
    case turnRight:
      digitalWrite(left1, HIGH);
      digitalWrite(left2, LOW);
      digitalWrite(right1, LOW);
      digitalWrite(right2, LOW);
      break;
    case stopAll:
      digitalWrite(left1, LOW);
      digitalWrite(left2, LOW);
      digitalWrite(right1, LOW);
      digitalWrite(right2, LOW);
      break;
    }
  }
  delay(100);
}


