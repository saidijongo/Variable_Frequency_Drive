// PWM pins for HIN1, LIN1, HIN2, LIN2 (IR2104 inputs)
int hinPin1 = 9;   // Left high-side MOSFET
int linPin1 = 10;  // Left low-side MOSFET
int hinPin2 = 11;  // Right high-side MOSFET
int linPin2 = 12;  // Right low-side MOSFET

// Potentiometer pin for speed control
int potPin = A0;

void setup() {
  // Set PWM pins as output
  pinMode(hinPin1, OUTPUT);
  pinMode(linPin1, OUTPUT);
  pinMode(hinPin2, OUTPUT);
  pinMode(linPin2, OUTPUT);

  // Set up Timer1 for 15kHz PWM on pins 9 and 10
  setupPWM_15kHz();
  
  // Set up Timer2 for 15kHz PWM on pins 11 and 12
  setupPWM_15kHz_Timer2();
}

void loop() {
  // Read the potentiometer value (0 to 1023)
  int potValue = analogRead(potPin);

  // Map the potentiometer value to PWM duty cycle (0 to 255)
  int pwmValue = map(potValue, 0, 1023, 0, 255);

  // Write the PWM signals to HIN and LIN pairs for both half-bridges
  analogWrite(hinPin1, pwmValue);       // Left high-side MOSFET
  analogWrite(linPin1, 255 - pwmValue); // Left low-side MOSFET (inverse)

  analogWrite(hinPin2, pwmValue);       // Right high-side MOSFET
  analogWrite(linPin2, 255 - pwmValue); // Right low-side MOSFET (inverse)
  
  delay(10);  // Small delay for stability
}

// Timer1 setup for pins 9 and 10 (HIN1 and LIN1)
void setupPWM_15kHz() {
  // Configuring Timer 1 for 15kHz PWM on pins 9 and 10 (using 8-bit Fast PWM mode)
  TCCR1A = _BV(WGM10) | _BV(COM1A1) | _BV(COM1B1); // Set Fast PWM mode (8-bit) and enable PWM on OC1A (pin 9) and OC1B (pin 10)
  TCCR1B = _BV(WGM12) | _BV(CS10);  // No prescaling (CS10), 15.625 kHz frequency with 16MHz clock
  OCR1A = 0;  // Initial duty cycle for HIN1 (pin 9)
  OCR1B = 0;  // Initial duty cycle for LIN1 (pin 10)
}

// Timer2 setup for pins 11 and 12 (HIN2 and LIN2)
void setupPWM_15kHz_Timer2() {
  // Configuring Timer 2 for 15kHz PWM on pins 11 and 12 (using 8-bit Fast PWM mode)
  TCCR2A = _BV(WGM20) | _BV(COM2A1) | _BV(COM2B1); // Set Fast PWM mode (8-bit) and enable PWM on OC2A (pin 11) and OC2B (pin 12)
  TCCR2B = _BV(WGM22) | _BV(CS20);  // No prescaling, ~15kHz PWM
  OCR2A = 0;  // Initial duty cycle for HIN2 (pin 11)
  OCR2B = 0;  // Initial duty cycle for LIN2 (pin 12)
}
