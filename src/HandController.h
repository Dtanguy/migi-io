// HandController.h

#ifndef HAND_CONTROLLER_H
#define HAND_CONTROLLER_H

#include "Arduino.h"
#include "Sam.h"

struct ServoMotionRange {
  int min;
  int max;
  int center;
};

class HandController {
 public:
  HandController();
  void checkServo();
  void move(int id, int deg);
  void updateMotion();
  void updateServoPositions();
  void printServoPositions();
  void setToPassiveMode();
  void moveToRaw(int rawAngle);
  void moveToZero();

 private:
  static TSam sam;
  short speedLevel = 0;
  short nbServos = 8;
  int expectedServoIDs[8] = {1, 2, 3, 4, 5, 6, 7, 8};
  ServoMotionRange servoMotionRanges[8] = {
      // 1
      {0, 180, 130},
      // 2
      {0, 180, 130},
      // 3
      {0, 180, 110},
      // 4
      {0, 230, 130},
      // 5
      {0, 180, 130},
      // 6
      {0, 230, 120},
      // 7
      {0, 180, 140},
      // 8
      {0, 230, 130}};
  int servoPositions[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  int servoTargets[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  int servoStartPositions[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  unsigned long servoTargetTimes[8] = {millis(), millis(), millis(), millis(),
                                       millis(), millis(), millis(), millis()};
};

#endif