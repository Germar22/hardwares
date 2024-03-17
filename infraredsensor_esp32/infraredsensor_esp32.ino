const int IR_PIN = 27; // GPIO pin connected to the OUT pin of the IR sensor

void setup() {
  Serial.begin(9600);
  pinMode(IR_PIN, INPUT);
}

void loop() {
  int sensorValue = digitalRead(IR_PIN);
  
  // Output sensor value to Serial Plotter
  Serial.println(sensorValue);
  
  delay(100); // Adjust delay as needed
}
