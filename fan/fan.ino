#include <avr/sleep.h>

int S = 13;  // The output pin

void setup() {
  pinMode(S, OUTPUT);

  // Slow down the CPU
  noInterrupts();
  CLKPR = 1 << CLKPCE;  // Preparing the process
  CLKPR = 8;  // Set speed = 1/256 of ordinary spped
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

