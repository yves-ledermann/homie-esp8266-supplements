/*
 * Example Sketch to Test homie-esp8266-supplements library
 *
 *
*/
// -----------------------------------------------------------------------------
// Configurations
// -----------------------------------------------------------------------------

#define OTA_PASS "assugrin"
#define OTA_PORT 8266

//------------------------------------------------------------------------------
// APP
//------------------------------------------------------------------------------

#define APP_NAME                "homie-WaterNode"
#define APP_VERSION             "2.0.1"
#define APP_AUTHOR              "yves@laedis.ch"
#define APP_WEBSITE             "http://yves.laedis.ch"

// -----------------------------------------------------------------------------
// HARDWARE
// -----------------------------------------------------------------------------

#define OneWire_PIN1    4 //Pin D2
#define USonic_PIN1    2 //Pin D4
#define AnalogIn_PIN1            A0 // Analog 0
#define PulseCounter_PIN1   D8 // 14 //Pin D5
#define PulseCounter_PIN2   D7 // 12 //Pin D6
#define PulseCounter_PIN3   D6 //Pin D7

// -----------------------------------------------------------------------------
// INCLUDES
// -----------------------------------------------------------------------------
#include <Arduino.h>
#include <homie-esp8266-supplements.h>

// -----------------------------------------------------------------------------
// NODES
// -----------------------------------------------------------------------------

//Node_ds18b20Multi ds18B20("ds18B20", OneWire_PIN1);
//Node_AnalogIn AnalogIn("AnalogIn", AnalogIn_PIN1);
//Node_UltrasonicSerial UltrasonicSerial("UsonicSerial", USonic_PIN1);
Node_PulseCounter PulseCounter1("PulseCounter1", PulseCounter_PIN1, "l", "min", G1);
Node_PulseCounter PulseCounter2("PulseCounter2", PulseCounter_PIN2, "l", "min", G1);

// -----------------------------------------------------------------------------
// FlowMeter helper Functions
// (was not able to implement the ISR inside the NodeClass)
// -----------------------------------------------------------------------------
void PulseCounter1ISR() {
  PulseCounter1.count();
}
void PulseCounter2ISR() {
  PulseCounter2.count();
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
  setupHomie();
  setupLogging(false);
  welcome();
  otaSetup(OTA_PORT, OTA_PASS);

  // Interrupt for PulseCounters
  attachInterrupt(PulseCounter_PIN1, PulseCounter1ISR, RISING);
  attachInterrupt(PulseCounter_PIN2, PulseCounter2ISR, RISING);
}


// -----------------------------------------------------------------------------
// Loop
// -----------------------------------------------------------------------------

void loop() {
  Homie.loop();
  otaLoop();
}
