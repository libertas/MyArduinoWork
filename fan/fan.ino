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

void onButtonClicked()
{
  digitalWrite(LED, HIGH);
  delay(1000);
  digitalWrite(LED, LOW);
  if(angle < 180)
    angle += 45;
  else
    angle = 0;
  for(int i=0; i<45; i++)
  {
    myServo.write(angle);
    delay(15);
  }
}

void setup() {
  pinMode(S, OUTPUT);
  pinMode(LED, OUTPUT);

  // Init the servo
  myServo.attach(ServoPin);
  for(int i=0; i<180; i++)
  {
    myServo.write(0);
    delay(20);
  }

  pinMode(Button, INPUT_PULLUP);
  attachInterrupt(0, onButtonClicked, LOW);
}

void loop() {
  // Start the fan
  digitalWrite(S, HIGH);

  // Delay 0.5 hour
  for(; angle > 0; angle -=45)
    for(int i=0; i<3; i++)
      for(int j=0; j<6; j++)
      {
        delay(100);
        myServo.write(angle);
      }


  // Reset the servo
  for(int i=0; i<180; i++)
  {
    myServo.write(0);
    delay(15);
  }

  // Stop the fan
  digitalWrite(S, LOW);

  // Stop forever
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sleep_mode();
  sleep_disable();
}

