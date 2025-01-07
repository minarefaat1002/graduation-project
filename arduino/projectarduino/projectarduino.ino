#include <ESP8266WiFi.h>
#include <WebSocketsClient.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

// WebSocket Server details
const char* server = "192.168.1.15";
const int port = 80; // Change if necessary

// WiFi credentials
const char* ssid = "mina";
const char* password = "11111111";

// GPS setup
SoftwareSerial ss(4, 5); // RX, TX
TinyGPSPlus gps;

// WebSocket setup
WebSocketsClient webSocket;

// Time tracking for speed calculation
unsigned long lastTime = 0;
float lastLat = 0.0, lastLon = 0.0;

// Function to calculate distance using Haversine formula
float haversine(float lat1, float lon1, float lat2, float lon2) {
  float R = 6371.0; // Earth's radius in kilometers
  float dLat = (lat2 - lat1) * DEG_TO_RAD;
  float dLon = (lon2 - lon1) * DEG_TO_RAD;
  lat1 = lat1 * DEG_TO_RAD;
  lat2 = lat2 * DEG_TO_RAD;

  float a = sin(dLat / 2) * sin(dLat / 2) +
            sin(dLon / 2) * sin(dLon / 2) * cos(lat1) * cos(lat2);
  float c = 2 * atan2(sqrt(a), sqrt(1 - a));
  return R * c; // Distance in kilometers
}

// Function to calculate speed (in km/h)
float calculateSpeed(float distance, unsigned long timeDiff) {
  if (timeDiff == 0) return 0;
  return (distance / (timeDiff / 1000.0)) * 3600.0; // speed in km/h
}

void setup() {
  Serial.begin(115200);
  ss.begin(9600); // Start GPS serial at 9600 baud

  // Connect to Wi-Fi
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(5000);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");

  // Connect to WebSocket server
  webSocket.begin(server, port, "/"); // WebSocket URL path if necessary
  webSocket.onEvent(webSocketEvent);

  delay(1000);  // Wait for WebSocket to establish
}

void loop() {
  while (ss.available() > 0) {
    gps.encode(ss.read());
    if (gps.location.isUpdated()) {
      float lat = gps.location.lat();
      float lon = gps.location.lng();

      unsigned long currentTime = millis();
      float distance = haversine(lastLat, lastLon, lat, lon);
      float speed = calculateSpeed(distance, currentTime - lastTime);

      // Send GPS data and speed via WebSocket
      String message = String("Latitude: ") + lat + ", Longitude: " + lon + ", Speed: " + speed + " km/h";
      webSocket.sendTXT(message);

      // Update last position and time for speed calculation
      lastLat = lat;
      lastLon = lon;
      lastTime = currentTime;
    }
  }

  // Handle WebSocket events
  webSocket.loop();
}

// WebSocket event handler
void webSocketEvent(WStype_t type, uint8_t* payload, size_t length) {
  switch (type) {
    case WStype_DISCONNECTED:
      Serial.println("Disconnected from WebSocket server");
      break;
    case WStype_CONNECTED:
      Serial.println("Connected to WebSocket server");
      break;
    case WStype_TEXT:
      Serial.printf("Received message: %s\n", payload);
      break;
    case WStype_BIN:
      Serial.println("Received binary message");
      break;
  }
}
