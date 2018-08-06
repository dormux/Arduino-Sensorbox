#ifndef SENSORLIGHT_H
#define SENSORLIGHT_H

#include "Sensor.h"
#include <Adafruit_TSL2591.h>

class SensorLight : public SensorDevice
{
private:
    Adafruit_TSL2591* _tsl;
    void handleError();
public:
	SensorLight(Adafruit_TSL2591* tsl);
	void begin(); 
    void measure();
    char* getValueName();
    char* getValueUnit();
    String toString(bool unit);
    int getEvaluationColor();
};

#endif //SENSORLIGHT_H