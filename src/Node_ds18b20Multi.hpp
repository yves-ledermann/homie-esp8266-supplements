#pragma once

#include <Homie.hpp>
#include <OneWire.h>
#include <DallasTemperature.h>
#define MaxSensorCount 10  // max num of sensors to be defined per Bus
#define TEMPERATURE_PRECISION 12

class Node_ds18b20Multi: public HomieNode {
private:
  String _name;
	int _PinNumber;
  bool _debug = true;
  uint8_t _precision = TEMPERATURE_PRECISION;
  uint16_t _tempInterval = 15000UL;
	unsigned long lastLoopSensor = 0;
  DallasTemperature sensor;
  OneWire oneWire;
	int sensorsonBus;

  int state = 10;
  int stateold = state;

	char* getAddressReadable(uint8_t Address[8]);
	char adressReadable[20];

  // To store the current time for delays
  uint32_t tsTemp;

  // Struct Array for DS18B20 Adress / Temp
  typedef struct
  {
   uint8_t Address[8];
   float Temp = -127.0;
  }  tempSensor;
  tempSensor sensorValues[MaxSensorCount];


public:
	Node_ds18b20Multi(const char* name, const uint8_t PinNumber);

  void setup();
	void loop();

  float getTemperature(int SensorNumber);
  void setDebug(bool debug);
  void setInterval(uint16_t interval);
  void setResolution(uint8_t precision);

};
