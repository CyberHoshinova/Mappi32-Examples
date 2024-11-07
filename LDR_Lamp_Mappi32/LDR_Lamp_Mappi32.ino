#define LDR_PIN 32     
#define RELAY_PIN 26   
const int lightThreshold = 500;
void setup() {
  Serial.begin(115200);
  pinMode(LDR_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);
}
void loop() {
  int lightLevel = analogRead(LDR_PIN);
  Serial.print("Light Level: ");
  Serial.println(lightLevel);
  if (lightLevel < lightThreshold) {
    digitalWrite(RELAY_PIN, HIGH);
    Serial.println("Lamp ON");
  } else {
    digitalWrite(RELAY_PIN, LOW);
    Serial.println("Lamp OFF");
  }
  delay(1000);
}
