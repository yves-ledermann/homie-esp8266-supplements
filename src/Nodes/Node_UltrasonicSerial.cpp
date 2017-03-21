#include "Node_UltrasonicSerial.hpp"

Node_UltrasonicSerial::Node_UltrasonicSerial(const char* name, const uint8_t PinNumber) :
		HomieNode("Sensor", "Sensors_t",
			[](String property, HomieRange range, String value) { return false; }),
		_name(name),
		_PinNumber(PinNumber),
		swSerial1(PinNumber, NULL),
		dataBuffer(75.00, 21, 3.50)
{
	if (_debug) {
		Homie.getLogger() << "[Node_UltrasonicSerial-" << _name << "] constructor finished" << endl;
	}
}

void Node_UltrasonicSerial::setup() {
	swSerial1.begin(9600);
	serialFlush();
	Homie.getLogger() << "[Node_UltrasonicSerial-" << _name << "] Setup finished \n\n************************\n\n\n" << endl;
}

void Node_UltrasonicSerial::loop() {

	if (millis() < lastLoopSensor) lastLoopSensor = 0; //Correct the 29day overflow

	if (millis() - lastLoopUpdate >= 200UL) {
		Node_UltrasonicSerial::updateDistance();
		lastLoopUpdate = millis();
	}


	if (millis() - lastLoopSensor >= _Interval) {
		Homie.getLogger() << "[Node_UltrasonicSerial-" << _name << "]" << _distance << endl;
		setProperty("Distance").send(String(_distance).c_str());
		lastLoopSensor = millis();
	}


} // loop

void Node_UltrasonicSerial::setDebug(bool debug) {
  _debug = debug;
}

void Node_UltrasonicSerial::setInterval(uint16_t interval) {
	_Interval = interval * 1000UL;
}

void Node_UltrasonicSerial::serialFlush(){
	// Puffer leeren
	// flush the receive buffer
	while(swSerial1.available() > 0) {
    char t = swSerial1.read();
  }
}

void Node_UltrasonicSerial::updateDistance() {

	if (_debug) {
		//Homie.getLogger() << "[Node_UltrasonicSerial-" << _name << "] getDistance" << endl;
	}
	String outText;
	int distance;
	//byte readByte;
	//byte read_buffer[4];
	byte crcCalc;
	byte mybuffer[4] = { 0 };
	byte bitpos = 0;

	//
	// Prüfen ob Daten in Port-Puffer
	// mindestens 4 bytes für ein datensatz
	if (swSerial1.available() < 4) {
		return;
	}

	//
	//  Aus dem Puffer lesen
	//
	while (swSerial1.available()) {
		// the first byte is ALWAYS 0xFF and I'm not using the checksum (last byte)
		// if your print the swSerial1.read() data as HEX until it is not available, you will get several measures for the distance (FF-XX-XX-XX-FF-YY-YY-YY-FF-...). I think that is some kind of internal buffer, so I'm only considering the first 4 bytes in the sequence (which I hope that are the most recent! :D )
		if (bitpos < 4) {
			mybuffer[bitpos++] = swSerial1.read();
		} else
			break;
	}

	if (_debug) {
		Homie.getLogger() << "[Node_UltrasonicSerial-" << _name << "] Buffer Debug:\n" << endl;
		Homie.getLogger() << "Buffer[0]...[3]: " << mybuffer[0] << " " << mybuffer[1] << " " << mybuffer[2] << " " << mybuffer[3] << endl;
	}

	// flush the receive buffer
	//serialFlush();

	//
	// Pufferanalyse
	//
	if (mybuffer[00] != 0xff) {
		if (_debug) {
			Homie.getLogger() << "[Node_UltrasonicSerial-" << _name << "] nitcht anfang der Daten" << endl;
		}
		serialFlush();
		return; // dies nicht der Anfang der Daten
	};

	crcCalc = mybuffer[00] + mybuffer[01] + mybuffer[02];
	if (mybuffer[03] != crcCalc) {
		if (_debug) {
			Homie.getLogger() << "[Node_UltrasonicSerial-" << _name << "] CRC Error buffer[03] crcCalc"<< mybuffer[03] << " "<< crcCalc << endl;
		}
		return; // Prüfsumme Datenpaket nicht übereinstimmt
	};
	//
	// Berechnung Abstand

	//
	distance = (mybuffer[01] * 0xff) + mybuffer[02];
	if (_debug) {
		Homie.getLogger() << "[Node_UltrasonicSerial-" << _name << "] berechnete Distanz in mm" << distance << endl;
	}

	//if (distance > 200 || distance < 2000){
		dataBuffer.write(distance / 10.0);
	//}

	_distance = dataBuffer.readMedian();
	if (_debug) {
		Homie.getLogger() << "[Node_UltrasonicSerial-" << _name << "] gefilterte Distanz in cm" << dist << endl;
	}
	return;
}
