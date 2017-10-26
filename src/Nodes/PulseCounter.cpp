#include "PulseCounter.hpp"

Node_PulseCounter::Node_PulseCounter(const char* name, const uint8_t PinNumber, const char* unit, const char* period, PulseSensorProperties prop) :
		HomieNode(name, "PulseCounter",
			[](String property, HomieRange range, String value) { return false; }),
		_name(name),							// node Name
		_PinNumber(PinNumber),		// HW-Pin
		_unit(unit),							// ex. litres => l
		_period(period),					// ex per Minute => min
		_properties(prop)         // store sensor properties
{
	advertise(String("Node_PulseCounter_" + _name).c_str());

  if (_debug) {
		Homie.getLogger() << "[Node_PulseCounter-" << _name << "] constructor finished" << endl;
	}
}

void Node_PulseCounter::setup() {
	pinMode(_PinNumber, INPUT_PULLUP);

	Homie.getLogger() << "[Node_PulseCounter-" << _name << "] Setup finished \n\n************************\n\n\n" << endl;
}


void Node_PulseCounter::loop() {

	if (millis() < _lastLoopUpdate) _lastLoopUpdate = 0; //Correct the 29day overflow

// fast Update if Pulses Counting
	if (_currentRate > _properties.capacity / 1000UL) {
		if (millis() - _lastLoopUpdate >= _Interval) {
			Node_PulseCounter::update();
			_lastLoopUpdate = millis();
		}
	}

// slow Update if Idle
	if (millis() - _lastLoopUpdate >= (_Interval * 5)) {
		Node_PulseCounter::update();
		_lastLoopUpdate = millis();
	}


} // loop

void Node_PulseCounter::setDebug(bool debug) {
  _debug = debug;
}

void Node_PulseCounter::setInterval(uint16_t interval) {
	_Interval = interval * 1000UL;
}

void Node_PulseCounter::update() {
	// wait between output updates
//	  delay(period);

	  // process the (possibly) counted ticks
	  tick(millis() - _lastLoopUpdate);

      setProperty("Rate").send(String(_currentRate).c_str());
			setProperty("Frequency").send(String(_currentFrequency).c_str());
      setProperty("Total-Quantity").send(String(_totalQuantity).c_str());
			setProperty("Total-Duration").send(String(_totalDuration).c_str());

			setProperty("FreeHeap").send(String(ESP.getFreeHeap()).c_str());

	if (_debug) {
		Homie.getLogger() << "[Node_PulseCounter-" << _name << "] update Pulses: "
				<< _currentPulses << " Currently " << _currentRate
				<< " Rate, " << _totalQuantity << " Quant. total"<< "\n" << endl;
	}
}

double Node_PulseCounter::getCurrentQuantity() {
    return _currentQuantity;                                            //!< in l
}

double Node_PulseCounter::getTotalRate() {
    return this->_totalQuantity / (this->_totalDuration / 1000.0f) * 60.0f;   //!< in l/min
}

double Node_PulseCounter::getTotalQuantity() {
    return this->_totalQuantity;                                              //!< in l
}

void Node_PulseCounter::tick(unsigned long duration) {
    /* sampling and normalisation */
    double seconds = duration / 1000.0f;                                    //!< normalised duration (in s, i.e. per 1000ms)
  //  unsigned int interrupts = SREG;                                         //!< save the interrupt status
    cli();                                                                  //!< going to change interrupt variable(s)
    double frequency = this->_currentPulses / seconds;                      //!< normalised frequency (in 1/s)
    this->_currentPulses = 0;                                               //!< reset pulse counter after successfull sampling
    //SREG = interrupts;                                                      //!< done changing interrupt variable(s)
		sei();

    /* determine current correction factor (from sensor properties) */
    unsigned int decile = floor(10.0f * frequency / (this->_properties.capacity * this->_properties.kFactor));  //!< decile of current flow relative to sensor capacity
    this->_currentCorrection = this->_properties.kFactor / this->_properties.mFactor[_min(decile, 9)];           //!< combine constant k-factor and m-factor for decile

    /* update current calculations: */
    this->_currentRate = frequency / this->_currentCorrection;          //!< get flow rate (in l/min) from normalised frequency and combined correction factor
    this->_currentQuantity = this->_currentRate / (60.0f/seconds);        //!< get volume (in l) from normalised flow rate and normalised time

    /* update statistics: */
    this->_currentDuration = duration;                                      //!< store current tick duration (convenience, in ms)
    this->_currentFrequency = frequency;                                    //!< store current pulses per second (convenience, in 1/s)
    this->_totalDuration += duration;                                       //!< accumulate total duration (in ms)
    this->_totalQuantity += this->_currentQuantity;                             //!< accumulate total volume (in l)
    this->_totalCorrection += this->_currentCorrection * duration;          //!< accumulate corrections over time
}

void Node_PulseCounter::count() {
    this->_currentPulses++;                                                 //!< this should be called from an interrupt service routine
}

void Node_PulseCounter::reset() {
    //unsigned int interrupts = SREG;                                         //!< save interrupt status
    cli();                                                                  //!< going to change interrupt variable(s)
    this->_currentPulses = 0;                                               //!< reset pulse counter
  //  SREG = interrupts;                                                      //!< done changing interrupt variable(s)
	sei();

    this->_currentFrequency = 0.0f;
    this->_currentDuration = 0.0f;
    this->_currentRate = 0.0f;
    this->_currentQuantity = 0.0f;
    this->_currentCorrection = 0.0f;
}

unsigned int Node_PulseCounter::getPin() {
    return this->_PinNumber;
}

unsigned long Node_PulseCounter::getCurrentDuration() {
    return this->_currentDuration;                                          //!< in ms
}

double Node_PulseCounter::getCurrentFrequency() {
    return this->_currentFrequency;                                         //!< in 1/s
}

double Node_PulseCounter::getCurrentError() {
    /// error (in %) = error * 100
    /// error = correction rate - 1
    /// correction rate = k-factor / correction
    return (this->_properties.kFactor / this->_currentCorrection - 1) * 100;  //!< in %
}

unsigned long Node_PulseCounter::getTotalDuration() {
    return this->_totalDuration;                                            //!< in ms
}

double Node_PulseCounter::getTotalError() {
    /// average error (in %) = average error * 100
    /// average error = average correction rate - 1
    /// average correction rate = k-factor / corrections over time * total time
    return (this->_properties.kFactor / this->_totalCorrection * this->_totalDuration - 1) * 100;
}

PulseSensorProperties UncalibratedSensor = {60.0f, 5.0f, {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};
PulseSensorProperties G1 = {60.0f, 4.58f, {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};
PulseSensorProperties FS300A = {60.0f, 5.5f, {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};
PulseSensorProperties FS400A = {60.0f, 4.8f, {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};
