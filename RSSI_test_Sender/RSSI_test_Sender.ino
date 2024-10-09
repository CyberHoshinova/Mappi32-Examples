#include <SPI.h>
#include <LoRa.h>

#define ss 15
#define rst 0
#define dio0 27

void setup() {
  Serial.begin(115200); // For Serial Monitor

  // Initialize LoRa
  SPI.begin(14, 12, 13, ss);
  LoRa.setPins(ss, rst, dio0);

  while (!LoRa.begin(433E6)) {
    Serial.println(".");
    delay(500);
  }
  LoRa.setSyncWord(0xF3);
  Serial.println("LoRa Initializing OK!");
}

void loop() {
  // Send a packet
  LoRa.beginPacket();
  LoRa.print("Test Packet");
  LoRa.endPacket();

  Serial.println("Packet sent");

  // Wait for a while before sending the next packet
  delay(2000); // Adjust the delay as needed
}
