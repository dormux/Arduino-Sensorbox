#include "SensorPressure.h"

SensorPressure::SensorPressure(Adafruit_MPL3115A2* mpl): SensorDevice(){
    _mpl=mpl;
    _ok_value=0;
    _bad_value=0;
}
void SensorPressure::begin(){
    if (!_mpl->begin()) {
    _error=true;
    }
    else _error=false;
}

void SensorPressure::measure(){
    if(_error){
        begin();
    }
    if (_mpl->begin()) {
      float pascals = _mpl->getPressure();
      _measured_tmp_value = pascals / 100.0f;
      _error=false;
    }
    else {
      _error = true;
      _measured_tmp_value = UNSET;
    }
}

char* SensorPressure::getValueName(){
    return "Luftdruck";
}
char* SensorPressure::getValueUnit(){
    return "hPa";
}
String SensorPressure::toString(bool unit){
    String returnValue =  String(_published_value, 1);
    if(unit) returnValue=returnValue+String(getValueUnit());
    return returnValue;
}
int SensorPressure::getEvaluationColor(){
    return BLACK;
}