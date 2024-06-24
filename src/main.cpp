#include <Arduino.h>

#include "HandController.h"

HandController handController;

void setup() {
  Serial.begin(115200);
  handController.checkServo();
  delay(1000);
  handController.moveToZero();
  // handController.setToPassiveMode();
  delay(5000);
}

int target1 = 180;
int target2 = 0;
int target = 0;
unsigned long lastMoveTime = 0;

void loop() {
  // Check if it's time to move the servo
  if (millis() - lastMoveTime >= 10000) {
    // Flip the target
    if (target == target1) {
      target = target2;
    } else {
      target = target1;
    }

    // Move the servo
    handController.move(2, -(target / 4));
    handController.move(4, target);
    handController.move(6, target);
    handController.move(8, target);

    // Update the last move time
    lastMoveTime = millis();
    Serial.println("Moving servo...");
  }

  // handController.updateServoPositions();
  // handController.printServoPositions();
  handController.updateMotion();
  // handController.setToPassiveMode();
  delay(10);
}