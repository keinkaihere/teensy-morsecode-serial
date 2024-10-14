// This is the LED on the board, useful for status
const int blinkPin = 13;
// This is the LED used for outputting data
const int outputLED = 6;

/*
 * Binary Morse Code Timing:
 * - Treats 4 bits (a nibble) as one word in morse code
 * - 0 is dot  (1 time unit), .
 * - 1 is dash (3 time unit), -
 * - 1 time unit between each symbol in the nibble
 * - 3 time units between nibbles
 * - Starts at MSB, so sequence 0110 1011 would be .--. -.--
 * So for example, .--. is 1 + 1 + 3 + 1 + 3 + 1 + 1 + 1 = 13 TU
 * TU is msec value, so giving it value 250 means that .--. takes 3.25sec
*/
const int TU = 250;

// Blinks status once, then outputs two nibbles. 'x' which is x78 is 0111 1000, .--- -...
void morsecodeByte(byte c) {
  char nibble1 = c & 0xF0;
  char nibble2 = (c & 0x0F) << 4;

  digitalWrite(blinkPin, HIGH);
  digitalWrite(outputLED, LOW);
  delay(TU);
  digitalWrite(blinkPin, LOW);
  delay(TU);

  for(int i = 0; i < 4; i++) {
    // get MSB of byte
    bool symbol = nibble1 & 0x80;
    // shift byte left 1 for next loop
    nibble1 = nibble1 << 1;
    // output symbol then output spacing
    digitalWrite(outputLED, HIGH);
    delay(TU*(1 + 2*symbol));
    digitalWrite(outputLED, LOW);
    delay(TU);
  }
  // all of the above end in 1 TU off delay so 2TU more makes 3TU
  delay(2*TU);
  for(int i = 0; i < 4; i++) {
    // get MSB of byte
    bool symbol = nibble2 & 0x80;
    // shift byte left 1 for next loop
    nibble2 = nibble2 << 1;
    // output symbol then output spacing
    digitalWrite(outputLED, HIGH);
    delay(TU*(1 + 2*symbol));
    digitalWrite(outputLED, LOW);
    delay(TU);
  }
}

void setup() {
  // initialize the digital pin as an output.
  pinMode(blinkPin, OUTPUT);
  pinMode(outputLED, OUTPUT);

  // initialize serial connection btw teensy and host machine
  Serial.begin(9600);
  while (!Serial) {
    // wait for Serial Monitor to be ready
  }
  Serial.println("Serial Ready");
}

void loop() {
  if (Serial.available()) {
    char incomingByte = Serial.read();  // will not be -1
    // actually do something with incomingByte
    Serial.println(incomingByte, BIN);
    morsecodeByte(incomingByte);
  }
}

