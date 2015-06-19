#include <avr/power.h>
#include <avr/sleep.h>

int S = 13;  // The output pin

void setup() {
  pinMode(S, OUTPUT);

  // Slow down the CPU
  clock_prescale_set(clock_div_256);
}

void loop() {
  // Start the fan
  digitalWrite(S, HIGH);

  // Delay 1 hour
  delay(14062); // 14062.5 = 1000*60*60/256

  // Stop the fan
  digitalWrite(S, LOW);

  // Stop forever
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sleep_mode();
  sleep_disable();
}

