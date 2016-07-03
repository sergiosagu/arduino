
// arduino digital pins

const byte SCLK = 8;
const byte DATA = 9;
const byte RSET = 10;

// MAGIC NUMBERS

const byte BRIGHTNESS_MASK = B11100000;
const byte COUNTER_MASK    = B11000000;
const byte POINTER_MASK    = B10100000;

const byte COUNTER_CONTROL = COUNTER_MASK | 0;  // # of chars (0 = all)
const byte POINTER_CONTROL = POINTER_MASK | 15; // start from the left

const byte RESET_DELAY = 2;  // millis
const byte CHAR_DELAY = 125; // millis

const byte BIT_DELAY = 2;   // micros
const byte HIGH_DELAY = 10; // micros
const byte LOW_DELAY = 5;   // micros

// CONSTANTS

const byte BRIGHTNESS_MIN = 0;  // 0 = 0%, 1 = 3%
const byte BRIGHTNESS_MAX = 25; // 25 = 80%, 31 = 100%

const String BLANK_CHAR    = " ";
const String BLANK_DISPLAY = "                ";

const byte DISPLAY_SIZE = 16;

const byte SLIDE_DELAY = 125; // millis

const byte FADE_DELAY  = 10;  // millis

const byte BLINK_DELAY = 250; // millis

const byte CRACK_DELAY = 50;  // millis
const byte CRACK_NOISE = 4;   // # of rounds per char

const byte TERM_DELAY  = 125; // millis
const byte TERM_BLINK  = 4;   // # of cursor blinks

/*
 * Main
 */

void setup() {
  pinMode(SCLK, OUTPUT);
  pinMode(DATA, OUTPUT);
  pinMode(RSET, OUTPUT);

  reset();
  writeByte(COUNTER_CONTROL);
  writeByte(POINTER_CONTROL);
  setBrightness(BRIGHTNESS_MAX);

  randomSeed(analogRead(0));
}

void loop() {
  demo();
}

void demo() {
  String txt = "Quick Demo";
  String msg = alignCenter(txt);

  pause(1);

  fadeIn(msg);
  print(msg, 2000, BRIGHTNESS_MAX);
  fadeOut(msg);
  pause(1);

  slideLeft(txt);
  slideRight(txt);
  pause(1);

  for (int i = 0; i < 5; i++) fadeBlink(alignLeft(txt));
  pause(1);

  for (int i = 0; i < 5; i++) blink(alignRight(txt));
  pause(1);

  crack(msg);
  print(msg, 2000, BRIGHTNESS_MAX);
  pause(1);

  term(txt);
  pause(1);
}

/*
 * Display effects
 */

void print(String msg, int msgDelay, byte brightness) {
  setBrightness(brightness);
  for (int i = 0; i < DISPLAY_SIZE; i++) {
    writeChar(msg.charAt(i));
  }
  delay(msgDelay);
}

void fadeIn(String msg) {
  for (int i = 0; i <= BRIGHTNESS_MAX; i++) {
    print(msg, FADE_DELAY, i);
  }
}

void fadeOut(String msg) {
  for (int i = BRIGHTNESS_MAX; i >= 0; i--) {
    print(msg, FADE_DELAY, i);
  }
}

void fadeBlink(String msg) {
  fadeIn(msg);
  fadeOut(msg);
}

void blink(String msg) {
  print(msg, BLINK_DELAY, BRIGHTNESS_MIN);
  print(msg, BLINK_DELAY, BRIGHTNESS_MAX);
}

void slideLeft(String msg) {
  msg = concat(BLANK_DISPLAY, msg);
  int size = msg.length();
  for (int i = 0; i < size; i++) {
    String txt = "";
    for (int j = 0; j < DISPLAY_SIZE; j++) {
      txt = concat(txt, String(msg.charAt((i + j) % size)));
    }
    print(txt, SLIDE_DELAY, BRIGHTNESS_MAX);
  }
}

void slideRight(String msg) {
  msg = concat(msg, BLANK_DISPLAY);
  int size = msg.length();
  for (int i = size - 1; i >= 0; i--) {
    String txt = "";
    for (int j = 0; j < DISPLAY_SIZE; j++) {
      txt = concat(String(msg.charAt(((i - j) + size) % size)), txt);
    }
    print(txt, SLIDE_DELAY, BRIGHTNESS_MAX);
  }
}

