/**
 * @file    ESP8266_show_connection_status.ino
 * @author  Frank Häfele(mail@frankhaefele.de)
 * @brief   Example to show connection state via 3 LEDs on MCP23008
 * @version 0.1
 * @date    2024-12-30
 * 
 * @copyright Copyright (c) 2024 Frank Häfele
 * 
 */

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <MCP23008-I2C.h>

constexpr const char *SSID {"dummy"};
constexpr const char *PASSWORD {"dummy"};

// ESP-01
constexpr int SDA_PIN {0};
constexpr int SCL_PIN {2};

/**
 * @brief LED definitions
 * 
 * LED_VCC   : connect anode (+) of LEDs here
 * LED_RED   : connect cathode of red LED here
 * LED_YELLOW: connect cathode of yellow LED here
 * LED_GREEN : connect cathode of green LED here
 */
constexpr unsigned int LED_VCC    {3U};  // LED Vcc
constexpr unsigned int LED_RED    {2U};  // not connected
constexpr unsigned int LED_YELLOW {1U};  // connect state unknown
constexpr unsigned int LED_GREEN  {0U};  // connected

// MCP23008 object
MCP23008_I2C::MCP23008 mcp{0x20, &Wire};


void setup_Pins() {
  // mcp ini; input Pull-up per default
  mcp.begin();
  // set pins 0...4 to outputs => 0
  mcp.setPinMode8(0b11110000);
  // activate the LED Vcc and the Yellow LED
  mcp.write8(0b00001101);
}

void connected() {
  // activate the LED Vcc and the Yellow green
  mcp.write8(0b00001110);
}

void disconnected() {
    // activate the LED Vcc and the LED RED
  mcp.write8(0b00001011);
}

// Wifi event handler
WiFiEventHandler staConnected_EventHandler, staGotIp_EventHandler, staDisconnected_EventHandler;


void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.print("\n### === ESP8266 Show Connection State via LEDs connected to MCP23008  === ###\n\n");

  // SDA , SCL
  Wire.begin(SDA_PIN, SCL_PIN);
  delay(100);

  setup_Pins();

  // set Wifi events
  staConnected_EventHandler = WiFi.onStationModeConnected([] (const WiFiEventStationModeConnected &event [[gnu::unused]]) {
    // do stuff when wifi is connectd
    Serial.println("\nEVENT: Wifi STA connected!");
    Serial.println("\n");
    connected();
   });
   
  staGotIp_EventHandler = WiFi.onStationModeGotIP([] (const WiFiEventStationModeGotIP &event [[gnu::unused]]) {
    // do stuff when ESP got IP
    Serial.println("\nEVENT: Wifi STA got IP-Address!");
    Serial.println("\n");
   });
   
  staDisconnected_EventHandler = WiFi.onStationModeDisconnected([] (const WiFiEventStationModeDisconnected &event [[gnu::unused]]) {
    // do stuff when ESP got disconnectd
    Serial.println("\nEVENT: Wifi STA disconnected!");
    Serial.println("\n");
    disconnected();
   });

  // Start Wifi
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASSWORD);
  if (WiFi.waitForConnectResult() == WL_CONNECTED) {
    Serial.println(F("INFO: Connected!"));
  }
  else {
     Serial.println(F("INFO: No Connection Possible"));
  }
}

void loop() {
  // nothing to  do here 
}