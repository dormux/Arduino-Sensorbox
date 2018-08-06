#ifndef SENSORHUMIDITY_H
#define SENSORHUMIDITY_H

#include "Sensor.h"
#include <DHT.h>

class SensorHumidity : public SensorDevice
{
private:
    DHT* _dht;
public:
	SensorHumidity(DHT *dht);
	void begin(); 
    void measure();
    char* getValueName();
    char* getValueUnit();
    String toString(bool unit);
    int getEvaluationColor();
};

#endif //SENSORHUMIDITY_H