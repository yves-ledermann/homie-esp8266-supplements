/*
 * Example Sketch to Test homie-esp8266-supplements library
 * with PowerMeter Node
 *
*/

/*  WEMOS D1 Mini
                     ______________________________
                    |   L T L T L T L T L T L T    |
                    |                              |
                 RST|                             1|TX
                  A0|                             3|RX
                  D0|16                           5|D1
                  D5|14                           4|D2
DERE Pin          D6|12                           0|D3
RX SSer/HSer swap D7|13                           2|D4
TX_SSer/HSer swap D8|15                            |GND
                 3V3|__                            |5V
                       |                           |
                       |___________________________|
*/


// -----------------------------------------------------------------------------
// Configurations
// -----------------------------------------------------------------------------

#define OTA_PASS                "assugrin"
#define OTA_PORT                8266

//------------------------------------------------------------------------------
// APP
//------------------------------------------------------------------------------

#define APP_NAME                "homie-PowerMeterModbusExample"
#define APP_VERSION             "2.0.1"
#define APP_AUTHOR              "yves@laedis.ch"
#define APP_WEBSITE             "http://yves.laedis.ch"

// -----------------------------------------------------------------------------
// HARDWARE
// -----------------------------------------------------------------------------

#define PowerMeterTX_PIN    D8 //
#define PowerMeterRX_PIN    D7 //
#define PowerMeterDERE_PIN  D6 //

// -----------------------------------------------------------------------------
// INCLUDES
// -----------------------------------------------------------------------------
#include <Arduino.h>
#include <homie-esp8266-supplements.h>

// -----------------------------------------------------------------------------
// NODES
// -----------------------------------------------------------------------------

Node_ModbusPowerMeter_Eastron PowerMeter_IT("IT", PowerMeterTX_PIN, PowerMeterRX_PIN, PowerMeterDERE_PIN);

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
