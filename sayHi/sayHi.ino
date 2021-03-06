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
  wdt_enable(WDTO_8S);

  pinMode(0, OUTPUT);
  digitalWrite(0, LOW);

  pinMode(in1, INPUT);
  pinMode(out1, OUTPUT);

  pinMode(in2, INPUT);
  pinMode(out2, OUTPUT);

  digitalWrite(out1, LOW);
  digitalWrite(out2, LOW);
}

void loop()
{
  if(digitalRead(in1))
  {
    wdt_reset();
    while(!digitalRead(in2));
    wdt_reset();
    digitalWrite(out1, HIGH);
    sleep();
  }

  if(digitalRead(in2))
  {
    wdt_reset();
    while(!digitalRead(in1));
    wdt_reset();
    digitalWrite(out2, HIGH);
    sleep();
  }

  wdt_enable(WDTO_500MS);
  sleep();
}

