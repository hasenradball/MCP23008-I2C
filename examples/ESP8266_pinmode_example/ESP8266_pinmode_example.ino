/**
 * @file    ESP8266_pinmode_example.ino
 * @author  Frank Häfele(mail@frankhaefele.de)
 * @brief   Example to demonstrate how to se pinMode on MCP23008 device
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

// ESP-01
constexpr int SDA_PIN {0};
constexpr int SCL_PIN {2};

MCP23008_I2C::MCP23008 mcp{0x20, &Wire};

void print_byte_as_bit(char value) {
   for (int i = 7; i >= 0; --i) {
      char c = (value & (1 << i)) ? '1' : '0';
      Serial.print(c);
   }
   Serial.println();
}

void setup_pinMode(uint8_t mask) {
  // set pins to input or output
  Serial.print("Set PinMode ");
  print_byte_as_bit(mask);
  auto setter_state = mcp.setPinMode8(mask);
  if (setter_state < MCP23008_I2C::MCP23008_STATE_OK) {
      Serial.printf("Set-Error: %d\n", setter_state);
  }

  delay(1000);
  Serial.print("Get PinMode ");
  auto getter_state = mcp.getPinMode8();
  if (getter_state < MCP23008_I2C::MCP23008_STATE_OK) {
      Serial.printf("Read-Error: %d\n", getter_state);
  }
  print_byte_as_bit(getter_state);
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  WiFi.mode(WIFI_OFF);

  // SDA , SCL
  Wire.begin(SDA_PIN, SCL_PIN);
  delay(500);

  Serial.print("\n### === PinMode-Test of MCP23008-I2C === ###\n\n");

  Serial.printf("\n\nMCP connected: %d\n", mcp.begin(false) == MCP23008_I2C::MCP23008_STATE_OK ? true : false);
  // Check PullUp State of pins
  Serial.print("get pullup state from register: ");
  print_byte_as_bit(mcp.getPullup8());

  delay(200);

  Serial.println("\n1.) setup pins as output all at once");
  setup_pinMode(0b00000000);
  
  delay(5000);

  Serial.println("\n2.) setup pins as input all at once");
  setup_pinMode(0b11111111);

  delay(5000);

  Serial.println("\n3.) setup pins as input and output all at once");
  setup_pinMode(0b11110000);
  Serial.println("\n4.) write high on outputs via OLAT Register");
  mcp.write8(0b11111111);
}


void loop() {
 // do nothing here
}