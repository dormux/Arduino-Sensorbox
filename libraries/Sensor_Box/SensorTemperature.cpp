#include "SensorTemperature.h"

SensorTemperature::SensorTemperature(DHT *dht): SensorDevice(){
    _dht=dht;
    _ok_value=25.0f;
    _bad_value=30.0f;
}
void SensorTemperature::begin(){
    _dht->begin();
}

void SensorTemperature::measure(){
    if(_error){
        begin();
    }
    float tmpvalue=_dht->readTemperature();
    if(isnan(tmpvalue)){
        tmpvalue=UNSET;
        _error=true;
    }
    _measured_tmp_value =tmpvalue;
}

char* SensorTemperature::getValueName(){
    return "Temperatur";
}
char* SensorTemperature::getValueUnit(){
    return "C";
}

String SensorTemperature::toString(bool unit){
    //float value=getMeasurement()+0.5f;
    //int help = (int)(value);
    String returnValue=String(getMeasurement(),0);
    if(unit) returnValue=returnValue+String("C");
    return returnValue;
}
int SensorTemperature::getEvaluationColor(){
    return BLACK;
}
