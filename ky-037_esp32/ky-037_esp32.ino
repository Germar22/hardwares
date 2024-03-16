const int soundPin = 34;   // GPIO pin connected to the sound sensor
const int ledPin = 2;      // ESP32 onboard LED pin
int soundVal;              // sound sensor readings

bool thresholdExceeded = false;  // Flag to indicate if threshold is exceeded
unsigned long startTime = 0;     // Variable to store the start time

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

  // If sound level is above a threshold, indicate a clap
  if (soundVal > 500 && !thresholdExceeded) {
    startTime = millis(); // Record the start time
    thresholdExceeded = true; // Set the flag to true
  }

  // Check if the threshold was exceeded for 5 seconds
  if (thresholdExceeded && millis() - startTime >= 5000) {
    digitalWrite(ledPin, HIGH); // Turn ON ESP32 onboard LED
  }  else if (!thresholdExceeded && millis() - startTime >= 2000) {
    digitalWrite(ledPin, LOW);  // Turn OFF ESP32 onboard LED if below threshold for 2 seconds
  }
}
//+ -> 3v
//GND -> GND
//AO -> 34
//DO -> 2
