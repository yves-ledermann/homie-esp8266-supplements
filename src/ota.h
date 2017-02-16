#ifndef SRC_OTA_H
#define SRC_OTA_H


#include "../../src/config.h"
#include <Homie.h> // used for logger
#include <ArduinoOTA.h>

void otaSetup();

void otaLoop();


#endif /* end of include guard: SRC_OTA_H */
