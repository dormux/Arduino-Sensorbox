#include "SensorAirVOC.h"

SensorAirVOC::SensorAirVOC(): SensorDevice(){
    _error=false;
    _ok_value=0;
    _bad_value=0;
}
void SensorAirVOC::begin(){
    /*Nothing to do Here*/
}

void SensorAirVOC::measure(){
    _measured_tmp_value = (float)analogRead(VOCPIN);
}

char* SensorAirVOC::getValueName(){
    return "VOC";
}
char* SensorAirVOC::getValueUnit(){
    return "";
}

String SensorAirVOC::toString(bool unit){
    String stringOne =  String(getMeasurement(), 0);
    return stringOne;
}
int SensorAirVOC::getEvaluationColor(){
    return BLACK;
}