# Low-Power-Wireless-IoT-Sensor-Node

Low-Power Wireless IoT Sensor Node
A wireless IoT sensor node built using ESP8266 (ESP-12E/NodeMCU) and DHT11 for environmental sensing and wireless communication analysis.
The project focuses on collecting sensor data, transmitting it wirelessly, and evaluating the quality of the wireless link using parameters such as RSSI, latency, packet delivery ratio (PDR), and communication range.
Current status: Sensor interfacing, wireless communication, two-node communication, and RF data logging are completed. Low-power optimization, power measurement, battery operation, and custom PCB development are planned for later phases.

Project Overview
The system consists of two ESP8266 NodeMCU boards.
                 SENSOR NODE
             
              ┌───────────────┐
              │   DHT11       │
              │ Temperature   │
              │ + Humidity    │
              └───────┬───────┘
            
                      │
                      ▼
              ┌───────────────┐
              │ ESP8266 #1    │
              │ Transmitter   │
              │ Wi-Fi AP      │
              └───────┬───────┘
                      │
                 Wi-Fi Link
                      │
                      ▼
              ┌───────────────┐
              │ ESP8266 #2    │
              │ RF Receiver   │
              └───────┬───────┘
                      │ USB
                      ▼
                  ┌───────┐
                  │Laptop │
                  └───┬───┘
                      │
                      ▼
              Python Data Analysis
                      │
          ┌───────────┼───────────┐
          ▼           ▼           ▼
        RSSI         PDR       Latency

Objectives
- Interface a DHT11 sensor with an ESP8266.
- Establish wireless communication between two ESP8266 nodes.
- Transmit temperature and humidity data wirelessly.
- Measure wireless signal strength using RSSI.
- Measure communication latency.
- Calculate Packet Delivery Ratio (PDR).
- Collect RF characterization data for different distances.
- Analyze the collected data using Python.
- Build a foundation for a low-power wireless IoT sensor node.
Hardware Used
Component	Quantity	Purpose
NodeMCU ESP8266 / ESP-12E	2	Wireless transmitter and receiver
DHT11	1	Temperature and humidity sensing
USB cable	2	Programming and power
Breadboard	1	Sensor connection
Jumper wires	As required	Connections
Laptop	1	Programming and data analysis


The Arduino Uno is not required for the current implementation.
DHT11 Wiring
The DHT11 is connected to the transmitter NodeMCU.
DHT11 Pin	NodeMCU
VCC	3V3
DATA	D2 / GPIO4
GND	GND


Connection
DHT11                 NodeMCU ESP8266

 VCC  ─────────────── 3V3
 DATA ─────────────── D2 (GPIO4)
 GND  ─────────────── GND

System Implementation
Phase 1 — DHT11 Sensor Interfacing
The first stage verifies that the ESP8266 can correctly read environmental data from the DHT11.
The system collects:
- Temperature in °C
- Relative humidity in %
The sensor data is initially displayed through the Arduino Serial Monitor.
Phase 2 — ESP8266 Wireless Sensor Node
The ESP8266 is configured as a Wi-Fi Access Point.
The sensor node creates its own wireless network:
SSID: IoT_Sensor_Node
Password: 12345678
IP Address: 192.168.4.1

The ESP8266 runs a lightweight HTTP server that allows another ESP8266 to request sensor data.
Phase 3 — Two-Node Wireless Communication
Two ESP8266 boards are used:
NodeMCU #1 — Transmitter
Responsibilities:
- Reads DHT11
- Creates Wi-Fi network
- Generates packet numbers
- Sends temperature and humidity data
NodeMCU #2 — Receiver
Responsibilities:
- Connects to NodeMCU #1
- Requests sensor packets
- Records received packets
- Measures RSSI
- Measures latency
- Detects lost packets
Communication:
ESP8266 #1
   │
   │ Wi-Fi
   ▼
ESP8266 #2
   │
   │ USB
   ▼
Laptop

RF Link Characterization
The wireless link is evaluated using several parameters.
1. RSSI
RSSI (Received Signal Strength Indicator) indicates the strength of the received Wi-Fi signal.
It is measured in dBm.
Example:
-40 dBm  → Strong signal
-60 dBm  → Moderate signal
-80 dBm  → Weak signal

The actual values depend on distance, obstacles, interference, antenna orientation, and the surrounding environment.
2. Packet Delivery Ratio
Packet Delivery Ratio indicates how many transmitted packets successfully reach the receiver.
Formula
PDR (%) = (Packets Received / Packets Sent) × 100

For example:
Packets Sent     = 100
Packets Received = 96

PDR = (96 / 100) × 100
    = 96%

3. Latency
Latency represents the time required for the receiver's request to obtain a response from the transmitter.
The current implementation measures application-level request/response latency, rather than precise one-way physical-layer RF propagation delay.
4. Communication Range
The RF link can be tested at different distances, for example:
1 m
5 m
10 m
15 m
20 m
25 m
30 m

At each distance, multiple packet attempts should be performed.
A recommended test is:
100 packets per distance

This provides enough data to calculate meaningful PDR and average RF performance.
RF Data Format
The receiver produces CSV-style output:
attempt,rssi_dbm,latency_ms,status
1,-47,35,RECEIVED
2,-48,38,RECEIVED
3,-51,42,RECEIVED
4,-53,46,RECEIVED
5,-55,51,RECEIVED

