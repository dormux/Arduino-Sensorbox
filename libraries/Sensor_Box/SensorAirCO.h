#ifndef SENSORAIRCO_H
#define SENSORAIRCO_H

#include "Sensor.h"
#include <Adafruit_CCS811.h>

class SensorAirCO : public SensorDevice
{
private:
    Adafruit_CCS811* _ccs;
public:
	SensorAirCO(Adafruit_CCS811* ccs, float humidity, float temperature);
	void begin(); 
    void measure();
    char* getValueName();
    char* getValueUnit();
    String toString(bool unit);
    int getEvaluationColor();
};

#endif //SENSORAIRCO_H