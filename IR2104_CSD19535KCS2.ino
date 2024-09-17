// PWM pins for HIN and LIN (IR2104 inputs)

int hinPin = 9;
int linPin = 10;

// Potentiometer pin for speed control
int potPin = A0;

void setup() {
  // Set PWM pins as output
  pinMode(hinPin, OUTPUT);
  pinMode(linPin, OUTPUT);

  // Set up PWM frequency (15kHz)
  analogWriteFrequency(9, 15000);  // 15kHz PWM for HIN (high-side)
  analogWriteFrequency(10, 15000); // 15kHz PWM for LIN (low-side)
}

void loop() {
  // Read the potentiometer value (0 to 1023)
  int potValue = analogRead(potPin);

  // Map the potentiometer value to PWM duty cycle (0 to 255)
  int pwmValue = map(potValue, 0, 1023, 0, 255);

  // Write the PWM signal to HIN and LIN to control high-side and low-side MOSFETs
  analogWrite(hinPin, pwmValue);      // Control high-side MOSFET
  analogWrite(linPin, 255 - pwmValue); // Control low-side MOSFET (inverse)
  
  delay(10);  // Small delay for stability
}
