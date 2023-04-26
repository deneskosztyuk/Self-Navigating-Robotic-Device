//Software to test a maze running robot
#include <SharpIR.h>

// Pin definitions
#define sensorFpin A0
#define sensorLpin A1
#define sensorRpin A2
#define model 1080

int enMotorL = 9; //left motor speed pin
int motorLF = 8; //left motor forward
int motorLB = 7; //left motor backward

int enMotorR = 6; //right motor speed pin
int motorRF = 5; //right motor forward
int motorRB = 3; //right motor backward

int threshold = 16;

SharpIR sensorF = SharpIR(sensorFpin, model);
SharpIR sensorL = SharpIR(sensorLpin, model);
SharpIR sensorR = SharpIR(sensorRpin, model);

int fDistance, lDistance, rDistance;

char newDir = 'S';
char currentDir;

void setup() {
  Serial.begin(115200);
  Serial.println("Starting");

  pinMode(motorLF, OUTPUT);
  pinMode(motorRF, OUTPUT);
  pinMode(motorLB, OUTPUT);
  pinMode(motorRB, OUTPUT);
  pinMode(enMotorR, OUTPUT);
  pinMode(enMotorL, OUTPUT);

  analogWrite(enMotorR, 255);
  analogWrite(enMotorL, 255);

  digitalWrite(motorLF, LOW);
  digitalWrite(motorLB, LOW);
  digitalWrite(motorRF, LOW);
  digitalWrite(motorRB, LOW);
}

void checkSensors() {
  fDistance = sensorF.distance();
  lDistance = sensorL.distance();
  rDistance = sensorR.distance();
}

void loop() {
  checkSensors();
  leftFollowingAlgorithm();
  setDirection();
  delay(100);
}

void leftFollowingAlgorithm() {
  if (lDistance < threshold - 5) {
    newDir = 'R'; // Move right if too close to the left wall
  } else if (lDistance > threshold + 5) {
    newDir = 'L'; // Move left if too far from the left wall
  } else if (fDistance < threshold) {
    newDir = 'S'; // Stop if there is an obstacle in front
  } else {
    newDir = 'F'; // Move forward if no obstacles are detected
  }
}

void setDirection() {
  if (newDir == currentDir) {
    Serial.println("No update to direction made");
  } else if (newDir == 'F') {
    digitalWrite(motorLF, HIGH);
    digitalWrite(motorLB, LOW);
    digitalWrite(motorRF, HIGH);
    digitalWrite(motorRB, LOW);
    Serial.println("Moving Forward");
  } else if (newDir == 'S') {
    digitalWrite(motorLF, LOW);
    digitalWrite(motorLB, LOW);
    digitalWrite(motorRF, LOW);
    digitalWrite(motorRB, LOW);
    Serial.println("Stopping");
  } else if (newDir == 'L') {
    digitalWrite(motorLF, LOW);
    digitalWrite(motorLB, HIGH);
    digitalWrite(motorRF, HIGH);
    digitalWrite(motorRB, LOW);
    Serial.println("Turning Left");
    delay(200);
  } else if (newDir == 'R') {
    digitalWrite(motorLF, HIGH);
    digitalWrite(motorLB, LOW);
    digitalWrite(motorRF, LOW);
    digitalWrite(motorRB, HIGH);
    Serial.println("Turning Right");
    delay(200);
  }

  currentDir = newDir;
  Serial.print("Current Dir: ");
  Serial.println(currentDir);
}
