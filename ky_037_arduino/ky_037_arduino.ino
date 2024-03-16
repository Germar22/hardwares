const int soundPin = A0;   // KY-037 sound sensor analog interface
const int ledPin = 13;     // Arduino LED pin
int soundVal;              // sound sensor readings

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
  if (soundVal > 22) {
    digitalWrite(ledPin, HIGH); // Turn ON Arduino's LED
    delay(100); // Wait for a short duration
    digitalWrite(ledPin, LOW);  // Turn OFF Arduino's LED
  }
}
