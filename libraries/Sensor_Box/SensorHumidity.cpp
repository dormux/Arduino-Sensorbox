#include "SensorHumidity.h"

SensorHumidity::SensorHumidity(DHT *dht): SensorDevice(){
    _dht=dht;
    _ok_value=0;
    _bad_value=0;
}
void SensorHumidity::begin(){
    _dht->begin();
}

void SensorHumidity::measure(){
    if(_error){
        begin();
    }
    float tmpvalue=_dht->readHumidity();
    if(isnan(tmpvalue)){
        tmpvalue=UNSET;
        _error=true;
    }
    else{
        //add constant offest to measurement
        tmpvalue+=10.0f;
    }
    _measured_tmp_value =tmpvalue;
}

char* SensorHumidity::getValueName(){
    return "Luftfeuchte";
}
char* SensorHumidity::getValueUnit(){
    return "%";
}

String SensorHumidity::toString(bool unit){
    String returnValue =  String(getMeasurement(), 0);
    if(unit) returnValue=returnValue+String("%");
    return returnValue;
}
int SensorHumidity::getEvaluationColor(){
    return BLACK;
}