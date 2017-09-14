#include "Misc/logging.hpp"

#define SERIAL_BAUDRATE         115200

// -----------------------------------------------------------------------------
// Function: Setup Logging
// -----------------------------------------------------------------------------

void setupLogging(bool telnet) {
  if (telnet) {
     RemoteDebug Debug;
     Debug.begin("Telnet Debugging active");
     Debug.setResetCmdEnabled(true);
     Homie.setLoggingPrinter(&Debug);
  }
  else {
    Serial.begin(SERIAL_BAUDRATE);
    Serial.println();
    Serial.println();

  }
      Homie.getLogger() << "[setupLogging] finished\n\n\n" << endl;
}
