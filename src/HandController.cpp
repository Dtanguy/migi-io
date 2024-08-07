// HandController.cpp

#include "HandController.h"

TSam HandController::sam(10, 11);

HandController::HandController() {}

void HandController::initLeds() {
  Serial.println("Initializing NeoPixel strip...");
  strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);
  strip.begin();
  strip.setBrightness(10);
  for (int i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 255));
  }
  strip.show();
}

void HandController::checkServo() {
  /*
  Serial.println("Finding servos...");
  int count;
  int* servos = sam.findServos(count);
  // Print the found servo IDs
  Serial.print("Found servoIDs: [");
  for (int i = 0; i < count; i++) {
    Serial.print(servos[i]);
    if (i < count - 1) {
      Serial.print(", ");
    }
  }
  Serial.println("]");
*/

  // Check if the found IDs match the expected ones
  for (int i = 0; i < nbServos; i++) {
    bool found = sam.CheckServo(expectedServoIDs[i]);

    if (!found) {
      Serial.print("Error: Servo ID ");
      Serial.print(expectedServoIDs[i]);
      Serial.println(" not found.");
      strip.setPixelColor(i, strip.Color(255, 0, 0));
    } else {
      strip.setPixelColor(i, strip.Color(0, 255, 0));
    }
    strip.show();
  }

  /*
    if (count != nbServos) {
      Serial.println("Error: Mismatch in number of servos found.");
    } else {
      for (int i = 0; i < count; i++) {
        if (servos[i] != expectedServoIDs[i]) {
          Serial.print("Error: Mismatch in servo ID at index ");
          Serial.println(i);
          strip.setPixelColor(i, strip.Color(0, 255, 0));
        } else {
          strip.setPixelColor(i, strip.Color(0, 255, 0));
        }
      }
      strip.show();
    }
    Serial.println("Servo IDs match the expected ones.");
    */
}

void HandController::move(int id, int deg) {
  if (deg == 666) return;

  // Find the motion range for the servo
  ServoMotionRange range;
  int j = 0;
  for (size_t i = 0; i < nbServos; i++) {
    if (expectedServoIDs[i] == id) {
      range = servoMotionRanges[i];
      j = i;
      break;
    }
  }

  // Calculate the target position
  int target = range.center + deg;
  target = constrain(target, range.min, range.max);
  servoTargets[j] = target;
  servoTargetTimes[j] = millis();
  servoStartPositions[j] = servoPositions[j];
}

void HandController::setTargets(int servoIDs[], int targets[], size_t size) {
  for (size_t i = 0; i < size; i++) {
    move(servoIDs[i], targets[i]);
  }
}

void HandController::setTargets(int targets[]) {
  for (size_t i = 0; i < nbServos; i++) {
    int id = expectedServoIDs[i];
    move(id, targets[i]);
  }
}

void HandController::updateMotion() {
  unsigned long currentTime = millis();

  for (size_t i = 0; i < nbServos; i++) {
    int id = expectedServoIDs[i];
    ServoMotionRange range = servoMotionRanges[i];

    int target = servoTargets[i];
    int startPos = servoStartPositions[i];
    unsigned long startTime = servoTargetTimes[i];
    unsigned long elapsedTime = currentTime - startTime;
    float blendTime = 500;

    if (elapsedTime < blendTime) {
      // Calculate the progress using an ease-in-out function
      float progress = static_cast<float>(elapsedTime) / blendTime;
      progress = -0.5 * (cos(M_PI * progress) - 1);

      // Calculate the new position based on the eased progress
      int newPosition =
          startPos + static_cast<int>((target - startPos) * progress);
      sam.PosSend(id, newPosition, speedLevel);
      servoPositions[i] = newPosition;
    } else {
      // Ensure the servo is at the target position after the blend time
      if (servoPositions[i] != target) {
        sam.PosSend(id, target, speedLevel);
        servoPositions[i] = target;
      }
    }
  }
}

void HandController::updateServoPositions() {
  for (size_t i = 0; i < nbServos; i++) {
    int id = expectedServoIDs[i];
    ServoMotionRange range = servoMotionRanges[i];
    servoPositions[i] = sam.PosRead(id) - range.center;
  }
}

void HandController::printServoPositions() {
  Serial.print("Servo positions: [");
  for (size_t i = 0; i < nbServos; i++) {
    Serial.print(servoPositions[i]);
    if (i < nbServos - 1) {
      Serial.print(", ");
    }
  }
  Serial.println("]");
}

void HandController::setToPassiveMode() {
  for (size_t i = 0; i < nbServos; i++) {
    int id = expectedServoIDs[i];
    sam.PassiveMode(id);
  }
}

void HandController::moveToRaw(int rawAngle) {
  for (size_t i = 0; i < nbServos; i++) {
    int id = expectedServoIDs[i];
    sam.PosSend(id, rawAngle, 4);
  }
}

void HandController::moveToZero() {
  for (size_t i = 0; i < nbServos; i++) {
    int id = expectedServoIDs[i];
    ServoMotionRange range = servoMotionRanges[i];
    sam.PosSend(id, 0 + range.center, speedLevel);
    servoPositions[i] = 0 + range.center;
    servoTargets[i] = 0 + range.center;
    servoTargetTimes[i] = millis();
    servoStartPositions[i] = 0 + range.center;
  }
}

// void HandController::setLedColor(int id, uint32_t color) {
//   if (id >= 1 && id <= NUM_LEDS) {
//     strip.setPixelColor(id - 1, color);
//     strip.show();
//   }
// }