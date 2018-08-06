#include "Sensor.h"

SensorDevice::SensorDevice(){
    _ring_buffer_pointer=0;
    _published_value=UNSET;
    _measured_tmp_value=UNSET;
    for(int i=0;i<60;i++){
        _history[i]=UNSET;
    }
    _error=true;
}

void SensorDevice::publish(){
    _published_value=_measured_tmp_value;
    _ring_buffer_pointer=(_ring_buffer_pointer+1)%60;
    _history[_ring_buffer_pointer]=_measured_tmp_value;
}
float SensorDevice::getMeasurement(){
    return _published_value;
}

float SensorDevice::getAvarage(int n){
    float tmpsum=0;
    int validValues=0;
    if(n>60)n=60;
    for(int i=0;i<n;i++){
        if(_history[(_ring_buffer_pointer-i + 60) % 60]>0){
            tmpsum+=_history[(_ring_buffer_pointer-i + 60) % 60];
            validValues++;
        }
    }
    if(tmpsum==0){
        return UNSET;
    }
    return tmpsum/float(validValues);
}
void SensorDevice::getHistory(float pdata[]){
    for (int i=0;i<60;i++){
        pdata[i]=_history[(_ring_buffer_pointer+i+1)%60];
    }
}

float SensorDevice::getOkValue(){
    return _ok_value;
}
float SensorDevice::getBadValue(){
    return _bad_value;
}
