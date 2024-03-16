const int soundPin = 34;   // GPIO pin connected to the sound sensor
const int ledPin = 2;      // ESP32 onboard LED pin
int soundVal;              // sound sensor readings
unsigned long startTime = 0; // Variable to store the start time
bool thresholdExceeded = false; // Flag to indicate if threshold is exceeded

void setup() {
  pinMode(soundPin, INPUT);
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // Read the sound sensor
  soundVal = analogRead(soundPin);

  // Print sound value to serial
  Serial.println(soundVal);

  // Check if sound level exceeds threshold and if threshold hasn't been exceeded before
  if (soundVal > 500 && !thresholdExceeded) {
    startTime = millis(); // Record the start time
    thresholdExceeded = true; // Set the flag to true
    digitalWrite(ledPin, HIGH); // Turn ON LED
  }

  // Check if threshold was exceeded within 5 seconds
  if (thresholdExceeded && millis() - startTime >= 5000) {
    digitalWrite(ledPin, LOW); // Turn OFF LED
    thresholdExceeded = false; // Reset threshold exceeded flag
  }

  // Check if sound level falls below threshold after LED has been turned off
  if (!thresholdExceeded && millis() - startTime >= 2000) {
    if (soundVal <= 500) {
      digitalWrite(ledPin, LOW); // Turn OFF LED
    }
  }
}


//+ -> 3v
//GND -> GND
//AO -> 34
//DO -> 2
