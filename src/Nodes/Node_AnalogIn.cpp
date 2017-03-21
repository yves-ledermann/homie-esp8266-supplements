#include "Node_AnalogIn.hpp"


Node_AnalogIn::Node_AnalogIn(const char* name, const uint8_t PinNumber) :
		HomieNode("Sensor", "AnalogIn",
			[](String property, HomieRange range, String value) { return false; }),
		_name(name),
		_PinNumber(PinNumber)
{
  advertise(String("AnalogIn_" + _name).c_str());

  if (_debug) {
		Homie.getLogger() << "[Node_AnalogIn-" << _name << "] constructor finished" << endl;
	}
}

void Node_AnalogIn::setup() {

	Homie.getLogger() << "[Ultrasonic_Serial_Node-" << _name << "] Setup finished \n\n************************\n\n\n" << endl;
}

void Node_AnalogIn::loop() {

	if (millis() < lastLoopUpdate) lastLoopUpdate = 0; //Correct the 29day overflow

	if (millis() - lastLoopUpdate >= _Interval) {
		Node_AnalogIn::update();
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

void Node_AnalogIn::setDebug(bool debug) {
  _debug = debug;
}

void Node_AnalogIn::setInterval(uint16_t interval) {
	_Interval = interval * 1000UL;
}

void Node_AnalogIn::update(){
  uint16_t value = analogRead(_PinNumber);

  setProperty("AnalogIn_" + _name).send(String(value));

}
