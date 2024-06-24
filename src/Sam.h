/*
  Description :
  SAM is an acronym for smart-actuator-module and high-performance motor
  developed by ROBOBUILDER.

  void PosSend(ID, Position, SpeedLevel);
  void WheelMode(ID, RotDir, RotSpd);
  void BreakMode();
  void PassiveMode(ID);
  void PosRead(ID);

  // �� SAM-3 : Not supported
  void SyncPosSend(LastID, SpeedLevel, Position);
  Parameters :
  ID		      : Id number (0 ~ 31)
  Position    : Target Positon (0 ~ 254)
  SpeedLevel 	: Moving speed & torq (0 ~ 4)
  RotDir	    : Direction of rotation 3(CCW), 4(CW)
  RotSpd      : Rotation speed. 0 ~ 15
  LastID 	    : The last id (0~31) of SAM connected.
*/

#ifndef Sam_h
#define Sam_h

#include "Arduino.h"
#include "SoftwareSerial.h"

#define CCW 3
#define CW 4

class TSam {
 public:
  TSam(int rx, int tx);
  void PosSend(unsigned char ID, unsigned char Position,
               unsigned char SpeedLevel);
  void WheelMode(unsigned char ID, unsigned char RotDir, unsigned char RotSpd);
  void BreakMode();
  void PassiveMode(unsigned char ID);
  bool CheckServo(unsigned char ID);
  unsigned char PosRead(unsigned char ID);
  int *findServos(int &count);

  // Not working yet
  void ChangeID(unsigned char oldID, unsigned char newID);

 private:
  SoftwareSerial mySerial;
};

#endif