#include <avr/sleep.h>

int S = 13;  // The output pin

void setup() {
  pinMode(S, OUTPUT);

}

void loop() {
  // Start the fan
  digitalWrite(S, HIGH);

  // Delay 1 hour
  for(int i=0; i<60; i++)
    for(int j=0; j<60; j++)
      delay(1000);

  // Stop the fan
  digitalWrite(S, LOW);

  // Stop forever
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sleep_mode();
  sleep_disable();
}

