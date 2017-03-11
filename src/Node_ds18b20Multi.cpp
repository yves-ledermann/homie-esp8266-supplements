#include "Node_ds18b20Multi.hpp"

Node_ds18b20Multi::Node_ds18b20Multi(const char* name, const uint8_t PinNumber) :
		HomieNode("Sensor", "Sensors_t",
			[](String property, HomieRange range, String value) { return false; }),
		_name(name),
		_PinNumber(PinNumber),
		oneWire(PinNumber)
{
	if (_debug) {
		Homie.getLogger() << "[Node_ds18b20Multi] constructor" << endl;
	}
}

void Node_ds18b20Multi::setup() {


	// Bus & Sensors
		//	OneWire oneWire(_PinNumber);
	// init bus
			sensor.setOneWire(&oneWire);
			sensor.begin();
	// locate devices on the bus
 			if (_debug) {
				Homie.getLogger() << "\n\n\n" << "[Node_ds18b20Multi-" << _name << "] Get num of devices on Bus" << endl;
			}
			sensorsonBus = sensor.getDeviceCount();
			if (_debug) {
				Homie.getLogger() << "[Node_ds18b20Multi-" << _name << "] Found " << sensorsonBus << " devices on Bus" << endl;
			}
	// report parasite power requirements
	 		if (_debug) {
				Homie.getLogger() << "[Node_ds18b20Multi-" << _name << "] Parasite power on Bus is: " << (sensor.isParasitePowerMode() ? " ON " : " OFF ") << endl;
			}
	// set Temperature Precision on bus
	 		sensor.setResolution(_precision);
			delay(50);
			if (_debug) {
				Homie.getLogger() << "[Node_ds18b20Multi-" << _name << "] Resolution on Bus is: " << sensor.getResolution() << endl;
			}
		// start Temp conversion for the first Time (else no Temp readings during setup)
		// Initialy request conversion
		sensor.requestTemperatures();
		delay(750);

	// actions for every Sensor on bus
		if (_debug) {
			Homie.getLogger() << "[Node_ds18b20Multi-" << _name << "] Start Init Sensors\n\n\n\n********************\n" << endl;
		}

	 for (uint8_t s=0; s < sensorsonBus ; s++)
		 {
			 // (Write address to Array based on Sensor Number)
			 // assigns the next address found to Address[i]
		 		if (sensor.getAddress(sensorValues[s].Address, s))
			 		{
						Homie.getLogger() << "[Node_ds18b20Multi-" << _name << "] Sensor "<< s <<" with Address " << getAddressReadable(sensorValues[s].Address) << " found" << endl;
			 		}
		 		else
		 			{
			 			Homie.getLogger() << "[Node_ds18b20Multi-" << _name << "] Unable to find any address" << endl;
		 			}

			// print Infos about Sensor
				if (_debug) {
					Homie.getLogger() << "[Node_ds18b20Multi-" << _name << "] Infos about Sensor "<< s <<" with Address " << getAddressReadable(sensorValues[s].Address) << ":" << endl;
					Homie.getLogger() << "[Node_ds18b20Multi-" << _name << "] Resolution: " << sensor.getResolution(sensorValues[s].Address) << endl;
			 		Homie.getLogger() << "[Node_ds18b20Multi-" << _name << "] Bits: Alarm low / high / has alarm " << (sensor.getLowAlarmTemp(sensorValues[s].Address), DEC) << " / " << (sensor.getHighAlarmTemp(sensorValues[s].Address), DEC) << " / ";
					Homie.getLogger() << (sensor.hasAlarm(sensorValues[s].Address) ? " TRUE " : " FALSE ") << endl;
					Homie.getLogger() << "[Node_ds18b20Multi-" << _name << "] Temperature: " << sensor.getTempC(sensorValues[s].Address) << "\n" << endl;
				}

		 }; //for Sensor

		 Homie.getLogger() << "[Node_ds18b20Multi-" << _name << "] Setup finished \n\n************************\n\n\n" << endl;
};

void Node_ds18b20Multi::loop() {
	// The keyword "static" makes sure the variable
  // isn't destroyed after each loop
	//	static int state = S_WAIT;
	//	static int stateold = state;


	//  static int state = 10;
	// 	 int stateold = state;

	// Log State Changes
	if (state != stateold) {
		if (_debug) {
			Homie.getLogger() << "[Node_ds18b20Multi-" << _name << "] loopStateMachine State: "<< state << endl;
		}
		stateold = state;
	}

	if (sensorsonBus > 0)  {
		  switch (state)
		  {
		    case 10: // Wait to get Temperatures
					if (millis() < lastLoopSensor) lastLoopSensor = 0; //Correct the 29day overflow
					if (millis() - lastLoopSensor >= _tempInterval || lastLoopSensor == 0) {
						sensor.requestTemperatures();
						tsTemp = millis();
						//lastLoopSensor = millis();
						state = 20;
						}
					break;

		    case 20: // get Temperatures
					if (millis() - tsTemp >= 1000UL) {
						for (int y=0; y < sensorsonBus ; y++) {
							// Get the Temperature from the Sensor
							float temp = -127.0;
							temp = Node_ds18b20Multi::sensor.getTempC(sensorValues[y].Address);
							// Write to log and MQTT
							if (_debug) {
								Homie.getLogger() << "[Node_ds18b20Multi-" << _name << "] Temperature for the sensor " << y << " with Address " << getAddressReadable(sensorValues[y].Address) << " is " << temp << endl;
							}
							if (temp == -127.0) { // ERROR
								Homie.getLogger() << "[Node_ds18b20Multi-" << _name << "] ERROR get Temperature on sensor " << y << " with Address " << getAddressReadable(sensorValues[y].Address) << " got " << temp << endl;
								setProperty(getAddressReadable(sensorValues[y].Address)).send("ERROR");
							}
							else { // right Temperature
								sensorValues[y].Temp = temp;
								setProperty(getAddressReadable(sensorValues[y].Address)).send(String(sensorValues[y].Temp).c_str());
							}

						} // for
						lastLoopSensor = millis();
					 	state = 10;
					 }
		      break;
		  } // end of switch
	} // if sensors on bus
}; // loop


char* Node_ds18b20Multi::getAddressReadable(uint8_t Address[8]) {

	sprintf(adressReadable, "%02x%02x%02x%02x%02x%02x%02x%02x\0", Address[0],Address[1],Address[2],Address[3],Address[4],Address[5],Address[6],Address[7]);
	return adressReadable;
};

void Node_ds18b20Multi::setDebug(bool debug) {
  _debug = debug;
}

void Node_ds18b20Multi::setInterval(uint16_t interval) {
	_tempInterval = interval * 1000UL;
}


void Node_ds18b20Multi::setResolution(uint8_t precision) {
	_precision = precision;
	sensor.setResolution(_precision);
	delay(50);
	if (_debug) {
		Homie.getLogger() << "[Node_ds18b20Multi-" << _name << "] Resolution on Bus is: " << sensor.getResolution() << endl;
	}
}

float Node_ds18b20Multi::getTemperature(int SensorNumber) {
	return sensorValues[SensorNumber].Temp;
}
