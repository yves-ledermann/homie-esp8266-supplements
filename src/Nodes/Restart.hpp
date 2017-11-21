/*
 * Restart.hpp
 * Homie Node to restart the device
 *
 *
 */

#pragma once

#include <HomieNode.hpp>

class RestartNode : public HomieNode {

protected:
  virtual void setup() override;
  virtual bool handleInput(const String& property, const HomieRange& range, const String& value) override;

public:
  RestartNode(const char *name);
  void restartNode();

};
