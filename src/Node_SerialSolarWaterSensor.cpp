#include "Node_SerialSolarWaterSensor.hpp"


#define TIMEOUT 50000
#define MARK_HEADER 7000
#define SPACE_HEADER 3000
#define MARK 500
#define SPACE_ONE 1500
#define SPACE_ZERO 600

Node_SolarWaterSens::Node_SolarWaterSens (const char* name, const uint8_t PinNumber) :
		HomieNode("Sensor", "SerialSolarWater",
			[](String property, HomieRange range, String value) { return false; }),
		_name(name),
		_PinNumber(PinNumber)
{
  advertise(String("Node_SolarWaterSens" + _name).c_str());

  if (_debug) {
		Homie.getLogger() << "[Node_SolarWaterSens-" << _name << "] constructor finished" << endl;
	}
}

void Node_SolarWaterSens::setup() {
	pinMode(_PinNumber, INPUT);
	Homie.getLogger() << "[Node_SolarWaterSens" << _name << "] Setup finished \n\n************************\n\n\n" << endl;
}

void Node_SolarWaterSens::loop() {

	if (millis() < lastLoopUpdate) lastLoopUpdate = 0; //Correct the 29day overflow

	if (millis() - lastLoopUpdate >= _Interval) {
		Node_SolarWaterSens::update();
		lastLoopUpdate = millis();

		char temp,level;
		if(Node_SolarWaterSens::readTempNLevelSensor(_PinNumber, temp, level)){

			Serial.print(temp,DEC);Serial.print("c ");Serial.println(level,DEC);
		}

	}


/*
	if (millis() - lastLoopSensor >= _Interval) {
		Homie.getLogger() << "[Ultrasonic_Serial_Node-" << _name << "]" << _distance << endl;
		setProperty("Distance").send(String(_distance).c_str());
		lastLoopSensor = millis();
	}
*/

} // loop

void Node_SolarWaterSens::setDebug(bool debug) {
  _debug = debug;
}

void Node_SolarWaterSens::setInterval(uint16_t interval) {
	_Interval = interval * 1000UL;
}

void Node_SolarWaterSens::update(){
}


int Node_SolarWaterSens::expectPulse(int val){
  unsigned long t=micros();
  while(digitalRead(_PinNumber)==val){
    if( (micros()-t)>TIMEOUT ) return 0;
  }
  return micros()-t;
}

// temp in celsious and level goes from 0 to 3
bool Node_SolarWaterSens::readTempNLevelSensor(char _PinNumber, char &temp, char &level){
   byte data[5]={0,0,0,0,0};
   unsigned long val1;
   unsigned long st=micros();
   val1 = expectPulse(HIGH);
   if(val1>MARK_HEADER){
     val1 = expectPulse(LOW);
     if(val1>SPACE_HEADER){
       int c=39;
       for(;c>-1;c--){
         val1 = expectPulse(HIGH);
         if(val1<MARK){
          val1 = expectPulse(LOW);
          if(val1==0){
            //Serial.println("Mark error 0");
            break;
          }
          if(val1<SPACE_ZERO){
            //0
            //data[c>>3]|=(1<<(c & B111));
          }else if(val1<SPACE_ONE){
            //1
            data[c>>3]|=(1<<(c & B111));
          }else{
           //Serial.print(val1);Serial.println(" Space error");
           break;
          }
         }else{
          //Serial.println("Mark error");
          break;
         }
       }
       // Each reading should not take more than 70ms (use time to detect errors)
       if(micros()-st<70000){
         temp=data[3];
         level=data[2];
         //Serial.print(data[3]);Serial.print(" ");Serial.println((data[2]));
         //Serial.print(data[4],HEX);Serial.print(" ");Serial.print(data[3],HEX);Serial.print(" ");Serial.print(data[2],HEX);Serial.print(" ");Serial.print(data[1],HEX);Serial.print(" ");Serial.println(data[0],HEX);
         return true;
       }
    }
   }
   return false;
}

// From comments

void Node_SolarWaterSens::handleSensorInterrupt() {
/* This routine has been optimized to just capture the part of the packet
that has the temp & level
On averages it uses 550 us per second
*/
static unsigned int duration;
static unsigned long lastTime;
static char c=15; //Current bit
static bool expectMark;
int wh_data[5];
int tempWH, levelWH;
unsigned long time = micros();
duration = time - lastTime;
if(5000>duration && duration>3000){
//Match SPACE_HEADER typically 4000us (new packet)
c=39;
// memset(data, 0, sizeof(data));
wh_data[2]=0;
wh_data[3]=0;
expectMark=true;
// }else if(c>0){ // Get the whole packet
}else if(c>=16){
if(expectMark){
// Skip MARK
expectMark=false;
}else{
// Decode SPACE
if(1700>duration && duration>1300){
// #define SPACE_ONE 1500
wh_data[c>>3]|=(1<<(c & B111));
c--;
}else if(800>duration && duration>400){
// #define SPACE_ZERO 500
c--;
}
expectMark=true;
if(c==15){
// Got all the bits
tempWH=wh_data[3];
levelWH=(wh_data[2] & 0x0F);
}
}
}
lastTime = time;
// count+=micros()-time;
}
