#include "ds18b20MultiNode.hpp"

ds18b20MultiNode::ds18b20MultiNode(const char* name, int PinNumber) :
		HomieNode("Sensor", "Sensors_t",
			[](String property, HomieRange range, String value) { return false; }),
		_name(name),
		_PinNumber(PinNumber)
{
	Homie.getLogger() << "[ds18b20MultiNode] constructor" << endl;
}

void ds18b20MultiNode::setup() {
	// Bus & Sensors
		OneWire oneWire(_PinNumber);

	// init bus
			sensor.setOneWire(&oneWire);
			sensor.begin();

	// locate devices on the bus
 			Homie.getLogger() << "\n\n\n" << "[ds18b20MultiNode-" << _name << "] Get num of devices on Bus" << endl;
			sensorsonBus = sensor.getDeviceCount();
			Homie.getLogger() << "[ds18b20MultiNode-" << _name << "] Found " << sensorsonBus << " devices on Bus" << endl;
	// report parasite power requirements
	 		Homie.getLogger() << "[ds18b20MultiNode-" << _name << "] Parasite power on Bus is: " << (sensor.isParasitePowerMode() ? " ON " : " OFF ") << endl;
	// set Temperature Precision on bus
	 		sensor.setResolution(TEMPERATURE_PRECISION);
			delay(50);
			Homie.getLogger() << "[ds18b20MultiNode-" << _name << "] Resolution on Bus is: " << sensor.getResolution() << endl;
		// start Temp conversion for the first Time (else no Temp readings during setup)
		// Initialy request conversion
		sensor.requestTemperatures();
		delay(750);

	// actions for every Sensor on bus
		Homie.getLogger() << "[ds18b20MultiNode-" << _name << "] Start Init Sensors\n\n\n\n********************\n" << endl;
	 for (uint8_t s=0; s < sensorsonBus ; s++)
		 {
			 // (Write address to Array based on Sensor Number)
			 // assigns the next address found to Address[i]
		 		if (sensor.getAddress(sensorValues[s].Address, s))
			 		{
						Homie.getLogger() << "[ds18b20MultiNode-" << _name << "] Sensor "<< s <<" with Address " << getAddressReadable(sensorValues[s].Address) << " found" << endl;
			 		}
		 		else
		 			{
			 			Homie.getLogger() << "[ds18b20MultiNode-" << _name << "] Unable to find any address" << endl;
		 			}

			// print Infos about Sensor
				Homie.getLogger() << "[ds18b20MultiNode-" << _name << "] Infos about Sensor "<< s <<" with Address " << getAddressReadable(sensorValues[s].Address) << ":" << endl;
				Homie.getLogger() << "[ds18b20MultiNode-" << _name << "] Resolution: " << sensor.getResolution(sensorValues[s].Address) << endl;
		 		Homie.getLogger() << "[ds18b20MultiNode-" << _name << "] Bits: Alarm low / high / has alarm " << (sensor.getLowAlarmTemp(sensorValues[s].Address), DEC) << " / " << (sensor.getHighAlarmTemp(sensorValues[s].Address), DEC) << " / ";
				Homie.getLogger() << (sensor.hasAlarm(sensorValues[s].Address) ? " TRUE " : " FALSE ") << endl;
				Homie.getLogger() << "[ds18b20MultiNode-" << _name << "] Temperature: " << sensor.getTempC(sensorValues[s].Address) << "\n" << endl;

		 }; //for Sensor

		 Homie.getLogger() << "[ds18b20MultiNode-" << _name << "] Setup finished \n\n************************\n\n\n" << endl;
};

void ds18b20MultiNode::loop() {
	// Bus & Sensors
		OneWire oneWire(_PinNumber);
	// init bus
			sensor.setOneWire(&oneWire);
			sensor.begin();

	if (millis() - lastLoopSensor >= 5000UL || lastLoopSensor == 0) {

// get Temperature
	  for (int y=0; y < sensorsonBus ; y++) {
	 	 sensorValues[y].Temp=sensor.getTempC(sensorValues[y].Address);
	 	 Homie.getLogger() << "[ds18b20MultiNode-" << _name << "] Temperature for the sensor " << y << " with Address " << getAddressReadable(sensorValues[y].Address) << " is " << sensorValues[y].Temp << endl;
	  }
		// call sensors.requestTemperatures() to issue a global temperature
	  // request to all devices on the bus
		sensor.requestTemperatures();
		lastLoopSensor = millis();
		Homie.getLogger() << "[ds18b20MultiNode-" << _name << "] temp loop" << endl;

		Homie.getLogger() << "[ds18b20MultiNode-" << _name << "] loop finished" << endl;
		//setProperty("degrees").send(String(temp));
}
};



char* ds18b20MultiNode::getAddressReadable(uint8_t Address[8]) {

	sprintf(adressReadable, "%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x\0", Address[0],Address[1],Address[2],Address[3],Address[4],Address[5],Address[6],Address[7]);
	return adressReadable;
};
