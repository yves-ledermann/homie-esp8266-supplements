#include "DYP-ME007Y-Ultrasonic-Sensor-Node.hpp"

Ultrasonic_ME007Y_Serial::Ultrasonic_ME007Y_Serial(const char* name, const uint8_t PinNumber) :
		HomieNode("Sensor", "Sensors_t",
			[](String property, HomieRange range, String value) { return false; }),
		_name(name),
		_PinNumber(PinNumber)
{
	if (_debug) {
		Homie.getLogger() << "[Ultrasonic_ME007Y_Serial-Node] constructor" << endl;
	}
}

void Ultrasonic_ME007Y_Serial::setup() {
}

void Ultrasonic_ME007Y_Serial::loop() {
}


void Ultrasonic_ME007Y_Serial::setDebug(bool debug) {
  _debug = debug;
}

void Ultrasonic_ME007Y_Serial::setInterval(uint16_t interval) {
	_Interval = interval * 1000UL;
}
