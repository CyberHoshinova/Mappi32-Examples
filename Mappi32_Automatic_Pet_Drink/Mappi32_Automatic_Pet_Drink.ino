// Define pin numbers
const int IR_SENSOR_PIN = 32;
const int RELAY_PIN = 5;

bool isCooldown = false;       // To track the cooldown status
unsigned long cooldownTime = 60000; 
unsigned long relayActivationTime = 10000;  // 10 seconds in milliseconds
unsigned long lastActivationTime = 0;  // To track last relay activation time

void setup() {
  pinMode(IR_SENSOR_PIN, INPUT);   // Set IR sensor pin as input
  pinMode(RELAY_PIN, OUTPUT);      // Set relay pin as output
  Serial.begin(115200);            // Start serial communication for debugging
}

void loop() {
  // Read the IR sensor value
  int sensorValue = analogRead(IR_SENSOR_PIN);
  Serial.print("IR Sensor Value: ");
  Serial.println(sensorValue);

  // Check if object is detected and the system is not in cooldown
  if (sensorValue < 1000 && !isCooldown) {
    // Activate relay for 10 seconds
    digitalWrite(RELAY_PIN, HIGH);
    Serial.println("Pet detected, Relay ON!");
    delay(relayActivationTime);    // Keep the relay on for 10 seconds
    digitalWrite(RELAY_PIN, LOW);  // Turn off the relay
    Serial.println("Relay OFF!");

    // Start cooldown period
    isCooldown = true;
    lastActivationTime = millis();  // Record the time of activation
  }

  // Check if cooldown time has passed
  if (isCooldown) {
    unsigned long elapsedTime = millis() - lastActivationTime;
    if (elapsedTime >= cooldownTime) {
      isCooldown = false;  // Reset cooldown
      Serial.println("Cooldown period ended.");
    } else {
      unsigned long remainingTime = (cooldownTime - elapsedTime) / 1000;  // Remaining time in seconds
      Serial.print("Still on Cooldown. Remaining time: ");
      Serial.print(remainingTime);
      Serial.println(" seconds");
    }
  }

  delay(1000);  // Small delay to avoid rapid polling
}