Possible status values:
RECEIVED
LOST

This data can later be imported into Python for analysis.
Python Data Analysis
The collected RF data is analyzed using Python.
The analysis calculates:
- Packets sent
- Packets received
- Packets lost
- Packet Delivery Ratio
- Average RSSI
- Average latency
The project also generates graphs such as:
Distance vs RSSI
Shows how signal strength changes with distance.
Distance vs PDR
Shows wireless reliability at different distances.
Distance vs Latency
Shows how communication latency changes with distance.
Software and Tools
- Arduino IDE
- ESP8266 Arduino Core
- C/C++
- Python
- Pandas
- Matplotlib
- Git
- GitHub
Arduino Libraries
ESP8266WiFi
DHT sensor library
Adafruit Unified Sensor

Project Structure
Low-Power-Wireless-IoT-Sensor-Node/


│
├── Arduino Code/
│   │
│   ├── 
│   │  ── Sensor_Node_Transmitter.ino
│   │
│   └── 
│       ── RF_Data_Logger_Receiver.ino
│
├── Python and Data/
│   └── rf_characterization_analysis.py
│   └── rf_characterization_sample_data.csv
│
├── Results/
│   ├── distance_vs_rssi.png
│   ├── distance_vs_pdr.png
│   └── distance_vs_latency.png
│
└── README.md


How to Run
1. Install ESP8266 Board Support
Add ESP8266 board support to Arduino IDE and select the appropriate NodeMCU ESP8266 board.
2. Install Required Libraries
Install:
DHT sensor library
Adafruit Unified Sensor

ESP8266WiFi is included with the ESP8266 board package.
3. Upload Transmitter Code
Connect NodeMCU #1 to the laptop.
Open:
Arduino/Sensor_Node_Transmitter/

Upload:
Sensor_Node_Transmitter.ino

Open Serial Monitor at:
115200 baud

You should see:
IoT SENSOR NODE - TRANSMITTER
Wi-Fi SSID : IoT_Sensor_Node
AP IP      : 192.168.4.1
Server     : Started

4. Upload Receiver Code
Connect NodeMCU #2.
Open:
Arduino/RF_Data_Logger_Receiver/

Upload:
RF_Data_Logger_Receiver.ino

Open Serial Monitor at:
115200 baud

The receiver should connect to:
IoT_Sensor_Node

and start producing RF measurements.
Important Arduino Folder Requirement
The transmitter and receiver sketches must be stored in separate folders.
Correct:
Sensor_Node_Transmitter/
    Sensor_Node_Transmitter.ino

RF_Data_Logger_Receiver/
    RF_Data_Logger_Receiver.ino

Do not put both .ino files inside the same Arduino sketch folder because Arduino combines .ino files during compilation, which can cause errors such as:
redefinition of 'setup()'
redefinition of 'loop()'
redefinition of 'ssid'

Current Project Status
Completed
- [x] ESP8266 setup
- [x] DHT11 interfacing
- [x] Temperature measurement
- [x] Humidity measurement
- [x] ESP8266 Wi-Fi Access Point
- [x] Wireless communication between two ESP8266 nodes
- [x] Sensor data transmission
- [x] Packet numbering
- [x] RSSI measurement
- [x] Latency measurement
- [x] Packet loss detection
- [x] PDR calculation
- [x] CSV-style RF data logging
- [x] Python-based RF data analysis setup
Planned
- [ ] Distance-based RF experiments
- [ ] Real hardware RF dataset collection
- [ ] Low-power sleep/wake operation
- [ ] Current and power measurement
- [ ] Battery-powered operation
- [ ] Energy-per-packet analysis
- [ ] Custom PCB design
- [ ] Final hardware enclosure
Future Low-Power Architecture
The planned low-power operating cycle is:
    
        ┌──────────────┐
        │     WAKE     │
        └──────┬───────┘
               ↓
        ┌──────────────┐
        │ Read DHT11   │
        └──────┬───────┘
               ↓
        ┌──────────────┐
        │ Transmit     │
        │ Sensor Data  │
        └──────┬───────┘
               ↓
        ┌──────────────┐
        │    SLEEP     │
        └──────┬───────┘
               │
               └──────────→ WAKE

This will reduce unnecessary Wi-Fi activity and allow the sensor node to operate from a battery.
Disclaimer About Sample Data
The included RF dataset is sample/representative data created for development and testing of the analysis pipeline.
It should not be presented as actual hardware measurements.
For the final project report and resume, the sample dataset should be replaced with measurements collected from the actual ESP8266 hardware at different distances.
Skills Demonstrated
Embedded Systems
ESP8266 / ESP-12E
Arduino IDE
C/C++
Sensor Interfacing
DHT11
Wi-Fi Communication
IoT
RF Link Characterization
RSSI Analysis
Packet Delivery Ratio
Latency Measurement
Data Logging
Python
Pandas
Matplotlib
Git & GitHub

Project Outcome
This project demonstrates the development of a wireless IoT sensor node from sensor interfacing and embedded firmware to wireless communication and RF performance analysis.
The next development stage will focus on low-power operation, power consumption measurement, battery operation, and eventually a custom PCB implementation.****
