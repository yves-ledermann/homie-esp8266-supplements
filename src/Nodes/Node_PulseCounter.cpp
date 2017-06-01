#include "Node_PulseCounter.hpp"


Node_PulseCounter::Node_PulseCounter(const char* name, const uint8_t PinNumber) :
		HomieNode("Sensor", "PulseCounter",
			[](String property, HomieRange range, String value) { return false; }),
		_name(name),
		_PinNumber(PinNumber)
{
	anchor = this;
	advertise(String("Node_PulseCounter_" + _name).c_str());

  if (_debug) {
		Homie.getLogger() << "[Node_PulseCounter-" << _name << "] constructor finished" << endl;
	}
}

void Node_PulseCounter::setup() {
	pinMode(_PinNumber, INPUT_PULLUP);
	attachInterrupt(_PinNumber, Node_PulseCounter::marshall, FALLING);
	Homie.getLogger() << "[Node_PulseCounter-" << _name << "] Setup finished \n\n************************\n\n\n" << endl;
}

void Node_PulseCounter::loop() {

	if (millis() < lastLoopUpdate) lastLoopUpdate = 0; //Correct the 29day overflow

	if (millis() - lastLoopUpdate >= _Interval) {
		Node_PulseCounter::update();
		lastLoopUpdate = millis();
	}


} // loop

void Node_PulseCounter::setDebug(bool debug) {
  _debug = debug;
}

void Node_PulseCounter::setInterval(uint16_t interval) {
	_Interval = interval * 1000UL;
}

void Node_PulseCounter::update() {
	if (_debug) {
		Homie.getLogger() << "[Node_PulseCounter-" << _name << "] update: "<< pulseCount << "\n"<< endl;
	}
}

void Node_PulseCounter::isr() {
      pulseCount++;
    }
void Node_PulseCounter::marshall() {
  anchor->isr();
}
Node_PulseCounter* Node_PulseCounter::anchor = NULL;
