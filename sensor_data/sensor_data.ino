#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <HX711.h> // Library for HX711 load cell amplifier

WiFiClient client;
const char* ssid = "BBVERMA";
const char* password = "bb02061986";
const char* serverName = "http://smartvessel.000webhostapp.com/post-data.php";
String apiKeyValue = "12345";

// Define the pins connected to the HX711 module
#define DOUT_PIN D1  // Data output pin
#define SCK_PIN D2   // Clock pin

HX711 scale;  // Create an instance of the HX711 library

int trigPin = 12; // D6 is the pin number for digital output
int echoPin = 13; // D7 is the pin number for digital output
int gasPin = A0; // A0 is the pin number for analog input

long duration;
float distanceCm;
float distanceInch;

unsigned long previousMillis = 0;   // Stores the previous timestamp
const long interval = 50000;        // Interval for sending data (milliseconds)

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(gasPin, INPUT);
  Serial.begin(9600);
  connectToWiFi();

  // Initialize the HX711 library
  scale.begin(DOUT_PIN, SCK_PIN);
  scale.set_scale();  // Reset the scale to 1.0
  scale.tare();       // Reset the scale to zero
}

void loop() {
  unsigned long currentMillis = millis();  // Get current time
  float gasValue = analogRead(gasPin);
  float containerClosed = checkContainerClosed();

  float Wload = scale.get_units(10);  // Read load cell value 10 times and get average value
  Wload = Wload/1000;
  if (WiFi.status() == WL_CONNECTED) {
    sendData(gasValue, containerClosed, Wload); // Pass current time as timestamp
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

float checkContainerClosed() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distanceCm = duration * 0.034 / 2;
  Serial.print("Distance: ");
  Serial.print(distanceCm);
  Serial.println(" cm");
  // If distance is less than or equal to 8cm, container is closed
  return (distanceCm);
}

void sendData(float gasValue, float containerClosed, float Wload) {
  Serial.println("Sending data to server...");
  Serial.print("Gas Reading: ");
  Serial.println(gasValue);
  Serial.print("Length ");
  Serial.println(containerClosed);
  Serial.print("Load (gms): ");
  Serial.println(Wload);
  const char *checkContainer;
  if (containerClosed <15) {
    Serial.println("Container is closed");
    checkContainer = "close";
  } else {
    Serial.println("Container is open");
    checkContainer = "open";
  }

  HTTPClient http;
  http.begin(client, serverName);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  // Prepare HTTP POST request data
  String httpRequestData = "api_key=" + apiKeyValue + "&gas_reading=" + String(gasValue) + "&container_closed=" + String(checkContainer) + "&Wload=" + String(Wload);

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
