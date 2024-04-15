#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char* WIFI_SSID = "realme";
const char* WIFI_PASSWORD = "123987654";
const int buttonPin = D5; // Pin connected to the push button
const int ledPin = 2;    // Pin connected to the LED

String HOST_NAME = "http://afas.atwebpages.com";
String PATH_NAME = "/dashboard/db/alert.php?";
String LATITUDE = "latitude=";
String LATITUDE_VALUE = "6.07329";
String LONGITUDE = "&longitude=";
String LONGITUDE_VALUE = "125.13147";
String LABEL = "&label=";
String LABEL_VALUE = "Placeda,%20Calumpang";
String BARANGAY_CODE = "&barangay_code=";
String BARANGAY_CODE_VALUE = "13";

int previousButtonState = HIGH;

void setup() {
  Serial.begin(115200);
  pinMode(buttonPin, INPUT_PULLUP); // Set button pin as input with internal pull-up resistor
  pinMode(ledPin, OUTPUT);          // Set LED pin as output

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println("Connecting to WiFi...");

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("\nConnected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void sendHTTPRequest() {
  HTTPClient http;

  WiFiClient client;
  String url = HOST_NAME + PATH_NAME + LATITUDE + LATITUDE_VALUE + LONGITUDE + LONGITUDE_VALUE + LABEL + LABEL_VALUE + BARANGAY_CODE + BARANGAY_CODE_VALUE;

  http.begin(client, url);

  int httpCode = http.GET();

  if (httpCode > 0) {
    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      Serial.println(payload);
    } else {
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);
    }
  } else {
    Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }

  http.end();
}

void loop() {
  int currentButtonState = digitalRead(buttonPin);

  if (currentButtonState == LOW && previousButtonState == HIGH) {
    digitalWrite(ledPin, !digitalRead(ledPin));
    sendHTTPRequest();
    Serial.println("Button pressed");
  }

  previousButtonState = currentButtonState;

  delay(50);
}
