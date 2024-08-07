#include <Arduino.h>

#include "HandController.h"

HandController handController;

void setup() {
  Serial.begin(115200);
  Serial.println("Starting hand controller...");
  handController.initLeds();
  handController.checkServo();
  delay(1000);
  handController.moveToZero();
  // handController.setToPassiveMode();
  delay(50000);
}

int mode = 0;
unsigned long lastMoveTime = 0;

void loop() {
  // Check if it's time to move the servo
  if (millis() - lastMoveTime >= 3000) {
    if (mode < 4) {
      mode = mode + 1;
    } else {
      mode = 0;
    }

    if (mode == 0) {
      int targets[] = {0, -60, 0, 90, 0, 90, 0, 90};
      handController.setTargets(targets);
    } else if (mode == 1) {
      int targets[] = {0, 0, 0, 0, 0, 0, 0, 0};
      handController.setTargets(targets);
    } else if (mode == 2) {
      int targets[] = {-70, 0, -20, 0, 0, 0, 20, 0};
      handController.setTargets(targets);
    } else if (mode == 3) {
      int targets[] = {-70, -50, -20, 90, 0, 90, 20, 90};
      handController.setTargets(targets);
    } else if (mode == 4) {
      int targets[] = {0, 0, 0, 0, 0, 0, 0, 0};
      handController.setTargets(targets);
    }

    // Update the last move time
    lastMoveTime = millis();
    Serial.println("Moving servo...");
  }

  handController.updateMotion();
  delay(10);
}