#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#define SOUND_VELOCITY 0.034
#define CM_TO_INCH 0.393701

WiFiClient client;
const char* ssid = "BBVERMA";
const char* password = "bb02061986";
const char* serverName = "http://smartvessel.000webhostapp.com/post-data.php";
String apiKeyValue = "12345";

int trigPin = 12;
int echoPin = 14;
int gasPin = A0;

long duration;
float distanceCm;
float distanceInch;


unsigned long previousMillis = 0;   // Stores the previous timestamp
const long interval = 30000;        // Interval for sending data (milliseconds)

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(gasPin, INPUT);
  Serial.begin(9600);
  connectToWiFi();
}

void loop() {
  unsigned long currentMillis = millis();  // Get current time
  float gasValue = analogRead(gasPin);
  int containerClosed = checkContainerClosed();
  Serial.println(containerClosed);
  if (WiFi.status() == WL_CONNECTED) {
      sendData(gasValue, containerClosed); // Pass current time as timestamp
  } else {
      Serial.println("WiFi Disconnected");
      connectToWiFi(); // Reconnect to WiFi if disconnected
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

int checkContainerClosed() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distanceCm = duration * SOUND_VELOCITY/2;
  Serial.print("Distance: " + String(distanceCm) + " cm");

   // Calculate distance in cm
  // If distance is less than or equal to 8cm, container is closed
  return (distanceCm);
  delay(1000);
}

void sendData(float gasValue, int containerClosed) {
  Serial.println("Sending data to server...");
  Serial.print("Gas Reading: ");
  Serial.println(gasValue);
  Serial.println(containerClosed);
  const char *checkContainer;
  if (containerClosed < 10) {
    Serial.println("Container is closed");
    checkContainer = "false";
  }
  else {
    Serial.println("Container is open");
    checkContainer = "true";
  }
  Serial.print("Container Closed: ");
  Serial.println(containerClosed ? "Yes" : "No");

  HTTPClient http;
  http.begin(client, serverName);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  // Prepare HTTP POST request data
  String httpRequestData = "api_key=" + apiKeyValue + "&gas_reading=" + String(gasValue) + "&container_closed=" + String(checkContainer);

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
