#include <DHT.h>

#define DHTPIN 4 // Pin where the DHT11 is connected
#define DHTTYPE DHT11 // Type of the DHT sensor

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();
}

void loop() {
  delay(100); // Wait for 2 seconds between measurements

  float humidity = dht.readHumidity(); // Read humidity
  float temperature = dht.readTemperature(); // Read temperature in Celsius
  
  // Check if any reads failed and exit early (to try again).
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Print data in CSV format
  Serial.print(temperature);
  Serial.print(",");
  Serial.println(humidity);
}

