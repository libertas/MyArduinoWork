int Button = 2;
int LEDpin = 13;
int state = LOW;
int i;

void setup() {
	pinMode(Button, INPUT_PULLUP);
	pinMode(LEDpin, OUTPUT);
}

void changeState() {
	if(state == LOW)
	{
		state = HIGH;
		digitalWrite(LEDpin, HIGH);
	}
	else
	{
		state = LOW;
		digitalWrite(LEDpin, LOW);
	}
}

void loop() {
	char c = 0;
	int buttonState = digitalRead(Button);
	if(buttonState==0)
	{
		for(i = 0; i < 15; i++)
		{
			delay(1);
			buttonState == 0 ? c++: c--;
		}
		if(c >= 0)
		{
			changeState();
			delay(200);
		}
	}
}
