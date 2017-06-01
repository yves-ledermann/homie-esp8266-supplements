#pragma once

#include <Homie.hpp>

class Node_PulseCounter: public HomieNode {

private:
  String _name;
	int _PinNumber;
  bool _debug = true;
  uint16_t _Interval = 15000UL;
	uint32_t lastLoopUpdate = 0;
  unsigned int pulseCount = 0;
  void update();

  void handleInterrupt ();

  static void marshall();
  void isr();
  static Node_PulseCounter* anchor;
  volatile byte isrFlag;

public:
	Node_PulseCounter(const char* name, const uint8_t PinNumber);

  void setup();
	void loop();


  void setDebug(bool debug);
  void setInterval(uint16_t interval);

};

/*
- interval
-


*/
