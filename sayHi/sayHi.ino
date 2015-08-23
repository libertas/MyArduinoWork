// Detect a human and say hi to him using Attiny 13a
#include <avr/wdt.h>
#include <avr/sleep.h>
int in = 3, out = 4;
int state = LOW;

void sleep()
{
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sleep_mode();
  sleep_disable();
}

void setup()
{
  pinMode(in, INPUT);
  pinMode(out, OUTPUT);
  digitalWrite(out, state);
}

void loop()
{
  if(digitalRead(in))
  {
    state = !state;
    digitalWrite(out, state);
    wdt_enable(WDTO_8S);
    sleep();
    
  }
  wdt_enable(WDTO_15MS);
  sleep();
}
