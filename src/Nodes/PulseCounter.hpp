#pragma once

#include <Homie.hpp>

/**
 * PulseSensorProperties
 *
 * Structure that holds essential information about a sensor.
 * Stores general sensor properties and calibration points.
 *
*/

typedef struct {
  double capacity;      //!< capacity, upper limit of flow rate (in l/min)
  double kFactor;       //!< "k-factor" (in (pulses/s) / (l/min)), e.g.: 1 pulse/s = kFactor * l/min
  double mFactor[10];   //!< multiplicative correction factor near unity, "meter factor" (per decile of flow)
} PulseSensorProperties;

extern PulseSensorProperties UncalibratedSensor; //!< default sensor
extern PulseSensorProperties G1;             //!< see documentation about 
extern PulseSensorProperties FS300A;             //!< see documentation about FS300A/SEN02141B
extern PulseSensorProperties FS400A;             //!< see documentation about FS400A/USN-HS10TA



class Node_PulseCounter: public HomieNode {

  public:
  	Node_PulseCounter(const char* name, const uint8_t PinNumber, const char* unit = "l", const char* period = "min", PulseSensorProperties prop = UncalibratedSensor);

    void setup();
    void handleInterrupt();
  	void loop();

    void setDebug(bool debug);
    void setInterval(uint16_t interval);

    double getCurrentRate();      //!< Returns the current flow rate since last reset (in l/min).
    double getCurrentQuantity();  //!< Returns the current volume since last reset (in l).

    double getTotalRate();        //!< Returns the (linear) average flow rate in this flow meter instance (in l/min).
    double getTotalQuantity();    //!< Returns the total volume flown trough this flow meter instance (in l).

    /**
     * The tick method updates all internal calculations at the end of a measurement period.
     *
     * We're calculating flow and volume data over time.
     * The actual pulses have to be sampled using the count method (i.e. via an interrupt service routine).
     *
     * Flow sensor formulae:
     *
     * Let K: pulses per second per unit of measure (i.e. (1/s)/(l/min)),
     *     f: pulse frequency (1/s),
     *     Q: flow rate (l/min),
     *     p: sensor pulses (no dimension/unit),
     *     t: time since last measurements (s).
     *
     * K = f / Q             | units: (1/s) / (l/min) = (1/s) / (l/min)
     * <=>                   | Substitute p / t for f in order to allow for different measurement intervals
     * K = (p / t) / Q       | units: ((1/s)/(l/min)) = (1/s) / (l/min)
     * <=>                   | Solve for Q:
     * Q = (p / t) / K       | untis: l/min = 1/s / (1/s / (l/min))
     * <=>                   | Volume in l:
     * V = Q / 60            | units: l = (l/min) / (min)
     *
     * The property K is sometimes stated in pulses per liter or pulses per gallon.
     * In these cases the unit of measure has to be converted accordingly (e.g. from gal/s to l/min).
     * See file G34_Flow_rate_to_frequency.jpg for reference.
     *
     * @param duration The tick duration (in ms).
     */
    void tick(unsigned long duration = 1000);
    void count();                                 //!< Increments the internal pulse counter. Serves as an interrupt callback routine.
    void reset();                                 //!< Prepares the flow meter for a fresh measurement. Resets all current values.

    /*
     * convenience methods and calibration helpers
     */
    unsigned int getPin();                        //!< Returns the Arduino pin number that the flow sensor is connected to.

    unsigned long getCurrentDuration();           //!< Returns the duration of the current tick (in ms).
    double getCurrentFrequency();                 //!< Returns the pulse rate in the current tick (in 1/s).
    double getCurrentError();                     //!< Returns the error resulting from the current measurement (in %).

    unsigned long getTotalDuration();             //!< Returns the total run time of this flow meter instance (in ms).
    double getTotalError();                       //!< Returns the (linear) average error of this flow meter instance (in %).


  private:
    String _name;
  	uint8_t _PinNumber;
    String _unit;
    String _period;
    PulseSensorProperties _properties;             //!< sensor properties (including calibration data)
    bool _debug = true;

    void update();
    uint16_t _Interval = 1000UL;
    uint32_t _lastLoopUpdate = 0;

    unsigned long _currentDuration;               //!< current tick duration (convenience, in ms)
    double _currentFrequency;                     //!< current pulses per second (convenience, in 1/s)
    double _currentRate = 0.0f;               //!< current flow rate (in l/tick), e.g.: 1 l / min = 1 pulse / s / (pulses / s / l / min)
    double _currentQuantity = 0.0f;                 //!< current volume (in l), e.g.: 1 l = 1 (l / min) / (60 * s)
    double _currentCorrection;                    //!< currently applied correction factor

    unsigned long _totalDuration = 0.0f;          //!< total measured duration since begin of measurement (in ms)
    double _totalQuantity = 0.0f;                   //!< total volume since begin of measurement (in l)
    double _totalCorrection = 0.0f;               //!< accumulated correction factors

    volatile unsigned long _currentPulses = 0;    //!< pulses within current sample period

};
