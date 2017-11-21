#include "Restart.hpp"
#include <Homie.hpp>

RestartNode::RestartNode(const char *name)
    : HomieNode(name, "RestartNode") {

}

void RestartNode::setup() {
  advertise("restart").settable();

}

bool RestartNode::handleInput(const String& property, const HomieRange& range, const String& value) {
  Homie.getLogger() << "Message: " << value << endl;
  if (value != "true") return false;

  if (value == "true") ESP.reset();

  return true;
}
