#pragma once

#include <Homie.hpp>

class Node_DigitalIn: public HomieNode {
private:
  String _name;
	int _PinNumber;
  bool _debug = true;
  uint16_t _Interval = 2000UL;
	unsigned long lastLoopUpdate = 0;

  void update();
public:
	Node_DigitalIn(const char* name, const uint8_t PinNumber);

  void setup();
	void loop();

  void setDebug(bool debug);
  void setInterval(uint16_t interval);

};
