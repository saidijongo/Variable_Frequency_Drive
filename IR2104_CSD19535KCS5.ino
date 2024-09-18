// PWM pins for HIN1, LIN1, HIN2, LIN2 (IR2104 inputs)
int hinPin1 = 9;   // Left high-side MOSFET
int linPin1 = 10;  // Left low-side MOSFET
int hinPin2 = 11;  // Right high-side MOSFET
int linPin2 = 12;  // Right low-side MOSFET

// Potentiometer pin for speed control
int potPin = A0;

// Parameters for sine wave generation
const int pwmFrequency = 15000;     // 15kHz PWM frequency
const int sineFrequency = 60;       // 60Hz AC frequency
const int numSamples = 256;         // Number of samples for one sine wave period
int sineTable[numSamples];          // Sine wave lookup table

void setup() {
  // Set PWM pins as output
  pinMode(hinPin1, OUTPUT);
  pinMode(linPin1, OUTPUT);
  pinMode(hinPin2, OUTPUT);
  pinMode(linPin2, OUTPUT);

  // Set up PWM
  setupPWM_15kHz();
  setupPWM_15kHz_Timer2();

  // Initialize sine table for 60Hz waveform (scaled for 8-bit PWM)
  for (int i = 0; i < numSamples; i++) {
    sineTable[i] = int(127.5 * (1 + sin(2 * PI * i / numSamples)));  // 0 to 255 range
  }
}

void loop() {
  // Read the potentiometer value (0 to 1023)
  int potValue = analogRead(potPin);
  int pwmAmplitude = map(potValue, 0, 1023, 0, 255);  // Amplitude scaling factor

  // Generate a 60Hz sine-modulated PWM
  for (int i = 0; i < numSamples; i++) {
    int sineValue = sineTable[i];  // Get sine wave value from lookup table

    // Scale sine value by amplitude from potentiometer
    int modulatedPWM = sineValue * pwmAmplitude / 255;

    // Apply PWM signals to the H-bridge
    analogWrite(hinPin1, modulatedPWM);        // Left high-side MOSFET
    analogWrite(linPin1, 255 - modulatedPWM);  // Left low-side MOSFET

    analogWrite(hinPin2, 255 - modulatedPWM);  // Right high-side MOSFET
    analogWrite(linPin2, modulatedPWM);        // Right low-side MOSFET

    delayMicroseconds(1000 * 16 / numSamples); // Delay for one sample (16.67ms / 256 samples)
  }
}

void setupPWM_15kHz() {
  // Configuring Timer 1 for 15kHz PWM on pins 9 and 10 (using 8-bit Fast PWM mode)
  TCCR1A = _BV(WGM10) | _BV(COM1A1) | _BV(COM1B1); // Set Fast PWM mode (8-bit) and enable PWM on OC1A (pin 9) and OC1B (pin 10)
  TCCR1B = _BV(WGM12) | _BV(CS10);  // No prescaling (CS10), 15.625 kHz frequency with 16MHz clock
  OCR1A = 0;  // Initial duty cycle for HIN1 (pin 9)
  OCR1B = 0;  // Initial duty cycle for LIN1 (pin 10)
}

void setupPWM_15kHz_Timer2() {
  // Configuring Timer 2 for 15kHz PWM on pins 11 and 12 (using 8-bit Fast PWM mode)
  TCCR2A = _BV(WGM20) | _BV(COM2A1) | _BV(COM2B1); // Set Fast PWM mode (8-bit) and enable PWM on OC2A (pin 11) and OC2B (pin 12)
  TCCR2B = _BV(WGM22) | _BV(CS20);  // No prescaling, ~15kHz PWM
  OCR2A = 0;  // Initial duty cycle for HIN2 (pin 11)
  OCR2B = 0;  // Initial duty cycle for LIN2 (pin 12)
}
