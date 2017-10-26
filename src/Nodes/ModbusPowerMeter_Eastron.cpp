#include "ModbusPowerMeter_Eastron.hpp"

Node_ModbusPowerMeter_Eastron::Node_ModbusPowerMeter_Eastron(const char* name, const uint8_t NodeAddress) :
	HomieNode(name, "ModbusPowerMeter",
		[](String property, HomieRange range, String value) { return false; }),
	_name(name),				// node Name
	_NodeAddress(NodeAddress)		// Node on Bus
	{
	advertise(String("Node_ModbusPowerMeter_Eastron_" + _name).c_str());

	if (_debug) {
		Homie.getLogger() << "[Node_ModbusPowerMeter_Eastron-" << _name << "] constructor finished" << endl;
	}
}


void Node_ModbusPowerMeter_Eastron::setup() {

	Homie.getLogger() << "[Node_ModbusPowerMeter_Eastron-" << _name << "] Setup finished \n\n************************\n\n\n" << endl;
}

void Node_ModbusPowerMeter_Eastron::loop() {

	if (millis() < _lastLoopUpdate) _lastLoopUpdate = 0; //Correct the 29day overflow

	if (millis() - _lastLoopUpdate >= _Interval) {
		Node_ModbusPowerMeter_Eastron::update();
		_lastLoopUpdate = millis();
	}


} // loop

void Node_ModbusPowerMeter_Eastron::setDebug(bool debug) {
  _debug = debug;
}

void Node_ModbusPowerMeter_Eastron::setInterval(uint16_t interval) {
	_Interval = interval * 1000UL;
}

void Node_ModbusPowerMeter_Eastron::update() {

	//TODO

	if (_debug) {
		Homie.getLogger() << "[Node_ModbusPowerMeter_Eastron-" << _name << "] update: "

		<< endl;
	}
}

uint8_t Node_ModbusPowerMeter_Eastron::getNodeAddress() {
    return this->_NodeAddress;
}
