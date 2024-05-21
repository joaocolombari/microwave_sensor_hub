/*
    Code for analog mux controller for the microwave sensor hub project
    Author: Joao Victor Colombari Carlet
    Company: GMETA
    contact: jvccarlet@usp.br
*/

const int bit0Pin = 12;  // LSB
const int bit1Pin = 14;
const int bit2Pin = 27;
const int bit3Pin = 26;  // MSB
const int buttonPin = 4;  // Button input pin

const int segmentPins[] = {2, 3, 4, 5, 6, 7, 8}; // Segment pins for the 7-segment display (a to g)

// Segment bitmaps for common-cathode 7-segment display (0-9)
const byte digitBitmaps[10] = {
  0b00111111, // 0
  0b00000110, // 1
  0b01011011, // 2
  0b01001111, // 3
  0b01100110, // 4
  0b01101101, // 5
  0b01111101, // 6
  0b00000111, // 7
  0b01111111, // 8
  0b01101111  // 9
};

int count = 0;
int lastButtonState = LOW;
int currentButtonState = LOW;

void setup() {
  pinMode(bit0Pin, OUTPUT);
  pinMode(bit1Pin, OUTPUT);
  pinMode(bit2Pin, OUTPUT);
  pinMode(bit3Pin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);

  for (int i = 0; i < 7; i++) {
    pinMode(segmentPins[i], OUTPUT);
  }
}

void loop() {
  // Read the button state
  currentButtonState = digitalRead(buttonPin);

  // Check if button is pressed (state changes from HIGH to LOW)
  if (currentButtonState == LOW && lastButtonState == HIGH) {
    count = (count + 1) % 16; // Increment count and wrap around at 16
    updateMux(count);
    updateDisplay(count);
  }

  // Save the current button state
  lastButtonState = currentButtonState;
  
  // Debounce delay
  delay(50);
}

void updateMux(int value) {
  digitalWrite(bit0Pin, value & 0x01); // Extract bit 0
  digitalWrite(bit1Pin, (value >> 1) & 0x01); // Extract bit 1
  digitalWrite(bit2Pin, (value >> 2) & 0x01); // Extract bit 2
  digitalWrite(bit3Pin, (value >> 3) & 0x01); // Extract bit 3
}

void updateDisplay(int value) {
  int tens = value / 10;  // Extract tens digit
  int ones = value % 10;  // Extract ones digit

  // Display tens digit
  for (int i = 0; i < 7; i++) {
    digitalWrite(segmentPins[i], (digitBitmaps[tens] >> i) & 0x01);
  }
  delay(5);

  // Display ones digit
  for (int i = 0; i < 7; i++) {
    digitalWrite(segmentPins[i], (digitBitmaps[ones] >> i) & 0x01);
  }
  delay(5);
}
