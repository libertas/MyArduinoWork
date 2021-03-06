int speakerPin = 9;
int LED = 13;
int Button = 2;
int state = HIGH; int length = 42; // the number of noteschar notes[] = "ccggaagffeeddcggffeedggffeedccggaagffeeddc "; // a space represents a restint beats[] = { 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2, 4 };int tempo = 300; void playTone(int tone, int duration) {  for (long i = 0; i < duration * 1000L; i += tone * 2) {    digitalWrite(speakerPin, HIGH);    delayMicroseconds(tone);    digitalWrite(speakerPin, LOW);    delayMicroseconds(tone);  }} void playNote(char note, int duration) {  char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' };  int tones[] = { 1915, 1700, 1519, 1432, 1275, 1136, 1014, 956 };   // play the tone corresponding to the note name  for (int i = 0; i < 8; i++) {    if (names[i] == note) {      playTone(tones[i], duration);    }  }}

void shine() {
	while(state == HIGH) {
		delay(1000);
		digitalWrite(LED, LOW);
		delay(1000);
		digitalWrite(LED, HIGH);
	}
	digitalWrite(LED, LOW);
	state = HIGH;
}

void checkButton() {
	state = LOW;
}
void setup() {  pinMode(speakerPin, OUTPUT);
  pinMode(LED, HIGH);
  pinMode(Button, INPUT_PULLUP);
  attachInterrupt(0, checkButton, LOW);} void loop() {
  //play the music
  digitalWrite(LED, HIGH);  for (int i = 0; i < length; i++) {    if (notes[i] == ' ') {      delay(beats[i] * tempo); // rest    } else {      playNote(notes[i], beats[i] * tempo);    }     // pause between notes    delay(tempo / 2);   }
  shine();
  //delay 2 hours
  for(int i = 0; i < 60; i++)
  	for(int j = 0; j < 60; j++)
  		delay(2000); }