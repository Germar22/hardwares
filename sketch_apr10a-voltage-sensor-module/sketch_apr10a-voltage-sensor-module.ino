#include <ESPWiFi.h>
#include <HTTPClient.h>

const char WIFI_SSID[] = "realme"; //
const char WIFI_PASSWORD[] = "123987654"; //
const int ledPin = 2;                  // ESP32 onboard LED pin
const int voltageSensorPin = 34;       // Analog pin for voltage sensor
const float VOLTAGE_THRESHOLD = 5.0;   // Set your voltage threshold here (in volts)

unsigned long lastRequestTime = 0;     // Variable to store the last time HTTP request was sent
bool thresholdExceeded = false;        // Flag to indicate if threshold is exceeded

String HOST_NAME = "http://afas.atwebpages.com"; 
String PATH_NAME   = "/dashboard/db/alert.php?";
String LATITUDE             = "latitude=";
String LATITUDE_VALUE       = "6.07329";
String LONGITUDE            = "&longitude=";
String LONGITUDE_VALUE      = "125.13147";
String LABEL                = "&label=";
String LABEL_VALUE          = "Placeda,%20Calumpang";
String BARANGAY_CODE        = "&barangay_code=";
String BARANGAY_CODE_VALUE  = "13";

void sendHTTPRequest() {
  HTTPClient http;
  http.begin(HOST_NAME + PATH_NAME + LATITUDE + LATITUDE_VALUE + LONGITUDE + LONGITUDE_VALUE + LABEL + LABEL_VALUE + BARANGAY_CODE + BARANGAY_CODE_VALUE);
  int httpCode = http.GET();

  // httpCode will be negative on error
  if (httpCode > 0) {
    // file found at server
    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      Serial.println(payload);
    } else {
      // HTTP header has been sent and Server response header has been handled
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);
    }
  } else {
    Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }

  http.end();
}

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);

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
  // Read the analog voltage from the sensor
  int sensorValue = analogRead(voltageSensorPin);
  
  // Convert the analog reading to voltage
  float voltage = sensorValue * (3.3 / 1023.0);

  // Print the voltage value to the serial monitor
  Serial.println(voltage);

  // Check if voltage has exceeded the threshold
  if (voltage > VOLTAGE_THRESHOLD && !thresholdExceeded) {
    thresholdExceeded = true;       // Set the flag to true
    lastRequestTime = millis();     // Record the time
  }

  // Check if the threshold was exceeded for 3 seconds
  if (thresholdExceeded && millis() - lastRequestTime >= 3000) {
    digitalWrite(ledPin, HIGH);     // Turn ON ESP32 onboard LED
    sendHTTPRequest();              // Send HTTP request
    thresholdExceeded = false;      // Reset the flag
  }

  // Add any additional code here if needed

  delay(100); // Adjust delay as needed
}


