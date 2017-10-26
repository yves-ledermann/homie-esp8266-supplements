#pragma once

#include <Homie.hpp>
//#include <SDM.h>

class Node_ModbusPowerMeter_Eastron: public HomieNode {

  public:
    Node_ModbusPowerMeter_Eastron(const char* name, const uint8_t NodeAddress);

    void setup();
  	void loop();

    void setDebug(bool debug);
    void setInterval(uint16_t interval);

    uint8_t getNodeAddress();

  private:
    String _name;
  	uint8_t _NodeAddress;

    bool _debug = false;

    void update();
    uint16_t _Interval = 1000UL;
    uint32_t _lastLoopUpdate = 0;

};
