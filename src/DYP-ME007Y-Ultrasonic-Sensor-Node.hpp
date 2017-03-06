#pragma once

#include <Homie.hpp>

class Ultrasonic_ME007Y_Serial: public HomieNode {
private:
  String _name;
	int _PinNumber;
  bool _debug = true;
  uint16_t _Interval = 15000UL;
	unsigned long lastLoopSensor = 0;

public:
	Ultrasonic_ME007Y_Serial(const char* name, const uint8_t PinNumber);

  void setup();
	void loop();

  void setDebug(bool debug);
  void setInterval(uint16_t interval);

};
