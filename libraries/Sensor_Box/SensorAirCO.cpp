#include "SensorAirCO.h"

SensorAirCO::SensorAirCO(Adafruit_CCS811* ccs,float humidity, float temperature): SensorDevice(){
    _ccs=ccs;
    if (!_ccs->begin()) {
    _error=true;
    }
     else {
        //calibrate sensor
        while (!_ccs->available());
        float temp = _ccs->calculateTemperature();
        _ccs->setTempOffset(temp - temperature);
        _ccs->setEnvironmentalData((int)humidity, temperature);
        _error=false;
    }
    _ok_value=1000.0f;
    _bad_value=2500.0f;
}
void SensorAirCO::begin(){
    if (!_ccs->begin()) {
    _error=true;
    }
    else{
        _error=false;
    }
}

void SensorAirCO::measure(){
    if(_error){
        begin();
    }
    if (!_ccs->readData()) {
      _measured_tmp_value = _ccs->geteCO2();
      _error=false;
    }
    else {
      _measured_tmp_value = UNSET;
      _error = true;
    }
}

char* SensorAirCO::getValueName(){
    return "CO2";
}
char* SensorAirCO::getValueUnit(){
    return "ppm";
}

String SensorAirCO::toString(bool unit){
    String returnValue =  String(getMeasurement(), 0);
    if(unit) returnValue=returnValue+String("ppm");
    return returnValue;
}
int SensorAirCO::getEvaluationColor(){
    if (_published_value < 1000.0f) {
        return BLACK;
    }
    else if (_published_value < 2500.0f) {
        return YELLOW;
    }
    else {
        return RED;
    }
}
