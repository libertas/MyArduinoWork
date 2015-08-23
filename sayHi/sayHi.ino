// Detect a human and say hi to him using Attiny 13a
int in = 3, out = 4, time = 3000;

void setup()
{
  pinMode(in, INPUT);
  pinMode(out, OUTPUT);
  digitalWrite(1, LOW);
}

void loop()
{
  if(digitalRead(in))
  {
    digitalWrite(out, HIGH);
    delay(10);
    digitalWrite(out, LOW);
    delay(time);
  }
}
