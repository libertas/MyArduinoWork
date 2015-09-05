
const unsigned char cs = 5, io = 6, clk = 7;
unsigned char data = 0;

void initSPI()
{
  digitalWrite(cs, LOW);
  digitalWrite(cs, HIGH);
  delayMicroseconds(4);
}

void stopSPI()
{
  digitalWrite(cs, LOW);
  delayMicroseconds(4);
}

void writeOut(unsigned char value)
{
  pinMode(io, OUTPUT);
  shiftOut(io, clk, LSBFIRST, value);
}

unsigned char readIn()
{
  unsigned char value = 0;
  pinMode(io, INPUT);
  for(int i=0; i<8; i++)
  {
    value |= digitalRead(io) << i;
    digitalWrite(clk, HIGH);
    digitalWrite(clk, LOW);
  }
  return value;
}

unsigned char readReg(unsigned char reg)
{
  initSPI();
  writeOut(0x81 | (reg << 1));
  stopSPI();
  return readIn();
}

void writeReg(unsigned char reg, unsigned char value)
{
  initSPI();
  writeOut(0x80 | (reg << 1));
  writeOut(value);
  stopSPI();
}

void setup()
{
  pinMode(cs, OUTPUT);
  digitalWrite(cs, HIGH);
  pinMode(io, OUTPUT);
  pinMode(clk, OUTPUT);
  Serial.begin(9600);
  writeReg(7, 0);
  writeReg(0, readReg(0) & ~(1 << 7));
  
}

void loop()
{
  initSPI();
  writeReg(0xBF, 0);
  for(int i=0; i<7; i++)
  {
    Serial.print(readIn());
    Serial.print(" ");
  }
  Serial.println();
  stopSPI();
  delay(1000);
}

