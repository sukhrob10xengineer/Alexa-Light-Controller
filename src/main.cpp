#include <WiFi.h>
#include "fauxmoESP.h"

#define SERIAL_BAUDRATE 115200

// Replace with your Wi-Fi credentials
#define WIFI_SSID "IoTPrivate"
#define WIFI_PASS "iotprivate303"

// Define pin for the LED
#define LED_PIN_1 12

// Name of the LED for Alexa
#define LED_1 "led one"

fauxmoESP fauxmo;

// Function to connect to Wi-Fi
void wifiSetup() {
  // Set Wi-Fi mode to STA
  WiFi.mode(WIFI_STA);

  // Connect to Wi-Fi
  Serial.printf("[WIFI] Connecting to %s ", WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }
  Serial.println();

  // Connection successful
  Serial.printf("[WIFI] STATION Mode, SSID: %s, IP address: %s\n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());
}

void setup() {
  // Initialize serial port
  Serial.begin(SERIAL_BAUDRATE);
  Serial.println();

  // Connect to Wi-Fi
  wifiSetup();

  // Set up the LED pin
  pinMode(LED_PIN_1, OUTPUT);
  digitalWrite(LED_PIN_1, LOW);  // LED off

  // Create and set up the fauxmoESP server
  fauxmo.createServer(true); // not necessary, this is the default value
  fauxmo.setPort(80); // This is required for gen3 devices

  // Enable the fauxmoESP library
  fauxmo.enable(true);

  // Add the virtual device
  fauxmo.addDevice(LED_1);

  // Handler for Alexa commands
  fauxmo.onSetState([](unsigned char device_id, const char* device_name, bool state, unsigned char value) {
    // Callback when a command is received from Alexa
    Serial.printf("[MAIN] Device #%d (%s) state: %s value: %d\n", device_id, device_name, state ? "ON" : "OFF", value);
    
    if (strcmp(device_name, LED_1) == 0) {
      // Control the LED
      Serial.println("LED 1 switched by Alexa");
      digitalWrite(LED_PIN_1, state ? HIGH : LOW);  // Turn LED on/off
    }
  });
}

void loop() {
  // Handle fauxmoESP events
  fauxmo.handle();
}