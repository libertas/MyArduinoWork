#include <avr/power.h>
#include <avr/sleep.h>
#include <Servo.h>

Servo myServo;

int S = 12;  // The output pin
int Button = 2;  // The button
int ServoPin = 9;  // The servo
int LED = 13; // The LED pin

/*
  The angle of the servo.
  The running time = 0.5 + angle*0.5/45 (hours)
*/
int angle = 0;

void moveServo()
{
  myServo.write(angle);
  delay(15);
}

int buttonCount = 10000;  // The default number is a magic number

void onButtonClicked()
{
  if (buttonCount < 10000)
  {
    digitalWrite(LED, HIGH);
    delay(1000);
    digitalWrite(LED, LOW);
    if (angle < 180)
      angle += 45;
    else
      angle = 0;
    moveServo();
    buttonCount = 10000;
  }
  else
  {
    buttonCount++;
  }
}

void setup() {
  // Set up the pins
  pinMode(S, OUTPUT);
  pinMode(LED, OUTPUT);

  // Init the servo
  myServo.attach(ServoPin);
  moveServo();

  // Set up the button
  pinMode(Button, INPUT_PULLUP);
  attachInterrupt(0, onButtonClicked, LOW);
}

void loop() {
  // Start the fan
  digitalWrite(S, HIGH);

  for (; angle >= 0; angle -= 45)
  {
    // Delay 0.5 hour
    for (int i = 0; i < 30; i++)
    {
      for (int j = 0; j < 60; j++)
        delay(1000);
      moveServo();
    }
  }

  // Stop the fan
  digitalWrite(S, LOW);

  // Stop forever
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sleep_mode();
  sleep_disable();
}

