/*
  Low-Power Wireless IoT Sensor Node
  ----------------------------------
  NodeMCU ESP8266 / ESP-12E

  ROLE:
  RF measurement receiver / data logger

  This board:
    1. Connects to the transmitter's Wi-Fi AP.
    2. Requests a sensor packet.
    3. Measures RSSI.
    4. Measures application-level round-trip/request-response latency.
    5. Calculates Packet Delivery Ratio (PDR).
    6. Prints CSV data to Serial Monitor.

  Serial Monitor: 115200 baud


*/

#include <ESP8266WiFi.h>

const char* WIFI_SSID = "IoT_Sensor_Node";
const char* WIFI_PASSWORD = "12345678";

// Default ESP8266 SoftAP address.
IPAddress TRANSMITTER_IP(192, 168, 4, 1);

WiFiClient client;

unsigned long packetAttempt = 0;
unsigned long packetsReceived = 0;
unsigned long packetsLost = 0;

const unsigned long REQUEST_TIMEOUT_MS = 1000;
const unsigned long ATTEMPT_INTERVAL_MS = 1000;

void connectToTransmitter() {
  Serial.println();
  Serial.println("Connecting to transmitter...");

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  unsigned long start = millis();

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");

    if (millis() - start > 15000) {
      Serial.println();
      Serial.println("Connection timeout. Restarting Wi-Fi...");
      WiFi.disconnect();
      delay(500);
      WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
      start = millis();
    }
  }

  Serial.println();
  Serial.println("Connected!");

  Serial.print("Receiver IP : ");
  Serial.println(WiFi.localIP());

  Serial.print("RSSI        : ");
  Serial.print(WiFi.RSSI());
  Serial.println(" dBm");

  Serial.println();
  Serial.println("CSV DATA");
  Serial.println("attempt,rssi_dbm,latency_ms,status");
  Serial.println("-----------------------------------------");
}

bool requestPacket(unsigned long& latencyMs, String& payload) {
  payload = "";

  unsigned long startTime = millis();

  if (!client.connect(TRANSMITTER_IP, 80)) {
    latencyMs = millis() - startTime;
    return false;
  }

  client.println("GET / HTTP/1.1");
  client.println("Host: 192.168.4.1");
  client.println("Connection: close");
  client.println();

  unsigned long timeoutStart = millis();

  while (millis() - timeoutStart < REQUEST_TIMEOUT_MS) {
    if (client.available()) {
      // Skip HTTP header lines.
      String line = client.readStringUntil('\n');
      line.trim();

      if (line.length() == 0) {
        // Empty line means HTTP headers have ended.
        break;
      }
    }

    yield();
  }

  // Read the actual payload.
  unsigned long payloadStart = millis();

  while (millis() - payloadStart < 500) {
    if (client.available()) {
      payload = client.readStringUntil('\n');
      payload.trim();

      if (payload.length() > 0) {
        break;
      }
    }

    yield();
  }

  latencyMs = millis() - startTime;

  client.stop();

  return payload.startsWith("PACKET=");
}

void printSummary() {
  float pdr = 0.0;

  if (packetAttempt > 0) {
    pdr = (float)packetsReceived / (float)packetAttempt * 100.0;
  }

  Serial.println();
  Serial.println("========== SUMMARY ==========");
  Serial.print("Attempts : ");
  Serial.println(packetAttempt);

  Serial.print("Received : ");
  Serial.println(packetsReceived);

  Serial.print("Lost     : ");
  Serial.println(packetsLost);

  Serial.print("PDR      : ");
  Serial.print(pdr, 2);
  Serial.println(" %");

  Serial.println("=============================");
  Serial.println();
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println();
  Serial.println("================================");
  Serial.println("    RF DATA LOGGER - RECEIVER");
  Serial.println("================================");

  connectToTransmitter();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Wi-Fi disconnected.");
    connectToTransmitter();
  }

  packetAttempt++;

  int rssi = WiFi.RSSI();

  unsigned long latency = 0;
  String payload;

  bool received = requestPacket(latency, payload);

  if (received) {
    packetsReceived++;

    Serial.print(packetAttempt);
    Serial.print(",");
    Serial.print(rssi);
    Serial.print(",");
    Serial.print(latency);
    Serial.println(",RECEIVED");

    Serial.print("Payload: ");
    Serial.println(payload);
  } else {
    packetsLost++;

    Serial.print(packetAttempt);
    Serial.print(",");
    Serial.print(rssi);
    Serial.print(",");
    Serial.print(latency);
    Serial.println(",LOST");
  }

  // Print summary every 10 attempts.
  if (packetAttempt % 10 == 0) {
    printSummary();
  }

  delay(ATTEMPT_INTERVAL_MS);
}
