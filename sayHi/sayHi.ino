// Detect a human and say hi to him using Attiny 13a
#include <avr/wdt.h>
#include <avr/sleep.h>
char in1 = 3, out1 = 4;
char in2 = 1, out2 = 2;

void sleep()
{
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sleep_mode();
  sleep_disable();
}

void setup()
{
  pinMode(in1, INPUT);
  pinMode(out1, OUTPUT);

  pinMode(in2, INPUT);
  pinMode(out2, OUTPUT);

  digitalWrite(out1, LOW);
  digitalWrite(out2, LOW);
}

void loop()
{
  if(digitalRead(in2))
  {
    digitalWrite(out2, HIGH);
    wdt_enable(WDTO_8S);
    sleep();
  }

  if(digitalRead(in1))
  {
    digitalWrite(out1, HIGH);
    wdt_enable(WDTO_8S);
    sleep();
  }

  wdt_enable(WDTO_15MS);
  sleep();
}

