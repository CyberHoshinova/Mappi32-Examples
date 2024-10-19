#include <ESP32Servo.h>

#define TRIG_PIN 26       // Trigger pin of the HC-SR04
#define ECHO_PIN 25       // Echo pin of the HC-SR04
#define SERVO_PIN 23      // Servo motor control pin

Servo myServo;

const int DISTANCE_THRESHOLD = 20;  // Distance threshold for the ultrasonic sensor (in cm)

long duration;
int distance;

void setup() {
  Serial.begin(115200);
  
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  
  myServo.attach(SERVO_PIN);
  myServo.write(0);  // Start with servo closed
  
  Serial.println("System initialized");
}

void loop() {
  // Measure distance using HC-SR04
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH);
  distance = duration * 0.034 / 2;  // Calculate distance in cm
  Serial.print("Ultrasonic Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // If object detected by HC-SR04, open the servo
  if (distance < DISTANCE_THRESHOLD) {
    Serial.println("Object detected. Opening servo.");
    myServo.write(90);  // Open the servo
    delay(5000);
  } else {
    Serial.println("No object detected. Keeping servo closed.");
    myServo.write(0);  // Keep the servo closed
  }

  delay(500);  // Wait for a bit before the next loop
}
