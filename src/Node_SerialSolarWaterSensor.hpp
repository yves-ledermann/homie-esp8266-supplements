#pragma once
/*
Solar Water Heater Sensor
The GHBO1/SHBO1 is a temperature and level sensor for solar water heaters. It is manufactured by Sunhome Technology Co.,Ltd & Green Homebuilders Inc. The manufacturer does not provide information on how to use it. They only sell it as a replacement part for solar water heater controllers like the SR500.
got all infos from http://hack4life.pbworks.com/w/page/75653090/Arduino%20Solar%20Water%20Heater%20Sensor


BLACK = 0V
RED = max 12V ???
WHITE = Signal

*/

// Extracted from http://hack4life.pbworks.com/Arduino%20Solar%20Water%20Heater%20Sensor


#include <Homie.hpp>

class Node_SolarWaterSens: public HomieNode {
private:
  String _name;
	int _PinNumber;
  bool _debug = true;
  uint16_t _Interval = 15000UL;
	uint32_t lastLoopUpdate = 0;

  void update();
  int expectPulse(int val);
  bool readTempNLevelSensor(char _PinNumber, char &temp, char &level);
  void handleSensorInterrupt();
public:
	Node_SolarWaterSens(const char* name, const uint8_t PinNumber);

  void setup();
	void loop();

  void setDebug(bool debug);
  void setInterval(uint16_t interval);

};
