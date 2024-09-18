// PWM pins for HIN and LIN (IR2104 inputs)
int hinPin = 9;
int linPin = 10;

// Potentiometer pin for speed control
int potPin = A0;

void setup() {
  // Set PWM pins as output
  pinMode(hinPin, OUTPUT);
  pinMode(linPin, OUTPUT);

  // Set up Timer1 for 15kHz PWM on pins 9 and 10
  setupPWM_15kHz();
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

void setupPWM_15kHz() {
  // Configuring Timer 1 for 15kHz PWM on pins 9 and 10 (using 8-bit Fast PWM mode)
  TCCR1A = _BV(WGM10) | _BV(COM1A1) | _BV(COM1B1); // Set Fast PWM mode (8-bit) and enable PWM on OC1A (pin 9) and OC1B (pin 10)
  TCCR1B = _BV(WGM12) | _BV(CS10);  // No prescaling (CS10), 15.625 kHz frequency with 16MHz clock
  OCR1A = 0;  // Initial duty cycle for HIN (pin 9)
  OCR1B = 0;  // Initial duty cycle for LIN (pin 10)
}
