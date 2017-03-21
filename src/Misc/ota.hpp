#ifndef SRC_OTA_H
#define SRC_OTA_H


#include <Homie.h> // used for logger
#include <ArduinoOTA.h>

void otaSetup(uint16_t port, const char *password);

void otaLoop();


#endif /* end of include guard: SRC_OTA_H */
