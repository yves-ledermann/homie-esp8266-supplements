/*
 * Example Sketch to Test homie-esp8266-supplements library
 * with all Nodes and Functions
 *
*/
// -----------------------------------------------------------------------------
// Configurations
// -----------------------------------------------------------------------------
#define SERIAL_BAUDRATE         115200
#define OTA_PASS                "assugrin"
#define OTA_PORT                8266

//------------------------------------------------------------------------------
// APP
//------------------------------------------------------------------------------

#define APP_NAME                "homie-SolarRoof"
#define APP_VERSION             "2.0.1"
#define APP_AUTHOR              "yves@laedis.ch"
#define APP_WEBSITE             "http://yves.laedis.ch"

// -----------------------------------------------------------------------------
// HARDWARE
// -----------------------------------------------------------------------------

#define DS18B20_BUS1            2 // Bus0 On Pin D3?
#define AnalogIn_PIN            A0 // Analog 0
#define USonic_PIN1             4 // Bus1 on Pin D2?
#define PulseCounter_PIN        5 //
#define SolarWaterSensor_PIN    16 //
#define DigitalIn_PIN           7 //

// -----------------------------------------------------------------------------
// INCLUDES
// -----------------------------------------------------------------------------
#include <Arduino.h>
#include <homie-esp8266-supplements.h>

// -----------------------------------------------------------------------------
// NODES
// -----------------------------------------------------------------------------

Node_ds18b20Multi ds18B20("ds18B20", DS18B20_BUS1);
Node_AnalogIn AnalogIn("AnalogIn", AnalogIn_PIN);
Node_UltrasonicSerial UltrasonicSerial("UsonicSerial", USonic_PIN1);
Node_DigitalIn DigitalIn("DigitalIn",DigitalIn_PIN );
Node_PulseCounter PulseCounter("PulseCounter", PulseCounter_PIN);



// -----------------------------------------------------------------------------
// Function: Setup Hardware
// -----------------------------------------------------------------------------

void setupLogging() {
      Serial.begin(SERIAL_BAUDRATE);
      Serial.println();
      Serial.println();
      Homie.getLogger() << "[setupHardware] finished\n\n\n" << endl;
}

// -----------------------------------------------------------------------------
// Function: Setup Homie
// -----------------------------------------------------------------------------

void setupHomie() {
  Homie_setFirmware(APP_NAME, APP_VERSION);
  Homie.disableResetTrigger();
  Homie.setup();
  Homie.getLogger() << "[setupHomie] finished" << endl;
}

// -----------------------------------------------------------------------------
// setup
// -----------------------------------------------------------------------------

void setup() {
  setupLogging();
  welcome();
  otaSetup(OTA_PORT, OTA_PASS);
  setupHomie();

}


// -----------------------------------------------------------------------------
// Loop
// -----------------------------------------------------------------------------

void loop() {
  Homie.loop();
  otaLoop();
}
