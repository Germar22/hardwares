#include <WiFi.h>
#include <HTTPClient.h>

#define MQ2_PIN 34     // Define GPIO pin for MQ2 sensor
#define LED_PIN 2      // Define digital pin for LED (can be changed to any suitable pin)

const char WIFI_SSID[] = "GlobeAtHome_E9602";
const char WIFI_PASSWORD[] = "bundafamily";

String HOST_NAME = "http://afas.atwebpages.com"; // change to your PC's IP address
String PATH_NAME   = "/dashboard/db/alert.php";
String queryString = "?";
String coordinates = "latitude=6.07776&longitude=125.13088&alert_time=2024-03-15%2012%3A00%3A00&fbclid=IwAR3-vStEmecQRHnqx0tEumiufVSq_hyZ14xLRMbq6b1RN-cCI-Fa6e_iQNY";

const int SMOKE_THRESHOLD = 140; // Define threshold value for smoke detection

unsigned long startTime = 0; // Variable to store the start time
bool ledOn = false; // Flag to indicate if LED is on

void setup() {
  Serial.begin(9600);   // Start serial communication at 9600 bps
  pinMode(LED_PIN, OUTPUT); // Set LED pin as output

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  int sensorValue = analogRead(MQ2_PIN); // Read analog value from MQ2 sensor

  // Send the sensor value to the serial plotter
  Serial.println(sensorValue);

  // Check if the value exceeds the threshold
  if (sensorValue > SMOKE_THRESHOLD) {
    if (!ledOn) {
      startTime = millis(); // Record the start time
      digitalWrite(LED_PIN, HIGH); // Turn on LED
      ledOn = true; // Set LED flag to true
      sendHTTPRequest(); // Send HTTP request
    }
  } else {
    // Check if the LED has been on for at least 5 seconds
    if (ledOn && (millis() - startTime >= 5000)) {
      digitalWrite(LED_PIN, LOW); // Turn off LED
      ledOn = false; // Set LED flag to false
    }
  }

  delay(100); // Delay for 0.1 second before taking the next reading
}

void sendHTTPRequest() {
  HTTPClient http;

  http.begin(HOST_NAME + PATH_NAME + queryString + coordinates); //HTTP
  int httpCode = http.GET();

  // httpCode will be negative on error
  if(httpCode > 0) {
    // file found at server
    if(httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      Serial.println(payload);
    } else {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);
    }
  } else {
    Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }

  http.end();
}

