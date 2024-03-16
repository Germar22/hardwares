const int soundPin = 34;   // GPIO pin connected to the sound sensor
const int ledPin = 2;      // ESP32 onboard LED pin
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
  if (soundVal > 120) {
    digitalWrite(ledPin, HIGH); // Turn ON ESP32 onboard LED
    delay(100); // Wait for a short duration
    digitalWrite(ledPin, LOW);  // Turn OFF ESP32 onboard LED
  }
}

//+ -> 3v
//GND -> GND
//AO -> 34
//DO -> 2
