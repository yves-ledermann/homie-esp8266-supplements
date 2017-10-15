#pragma once

#include <Arduino.h>
#include <RemoteDebug.h> // Remote debug over telnet - not recommended for production, only for development
#include <Homie.h>

void setupLogging(bool telnet);
