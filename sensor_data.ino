#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

WiFiClient client;
const char* ssid = "BBVERMA";
const char* password = "bb02061986";
const char* serverName = "http://smartvessel.000webhostapp.com/post-data.php";
String apiKeyValue = "12345";

int trigPin = 5;
int echoPin = 4;
int gasPin = A0;

unsigned long previousMillis = 0;   // Stores the previous timestamp
const long interval = 20000;        // Interval for sending data (milliseconds)

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(gasPin, INPUT);
  Serial.begin(9600);
  connectToWiFi();
}

void loop() {
  unsigned long currentMillis = millis();  // Get current time

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;  // Update the previous timestamp

    float gasValue = analogRead(gasPin);
    bool containerClosed = checkContainerClosed();

    if (WiFi.status() == WL_CONNECTED) {
      sendData(gasValue, containerClosed, currentMillis); // Pass current time as timestamp
    } else {
      Serial.println("WiFi Disconnected");
      connectToWiFi(); // Reconnect to WiFi if disconnected
    }
  }

  // Adjust delay as needed
  delay(100);
}

void connectToWiFi() {
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected!");
}

bool checkContainerClosed() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  float distance = duration / 29.1 / 2; // Calculate distance in cm

  // If distance is less than or equal to 5cm, container is closed
  return (distance <= 5);
}

void sendData(float gasValue, bool containerClosed, unsigned long timestamp) {
  HTTPClient http;
  http.begin(client, serverName);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  // Prepare HTTP POST request data
  String httpRequestData = "api_key=" + apiKeyValue + "&gas_reading=" + String(gasValue) + "&container_closed=" + String(containerClosed) + "&timestamp=" + String(timestamp);

  Serial.print("Sending HTTP POST request: ");
  Serial.println(httpRequestData);

  int httpResponseCode = http.POST(httpRequestData);

  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
  } else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }

  http.end();
}
