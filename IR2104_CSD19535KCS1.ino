// PWM pins for HIN and LIN
int hinPin = 9;
int linPin = 10;

// Potentiometer pin
int potPin = A0;

void setup() {
  // Set PWM pins as output
  pinMode(hinPin, OUTPUT);
  pinMode(linPin, OUTPUT);

  // Set up PWM frequency on pins 9 and 10
  analogWriteFrequency(9, 15000);  // Set 15kHz frequency
  analogWriteFrequency(10, 15000);
}

void loop() {
  // Read the potentiometer value (0-1023)
  int potValue = analogRead(potPin);

  // Map potentiometer value to PWM duty cycle (0-255)
  int pwmValue = map(potValue, 0, 1023, 0, 255);

  // Write PWM to HIN and LIN pins (control high-side and low-side MOSFETs)
  analogWrite(hinPin, pwmValue);  // High-side MOSFET control
  analogWrite(linPin, 255 - pwmValue);  // Low-side MOSFET control (inverse)
  
  delay(10);  // Short delay for stability
}
