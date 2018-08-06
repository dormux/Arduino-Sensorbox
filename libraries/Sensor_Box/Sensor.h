#ifndef SENSOR_H
#define SENSOR_H

#include "Directives.h"
#include "Arduino.h"

class SensorDevice {
    protected:
    float _measured_tmp_value;
    float _published_value;
    float _history[60];
    int   _ring_buffer_pointer;
    bool  _error;
    float _ok_value;
    float _bad_value;
    public:
        SensorDevice();
        virtual void begin(){ /*nothing*/ }; 
        virtual void measure(){ /*nothing*/ };
        virtual char* getValueName(){ /*nothing*/ };
        virtual char* getValueUnit(){ /*nothing*/ };
        virtual String toString(bool unit){ /*nothing*/ };
        virtual int getEvaluationColor(){ /*nothing*/ };
        virtual void publish();
        float getOkValue();
        float getBadValue();
        float getMeasurement();
        float getAvarage(int n);
        void getHistory(float pdata[]);
};
#endif /* SENSOR_H */