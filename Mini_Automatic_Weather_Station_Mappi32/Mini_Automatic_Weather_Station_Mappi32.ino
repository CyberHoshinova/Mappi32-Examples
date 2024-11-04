#include "Wire.h" // I2C library for SHT35 communication
#include "SHTSensor.h" // SHTSensor library for SHT35 sensor

// Create an instance for the SHT35 sensor
SHTSensor sht;

#define RXD2 16 // setting RX (TX) to pin 16 for communication with GSM Shield
#define TXD2 17 // setting TX (RX) to pin 17 for communication with GSM Shield
#define SIM Serial2  // variable for Serial2

#define api_keyNumber "QYV4SQNCFVVNHKV3" // ThingSpeak API key

int responSIM;
int conLen = 0; // Calculate content length for the data to be sent to ThingSpeak

// Anemometer Setup
const int RecordTime = 3;  // Define Measuring Time (seconds)
const int SensorPin = 23;  // Define Interrupt Pin (GPIO23)
int InterruptCounter = 0;  // Counter for wind speed
float WindSpeed = 0;       // Wind speed in km/h

void setup() {
  Serial.begin(115200);
  SIM.begin(115200, SERIAL_8N1, RXD2, TXD2); // Set baud rate, protocol, and pins for Serial2 communication
  delay(1000);
  Serial.println("Mappi32 with Shield SIM");
  Serial.println("Waiting...");

  // Initialize the SHT35 sensor
  Wire.begin(); // Initialize I2C communication
  if (sht.init()) {
    Serial.println("SHT35 sensor initialized successfully.");
  } else {
    Serial.println("Failed to initialize SHT35 sensor. Check wiring!");
    while (1) delay(10); // Halt execution if the sensor is not found
  }
  sht.setAccuracy(SHTSensor::SHT_ACCURACY_MEDIUM); // Set accuracy for SHT35

  // Set up the anemometer pin as input
  pinMode(SensorPin, INPUT);
}

void loop() {
  // Check if SIM is ready
  SIM.println("AT");
  delay(1000);
  responSIM = int(SIM.read());

  if (responSIM < 1) {
    Serial.print(".");
  }

  if (responSIM > 1) {
    Serial.println("\nSIM READY");
    delay(2000);

    // Get temperature and humidity readings from SHT35 sensor
    sht.readSample();
    float temperature = sht.getTemperature();
    float relative_humidity = sht.getHumidity();

    // Print temperature and humidity to Serial
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" °C");

    Serial.print("Humidity: ");
    Serial.print(relative_humidity);
    Serial.println(" %");

    // Measure wind speed using the anemometer
    measureWindSpeed();
    Serial.print("Wind Speed: ");
    Serial.print(WindSpeed);
    Serial.println(" km/h");

    // Send data to ThingSpeak
    SIM.println("AT+CPIN?");
    delay(1000);
    ShowSerialData();

    SIM.println("AT+CGATT=1");
    delay(1000);
    ShowSerialData();

    SIM.println("AT+QICSGP=1,1,\"m2minternet\",\"\",\"\",1"); // APN for SIM card
    delay(1000);
    ShowSerialData();

    SIM.println("AT+QIACT=1");
    delay(1000);
    ShowSerialData();

    SIM.println("AT+QIACT?");
    delay(1000);
    ShowSerialData();

    SIM.println("AT+QIOPEN=1,0,\"TCP\",\"api.thingspeak.com\",80,0,1");
    delay(1000);
    ShowSerialData();

    SIM.println("AT+QISTATE=1,0");
    delay(1000);
    ShowSerialData();

    SIM.println("AT+QISEND=0");
    delay(1000);
    ShowSerialData();

    SIM.println("POST /update.json HTTP/1.1");
    delay(1000);
    ShowSerialData();

    SIM.println("Host: api.thingspeak.com");
    delay(1000);
    ShowSerialData();

    SIM.println("Content-Type: application/json");
    delay(1000);
    ShowSerialData();

    // Create the data payload with temperature, humidity, and wind speed
    String data = "{\"api_key\":\"" + String(api_keyNumber) + "\",\"field1\":" + String(WindSpeed) +
                  ",\"field2\":" + String(temperature) + ",\"field3\":" + String(relative_humidity) + "}";
    conLen = data.length(); // Calculate content length

    SIM.println("Content-Length: " + String(conLen) + "\r\n");
    delay(1000);
    ShowSerialData();

    SIM.println(data); // Send the actual data
    delay(1000);
    ShowSerialData();

    delay(1000);

    SIM.write(0x1A); // End of data transmission
    delay(1000);
    ShowSerialData();

    SIM.println("AT+QICLOSE=1"); // Close the connection
    delay(1000);
    ShowSerialData();

    SIM.println("AT+QIDEACT=1"); // Deactivate context
    delay(1000);
    ShowSerialData();

    delay(1000);
    Serial.println("Waiting...");
  }

  // Wait for 5 minutes before the next loop
  delay(3000);
}

// Function to measure wind speed using the anemometer
void measureWindSpeed() {
  InterruptCounter = 0;
  attachInterrupt(digitalPinToInterrupt(SensorPin), countup, RISING);
  delay(1000 * RecordTime);
  detachInterrupt(digitalPinToInterrupt(SensorPin));

  // Calculate wind speed in km/h
  WindSpeed = (float)InterruptCounter / (float)RecordTime * 2.4;
}

// Interrupt function to count the anemometer rotations
void countup() {
  InterruptCounter++;
}

// Function to display serial data from GSM Shield
void ShowSerialData() {
  while (SIM.available() != 0) {
    Serial.write(char(SIM.read())); // Print response from GSM Shield
  }
}
