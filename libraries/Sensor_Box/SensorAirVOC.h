#ifndef SENSORAIRVOC_H
#define SENSORAIRVOC_H

#include "Sensor.h"
#include <Arduino.h>

class SensorAirVOC : public SensorDevice
{ 
public:
	SensorAirVOC();
	void begin(); 
    void measure();
    char* getValueName();
    char* getValueUnit();
    String toString(bool unit);
    int getEvaluationColor();
};

#endif //SENSORAIRVOC_H