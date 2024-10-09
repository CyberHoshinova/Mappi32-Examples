#include <SPI.h>
#include <LoRa.h>

#define ss 15
#define rst 0
#define dio0 27

void setup() {
  Serial.begin(115200);
  while (!Serial);
  Serial.println("LoRa Receiver");

  // Setup LoRa transceiver module
  SPI.begin(14, 12, 13, 15);
  LoRa.setPins(ss, rst, dio0);

  while (!LoRa.begin(433E6)) {
    Serial.println(".");
    delay(500);
  }

  LoRa.setSyncWord(0xF3);
  Serial.println("LoRa Initializing OK!");
}

void loop() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    String LoRaData;
    int rssi = LoRa.packetRssi(); // Get RSSI value

    Serial.println("Received packet");
    
    while (LoRa.available()) {
      LoRaData = LoRa.readString();
      Serial.println(LoRaData);
    }
    
    Serial.print("RSSI: ");
    Serial.println(rssi);
  }
}
