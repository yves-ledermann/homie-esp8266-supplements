/*
 * Example Sketch to Test homie-esp8266-supplements library
 * with all Nodes and Functions
 *
*/
// -----------------------------------------------------------------------------
// Configurations
// -----------------------------------------------------------------------------

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

#define SolarWaterSensor_PIN    4 // 

// -----------------------------------------------------------------------------
// INCLUDES
// -----------------------------------------------------------------------------
#include <Arduino.h>
#include <homie-esp8266-supplements.h>

// -----------------------------------------------------------------------------
// NODES
// -----------------------------------------------------------------------------

Node_SerialSolarWaterSensor SolarWaterSensor("SolarWaterSensor", SolarWaterSensor_PIN );

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
  setupLogging(false);
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
