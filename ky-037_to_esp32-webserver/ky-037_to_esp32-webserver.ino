#include <WiFi.h>
#include <HTTPClient.h>

const char WIFI_SSID[] = "GlobeAtHome_E9602";
const char WIFI_PASSWORD[] = "bundafamily";
const int soundPin = 34;   // GPIO pin connected to the sound sensor
const int ledPin = 2;      // ESP32 onboard LED pin
int soundVal;              // sound sensor readings

bool thresholdExceeded = false;  // Flag to indicate if threshold is exceeded
unsigned long startTime = 0;     // Variable to store the start time

String HOST_NAME = "http://afas.atwebpages.com"; // change to your PC's IP address
String PATH_NAME   = "/dashboard/db/alert.php";
String queryString = "?coordinates";
String coordinates = "=6.07775&longitude=125.13093&alert_time=2024-03-15%2012%3A00%3A00&fbclid=IwAR3-vStEmecQRHnqx0tEumiufVSq_hyZ14xLRMbq6b1RN-cCI-Fa6e_iQNY";

void setup() {
  pinMode(soundPin, INPUT);
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
    
    // Send coordinates to the web server
    HTTPClient http;
    http.begin(HOST_NAME + PATH_NAME + queryString + coordinates); //HTTP
    int httpCode = http.GET();

    // httpCode will be negative on error
    if (httpCode > 0) {
      // file found at server
      if (httpCode == HTTP_CODE_OK) {
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
    
  } else if (thresholdExceeded && millis() - startTime >= 7000) {
    digitalWrite(ledPin, LOW);  // Turn OFF ESP32 onboard LED after 2 seconds below threshold
    thresholdExceeded = false; // Reset the flag
  } else if (!thresholdExceeded && millis() - startTime >= 2000) {
    digitalWrite(ledPin, LOW);  // Turn OFF ESP32 onboard LED if below threshold for 2 seconds
  }
}
