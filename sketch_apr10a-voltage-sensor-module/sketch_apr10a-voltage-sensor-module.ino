// Define the analog pin connected to the voltage sensor
const int voltageSensorPin = 34; // Assuming the sensor is connected to pin 34

void setup() {
  // Initialize serial communication
  Serial.begin(9600);
}

void loop() {
  // Read the analog voltage from the sensor
  int sensorValue = analogRead(voltageSensorPin);
  
  // Convert the analog reading to voltage (assuming 3.3V reference voltage)
  float voltage = sensorValue * (3.3 / 1023.0);
  
  // Print the voltage value to the serial monitor
  Serial.println(voltage); // Print without any additional text
  
  delay(100); // Delay for readability (adjust as needed)
}

