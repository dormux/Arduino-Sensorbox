#ifndef SENSORPRESSURE_H
#define SENSORPRESSURE_H

#include "Sensor.h"
#include <Adafruit_MPL3115A2.h>

class SensorPressure : public SensorDevice
{
private:
    Adafruit_MPL3115A2* _mpl;
public:
	SensorPressure(Adafruit_MPL3115A2* _mpl);
	void begin(); 
    void measure();
    char* getValueName();
    char* getValueUnit();
    String toString(bool unit);
    int getEvaluationColor();
};

#endif //SENSORLIGHT_H