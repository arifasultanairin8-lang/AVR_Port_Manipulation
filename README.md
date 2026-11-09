IoT Telemetry Node: ESP32 Wi-Fi & Register Control
📌 Project Overview

This project implements an IoT telemetry node using an ESP32 DevKit V1, an analog sensor (Potentiometer or LDR), and a status LED.

The system continuously reads the 12-bit analog value from the sensor, connects to Wokwi's virtual Wi-Fi network, and sends an HTTP GET request every 3 seconds containing the sensor reading. Simultaneously, it uses low-level hardware registers (GPIO_OUT_W1TS_REG and GPIO_OUT_W1TC_REG) to cleanly toggle the status LED during data transmission.

🎯 Objectives

Build an IoT telemetry node (0–4095 range).

Read raw 12-bit analog sensor data from GPIO 4 (or GPIO 15).

Connect to Wokwi's virtual Wi-Fi network (SSID: "Wokwi-GUEST", password: "").

Send periodic HTTP GET requests (e.g., [http://httpbin.org/get?sensor=VAL](http://httpbin.org/get?sensor=VAL)) every 3 seconds.

Control the status LED on GPIO 2 using direct hardware register manipulation instead of digitalWrite().

Ensure sensor readings remain stable and responsive during active Wi-Fi transmission.

🛠️ Components Required

ESP32 DevKit V1

1 × Potentiometer (or Soil/LDR Analog Sensor)

1 × Status LED

1 × 220Ω Resistor

Jumper Wires

Breadboard

🔌 Pin Configuration

Component	ESP32 Pin	Register / Notes
Analog Sensor (SIG)	GPIO 4 (or GPIO 15)	Read via analogRead()
Status LED	GPIO 2	Controlled via GPIO_OUT_W1TS_REG / GPIO_OUT_W1TC_REG
Sensor VCC	3V3	Power Supply
Sensor / LED GND	GND	Common Ground
⚙️ Working Principle

The status LED pin (GPIO 2) and sensor pin (GPIO 4) are initialized in the setup phase.

The system connects to Wokwi's virtual Wi-Fi access point (Wokwi-GUEST).

The 12-bit analog sensor value (
0−4095) is continuously read and printed to the Serial Monitor.

Every 3 seconds, an HTTP GET request is dispatched to the HTTP server endpoint carrying the sensor value.

During data transmission, the status LED on GPIO 2 is turned ON using GPIO_OUT_W1TS_REG (Set Register) and turned OFF using GPIO_OUT_W1TC_REG (Clear Register).

Non-blocking time management ensures high responsiveness and stability when adjusting the sensor knob.

📊 Expected Output

Status	Serial Monitor Log	LED State
Wi-Fi Connecting	Connecting to Wi-Fi.....	OFF
Wi-Fi Connected	Connected to Wi-Fi!	OFF
Sensor Reading	Raw Sensor Value: 1845	OFF
HTTP Transmission	HTTP Response code: 200	ON (Toggled via Register)
💻 Code

The Arduino source code is available in sketch.ino.

C++
#include <WiFi.h>
#include <HTTPClient.h>

// Wi-Fi Credentials for Wokwi Simulator
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// Endpoint URL
const char* serverName = "http://httpbin.org/get?sensor=";

// Pin Definitions
const int sensorPin = 4; // GPIO 4 (or 15)
const int ledPin = 2;    // GPIO 2 for Status LED

unsigned long lastTime = 0;
unsigned long timerDelay = 3000; // Every 3 seconds

void setup() {
  Serial.begin(115200);

  pinMode(sensorPin, INPUT);
  pinMode(ledPin, OUTPUT);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi!");
}

void loop() {
  // Read 12-bit analog value (0 - 4095)
  int sensorValue = analogRead(sensorPin);
  Serial.print("Raw Sensor Value: ");
  Serial.println(sensorValue);

  // Send HTTP GET request every 3 seconds and toggle LED using registers
  if ((millis() - lastTime) > timerDelay) {
    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      
      String serverPath = String(serverName) + String(sensorValue);
      http.begin(serverPath.c_str());
      
      int httpResponseCode = http.GET();
      
      if (httpResponseCode > 0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload);

        // Turn ON LED using ESP32 W1TS register (GPIO 2) during data transmission
        REG_WRITE(GPIO_OUT_W1TS_REG, (1 << ledPin));
        delay(200); 
        // Turn OFF LED using W1TC register
        REG_WRITE(GPIO_OUT_W1TC_REG, (1 << ledPin));
        
      } else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      http.end();
    } else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }

  delay(100); 
}
🔗 Simulation Links

Wokwi Simulation: https://wokwi.com/projects/480130282233578497


📚 Registers Used

GPIO_OUT_W1TS_REG – ESP32 GPIO Output Write 1 to Set Register (Sets GPIO output HIGH)

GPIO_OUT_W1TC_REG – ESP32 GPIO Output Write 1 to Clear Register (Sets GPIO output LOW)

👩‍💻 Project Information

Platform: ESP32 DevKit V1

Simulator: Wokwi

Project Type: IoT Telemetry / Register Manipulation

Language: C++ (Arduino Framework)
