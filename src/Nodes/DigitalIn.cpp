#include "DigitalIn.hpp"


Node_DigitalIn::Node_DigitalIn(const char* name, const uint8_t PinNumber) :
		HomieNode("Sensor", "DigitalIn",
			[](String property, HomieRange range, String value) { return false; }),
		_name(name),
		_PinNumber(PinNumber)
{
  advertise(String("Node_DigitalIn_" + _name).c_str());

  if (_debug) {
		Homie.getLogger() << "[Node_DigitalIn-" << _name << "] constructor finished" << endl;
	}
}

void Node_DigitalIn::setup() {
	pinMode(_PinNumber, INPUT_PULLUP);
	Homie.getLogger() << "[Node_DigitalIn_" << _name << "] Setup finished \n\n************************\n\n\n" << endl;
}

void Node_DigitalIn::loop() {

	if (millis() < lastLoopUpdate) lastLoopUpdate = 0; //Correct the 29day overflow

	if (millis() - lastLoopUpdate >= _Interval) {
		Node_DigitalIn::update();
		lastLoopUpdate = millis();
	}

/*
	if (millis() - lastLoopSensor >= _Interval) {
		Homie.getLogger() << "[Ultrasonic_Serial_Node-" << _name << "]" << _distance << endl;
		setProperty("Distance").send(String(_distance).c_str());
		lastLoopSensor = millis();
	}
*/

} // loop

void Node_DigitalIn::setDebug(bool debug) {
  _debug = debug;
}

void Node_DigitalIn::setInterval(uint16_t interval) {
	_Interval = interval * 1000UL;
}

void Node_DigitalIn::update(){
  bool value = digitalRead(_PinNumber);

  setProperty("Node_DigitalIn_" + _name).send(String(value));

}
