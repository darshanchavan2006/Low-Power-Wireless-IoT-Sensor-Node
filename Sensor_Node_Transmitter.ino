/*
  Low-Power Wireless IoT Sensor Node
  ----------------------------------
  NodeMCU ESP8266 / ESP-12E + DHT11

  ROLE:
  Sensor transmitter / Wi-Fi Access Point

  DHT11:
    VCC  -> 3V3
    DATA -> D2 (GPIO4)
    GND  -> GND

  The receiver connects to this ESP8266 and requests one
  sensor packet at a time.

  Serial Monitor: 115200 baud
*/

#include <ESP8266WiFi.h>
#include <DHT.h>

#define DHTPIN  D2
#define DHTTYPE DHT11

const char* AP_SSID = "IoT_Sensor_Node";
const char* AP_PASSWORD = "12345678";

DHT dht(DHTPIN, DHTTYPE);
WiFiServer server(80);

unsigned long packetNumber = 0;

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println();
  Serial.println("================================");
  Serial.println("  IoT SENSOR NODE - TRANSMITTER");
  Serial.println("================================");

  dht.begin();

  WiFi.mode(WIFI_AP);
  WiFi.softAP(AP_SSID, AP_PASSWORD);

  Serial.print("Wi-Fi SSID : ");
  Serial.println(AP_SSID);

  Serial.print("AP IP      : ");
  Serial.println(WiFi.softAPIP());

  server.begin();

  Serial.println("Server    : Started");
  Serial.println("Status    : Waiting for receiver...");
  Serial.println("--------------------------------");
}

void loop() {
  WiFiClient client = server.available();

  if (!client) {
    return;
  }

  // Wait briefly for the receiver's HTTP request.
  unsigned long requestStart = millis();
  while (!client.available() && millis() - requestStart < 1000) {
    delay(1);
  }

  // Read and discard the HTTP request.
  while (client.available()) {
    client.read();
  }

  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  packetNumber++;

  bool sensorOK = !isnan(temperature) && !isnan(humidity);

  if (!sensorOK) {
    temperature = -1.0;
    humidity = -1.0;
  }

  // CSV-like payload sent to receiver.
  // Example:
  // PACKET=12,TEMP=27.10,HUM=58.00
  String payload;
  payload += "PACKET=";
  payload += packetNumber;
  payload += ",TEMP=";
  payload += String(temperature, 2);
  payload += ",HUM=";
  payload += String(humidity, 2);

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/plain");
  client.println("Connection: close");
  client.println();
  client.println(payload);

  Serial.print("Sent: ");
  Serial.println(payload);

  client.stop();

  delay(1000);
}
