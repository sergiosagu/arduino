
// arduino digital pins

int SCLK = 2;
int DATA = 3;
int RSET = 4;

// MAGIC NUMBERS

boolean COUNTER_CONTROL[8] = { 1, 1, 0, 0, 0, 0, 0, 0 }; // use all 16 chars
boolean BRIGHT_CONTROL[8]  = { 1, 1, 1, 1, 1, 1, 1, 1 }; // 100% brightness
boolean POINTER_CONTROL[8] = { 1, 0, 1, 0, 1, 1, 1, 1 }; // start from the left

boolean CHAR_H[8]  = { 0, 0, 0, 0, 1, 0, 0, 0 };
boolean CHAR_E[8]  = { 0, 0, 0, 0, 0, 1, 0, 1 };
boolean CHAR_L[8]  = { 0, 0, 0, 0, 1, 1, 0, 0 };
boolean CHAR_O[8]  = { 0, 0, 0, 0, 1, 1, 1, 1 };
boolean CHAR__[8]  = { 0, 0, 1, 0, 0, 0, 0, 0 };
boolean CHAR_W[8]  = { 0, 0, 0, 1, 0, 1, 1, 1 };
boolean CHAR_R[8]  = { 0, 0, 0, 1, 0, 0, 1, 0 };
boolean CHAR_D[8]  = { 0, 0, 0, 0, 0, 1, 0, 0 };

int RESET_DELAY = 2; // millis
int CHAR_DELAY = 100; // millis

int BIT_DELAY = 2; // micros
int HIGH_DELAY = 10; // micros
int LOW_DELAY = 5; // micros

void setup() {                
  pinMode(SCLK, OUTPUT);     
  pinMode(DATA, OUTPUT);     
  pinMode(RSET, OUTPUT); 
  Serial.begin(9600);

  reset();
  writeByte(COUNTER_CONTROL);
  writeByte(BRIGHT_CONTROL);
  writeByte(POINTER_CONTROL);
}

void loop() {
  writeChar(CHAR_H);
  writeChar(CHAR_E);
  writeChar(CHAR_L);
  writeChar(CHAR_L);
  writeChar(CHAR_O);
  writeChar(CHAR__);
  writeChar(CHAR_W);
  writeChar(CHAR_O);
  writeChar(CHAR_R);
  writeChar(CHAR_L);
  writeChar(CHAR_D);
  writeChar(CHAR__);
}

void writeChar(boolean aByte[]) {
  writeByte(aByte);
  delay(CHAR_DELAY);
}

void writeByte(boolean aByte[]) {
  for (int i = 0; i < 8; i++) {
    writeBit(aByte[i]);
  } 
}

void writeBit(boolean aBit) {
  digitalWrite(DATA, (aBit == 1) ? HIGH : LOW);
  delayMicroseconds(BIT_DELAY);

  digitalWrite(SCLK, HIGH);
  delayMicroseconds(HIGH_DELAY);

  digitalWrite(SCLK, LOW);
  delayMicroseconds(LOW_DELAY);
}

void reset() {
  digitalWrite(DATA, LOW);
  digitalWrite(SCLK, LOW);
  digitalWrite(RSET, LOW);
  delay(RESET_DELAY);
  digitalWrite(RSET, HIGH);
  delay(RESET_DELAY);
}

