#ifndef SENSORTEMPERATURE_H
#define SENSORTEMPERATURE_H

#include "Sensor.h"
#include <DHT.h>

class SensorTemperature : public SensorDevice
{
private:
    DHT* _dht;
public:
	SensorTemperature(DHT *dht);
	void begin(); 
    void measure();
    char* getValueName();
    char* getValueUnit();
    String toString(bool unit);
    int getEvaluationColor();
};

#endif //SENSORTEMPERATURE_H