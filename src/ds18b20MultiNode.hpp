#pragma once

#include <Homie.hpp>
#include <OneWire.h>
#include <DallasTemperature.h>
#define MaxSensorCount 10  // max num of sensors to be defined per Bus
#define TEMPERATURE_PRECISION 12

class ds18b20MultiNode: public HomieNode {
private:
  String _name;
	int _PinNumber;

	int sensorsonBus;
	unsigned long lastLoopSensor = 0;
	DallasTemperature sensor;
	char* getAddressReadable(uint8_t Address[8]);
	char adressReadable[24];
	uint8_t Address[8];
public:
	ds18b20MultiNode(const char* name, int PinNumber);
	void setup();
	void loop();

	// Struct Array for DS18B20 Adress / Temp
		typedef struct
		 {
			uint8_t Address[8];
			float Temp;
		 }  tempSensor;
		 tempSensor sensorValues[MaxSensorCount];



};
