#pragma once

#include <Homie.hpp>

#include <SoftwareSerial.h>
#include <HampelFilter.h>

class Node_UltrasonicSerial: public HomieNode {
private:
  String _name;
	int _PinNumber;
  bool _debug = false;
  uint16_t _Interval = 5000UL;
	unsigned long lastLoopSensor = 0;
  unsigned long lastLoopUpdate = 0;
  SoftwareSerial swSerial1;
  HampelFilter dataBuffer;
  float _distance = 0.0;
  float dist = 0.0;
  int ionstart = 0;


  void serialFlush();
  void updateDistance();
public:
	Node_UltrasonicSerial(const char* name, const uint8_t PinNumber);

  void setup();
	void loop();

  void setDebug(bool debug);
  void setInterval(uint16_t interval);

};
