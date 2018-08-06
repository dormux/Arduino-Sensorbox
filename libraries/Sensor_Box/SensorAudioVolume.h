#ifndef SENSORAUDIOVOLUME_H
#define SENSORAUDIOVOLUME_H

#include "Sensor.h"
#include <math.h>
#include <Arduino.h>

class SensorAudioVolume : public SensorDevice
{
private:
    float measureSound();
    int _amount_measurements;
    int peakToDB(float peak);
public:
	SensorAudioVolume();
	void begin(); 
    void measure();
    void publish() override;
    char* getValueName();
    char* getValueUnit();
    String toString(bool unit);
    int getEvaluationColor();
};

#endif //SENSORAUDIOVOLUME_H