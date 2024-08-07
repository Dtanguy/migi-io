#include "Sam.h"

TSam::TSam(int rx, int tx) : mySerial(rx, tx) { mySerial.begin(115200); }

void TSam::PosSend(unsigned char ID, unsigned char Position,
                   unsigned char SpeedLevel) {
  unsigned char Data1 = 0, CheckSum = 0;
  Data1 = (SpeedLevel << 5) | ID;
  CheckSum = (Data1 ^ Position) & 0x7f;

  mySerial.write(0xff);
  mySerial.write(Data1);
  mySerial.write(Position);
  mySerial.write(CheckSum);
}

void TSam::WheelMode(unsigned char ID, unsigned char RotDir,
                     unsigned char RotSpd) {
  unsigned char Data1 = 0, Data2 = 0, CheckSum = 0;
  Data1 = (6 << 5) | ID;
  Data2 = (0xff & (RotDir << 4)) | (0xff & RotSpd);
  CheckSum = (Data1 ^ Data2) & 0x7f;

  mySerial.write(0xff);
  mySerial.write(Data1);
  mySerial.write(Data2);
  mySerial.write(CheckSum);
}

void TSam::BreakMode() {
  unsigned char Data1 = 0, Data2 = 0, CheckSum = 0;
  Data1 = (6 << 5) | 31;
  Data2 = 0x20;
  CheckSum = (Data1 ^ Data2) & 0x7f;

  mySerial.write(0xff);
  mySerial.write(Data1);
  mySerial.write(Data2);
  mySerial.write(CheckSum);
}

void TSam::PassiveMode(unsigned char ID) {
  unsigned char Data1 = 0, Data2 = 0, CheckSum = 0;
  Data1 = (6 << 5) | ID;
  Data2 = 0x10;
  CheckSum = (Data1 ^ Data2) & 0x7f;

  mySerial.write(0xff);
  mySerial.write(Data1);
  mySerial.write(Data2);
  mySerial.write(CheckSum);
}

// Function to check if a servo is connected
bool TSam::CheckServo(unsigned char ID) {
  mySerial.setTimeout(20);
  unsigned char Data1 = (5 << 5) | ID;
  unsigned char CheckSum = (Data1) & 0x7f;
  mySerial.flush();

  mySerial.write((uint8_t)0xff);
  mySerial.write(Data1);
  mySerial.write((uint8_t)0);
  mySerial.write(CheckSum);
  delay(50);

  if (mySerial.available() != 2) {
    // Expected data not available
    return false;
  }

  char bytesReturned[2] = {0, 0};
  if (mySerial.readBytes(bytesReturned, 2) != 2) {
    // readBytes times out
    return false;
  }

  mySerial.flush();
  return true;
}

// Function to read the position of a servo quickly
unsigned char TSam::PosRead(unsigned char ID) {
  mySerial.setTimeout(20);
  unsigned char Data1 = 0, CheckSum = 0;
  char bytesReturned[2] = {0, 0};

  Data1 = (5 << 5) | ID;
  CheckSum = (Data1) & 0x7f;

  while (mySerial.available()) mySerial.readBytes(bytesReturned, 1);
  mySerial.write((uint8_t)0xff);
  mySerial.write(Data1);
  mySerial.write((uint8_t)0);
  mySerial.write(CheckSum);

  mySerial.readBytes(bytesReturned, 2);
  return (unsigned char)bytesReturned[1];
}

int *TSam::findServos(int &count) {
  static int servos[31];
  count = 0;
  for (unsigned char ID = 0; ID < 31; ID++) {
    bool response = this->CheckServo(ID);
    // Serial.println(response);
    if (response) {
      servos[count++] = ID;
    }
  }
  return servos;
}

void TSam::ChangeID(unsigned char oldID, unsigned char newID) {
  mySerial.write(0xFF);                                            // Start byte
  mySerial.write(0xFF);                                            // Start byte
  mySerial.write(oldID);                                           // Servo ID
  mySerial.write(0x04);                                            // Length
  mySerial.write(0x03);                                            // Write data
  mySerial.write(0x03);                                            // ID address
  mySerial.write(newID);                                           // New ID
  unsigned char CheckSum = ~(oldID + 0x04 + 0x03 + 0x03 + newID);  // Checksum
  mySerial.write(CheckSum);
}