void crack(String msg) {

  boolean mask[DISPLAY_SIZE] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  // initial cracking noise
  for (int n = 0; n < CRACK_NOISE; n++) {
    print(randomText(DISPLAY_SIZE), CRACK_DELAY, BRIGHTNESS_MAX);
  }

  for (int i = 0; i < DISPLAY_SIZE; i++) {
    // pick the next char to crack
    boolean set = false;
    while (!set) {
      byte pos = random(0, DISPLAY_SIZE);
      if (mask[pos] == 0) {
        mask[pos] = 1;
        set = true;
      }
    }
    // cracking process with already cracked chars
    for (int n = 0; n < CRACK_NOISE; n++) {
      String txt = "";
      for (int j = 0; j < DISPLAY_SIZE; j++) {
        if (mask[j] == 0) {
          txt += randomChar();
        } else {
          txt += msg.charAt(j);
        }
      }
      print(txt, CRACK_DELAY, BRIGHTNESS_MAX);
    }
  }
}

void term(String msg) {

  // initial blinks
  for (int i = 0; i < TERM_BLINK; i++) blink(alignLeft("_"));

  int size = msg.length() + 1;
  if (size > DISPLAY_SIZE) {
    size++; // one more loop cycle for the final cursor
  }

  String txt;
  byte cursorAt = 0;

  for (int i = 0; i < size; i++) {
    txt = "";
    for (int j = 0; j < min(DISPLAY_SIZE - 1, i); j++) {
      if (i < DISPLAY_SIZE) {
        txt += msg.charAt(j);
      } else {
        txt += msg.charAt(j + i - DISPLAY_SIZE);
      }
    }
    txt += "_";
    cursorAt = txt.length() - 1;
    txt += blankText(max(0, DISPLAY_SIZE - (i + 1)));
    print(txt, TERM_DELAY, BRIGHTNESS_MAX);
  }

  // final blinks
  for (int i = 0; i < (TERM_BLINK * 2); i++) {
    txt.setCharAt(cursorAt, ((i % 2 == 0) ? ' ' : '_'));
    print(txt, BLINK_DELAY, BRIGHTNESS_MAX);
  }
}

void pause(byte seconds) {
  print(BLANK_DISPLAY, seconds * 1000, BRIGHTNESS_MIN);
}

/*
 * String manipulation
 */

String alignCenter(String txt) {
  int size = txt.length();
  if (size >= DISPLAY_SIZE) {
    return txt;
  }
  byte n = (DISPLAY_SIZE - size) / 2;
  if ((DISPLAY_SIZE - size) % 2 == 0) {
    txt = concat(blankText(n), txt);
  } else {
    txt = concat(blankText(n + 1), txt);
  }
  return concat(txt, blankText(n));
}

String alignRight(String txt) {
  int size = txt.length();
  if (size >= DISPLAY_SIZE) {
    return txt;
  }
  return concat(blankText(DISPLAY_SIZE - size), txt);
}

String alignLeft(String txt) {
  int size = txt.length();
  if (size >= DISPLAY_SIZE) {
    return txt;
  }
  return concat(txt, blankText(DISPLAY_SIZE - size));
}

String alignJustify(String txt) {
  int size = txt.length();
  if (size >= DISPLAY_SIZE) {
    return txt;
  }
  if (txt.indexOf(BLANK_CHAR) != txt.lastIndexOf(BLANK_CHAR)) {
    // justifies only two words separated by one single blank!
    return txt;
  }
  txt.replace(BLANK_CHAR, blankText(DISPLAY_SIZE - size + 1));
  return txt;
}

String concat(String s1, String s2) {
  return s1 + s2;
}

char randomChar() {
  char c = random(65, 126); // A to Z + a to z + 10 digits
  if ((c > 90) && (c < 97)) {
    c = (c - 91) + 48; // digits 0 to 5    
  } else if (c > 122) {
    c = (c - 123) + 54; // digits 6 to 9
  }
  return c;
}

String randomText(byte size) {
  String txt = "";
  for (int i = 0; i < size; i++) {
    txt += randomChar();
  }
  return txt;
}

String blankText(byte size) {
  String txt = "";
  for (int i = 0; i < size; i++) {
    txt += BLANK_CHAR;
  }
  return txt;
}

/*
 * Helpers
 */

byte currentBrightness = -1;

void setBrightness(byte brightness) {
  if (currentBrightness != brightness) {
    writeByte(BRIGHTNESS_MASK | brightness);
    currentBrightness = brightness;
  }
}

void writeChar(char aChar) {
  writeByte(getChar(aChar));
}

char getChar(char c) {
  // direct ASCII mapping
  if ((c >= 32) && (c <= 127)) {
    return c;
  }
  return 63; // '?'
}

void writeByte(byte aByte) {
  for (int i = 0; i < 8; i++) {
    writeBit(bitRead(aByte, 7 - i));
  }
}

void writeBit(boolean aBit) {
  digitalWrite(DATA, aBit ? HIGH : LOW);
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

