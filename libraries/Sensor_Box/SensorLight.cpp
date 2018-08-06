#include "SensorLight.h"

SensorLight::SensorLight(Adafruit_TSL2591* tsl): SensorDevice(){
    _tsl=tsl;
    _ok_value=0;
    _bad_value=0;
}
void SensorLight::begin(){
    if (!_tsl->begin())
    {
    _error = true;
    }
    else
    {
        _tsl->setGain(TSL2591_GAIN_LOW);
        _tsl->setTiming(TSL2591_INTEGRATIONTIME_300MS);
    }
}

void SensorLight::handleError(){
    _tsl->setGain(TSL2591_GAIN_LOW);
    uint32_t lum = _tsl->getFullLuminosity();
    if (lum){
        _error = false;
        return;
    }
    _tsl->setGain(TSL2591_GAIN_MED);
    lum = _tsl->getFullLuminosity();
    if (lum){
        _error = false;
        return;
    }
    _tsl->setGain(TSL2591_GAIN_HIGH);
    lum = _tsl->getFullLuminosity();
    if (lum){
        _error = false;
        return;
    }
    _tsl->setGain(TSL2591_GAIN_MAX);
    lum = _tsl->getFullLuminosity();
    if (lum){
        _error = false;
        return;
    }
    _error = true;
}

void SensorLight::measure(){
    if(_error){
        begin();
        handleError();
    }
    if(_tsl->getGain()==TSL2591_GAIN_LOW) Serial.println("LOW");
    else if(_tsl->getGain()==TSL2591_GAIN_MED) Serial.println("MED");
    else if(_tsl->getGain()==TSL2591_GAIN_HIGH) Serial.println("HIGH");
    else if(_tsl->getGain()==TSL2591_GAIN_MAX) Serial.println("MAX");
    uint32_t lum = _tsl->getFullLuminosity();
    if (lum) {
      uint16_t ir, full;
      ir = lum >> 16;
      full = lum & 0xFFFF;
      _measured_tmp_value = _tsl->calculateLux(full, ir);
      if (_measured_tmp_value==0){
          _error=true;
          _measured_tmp_value=UNSET;
      }
      if (_measured_tmp_value<0){
          _error=true;
          _measured_tmp_value=UNSET;
      }
    }
    else {
      _error = true;
      _measured_tmp_value = UNSET;
    }
}

char* SensorLight::getValueName(){
    return "Licht";
}
char* SensorLight::getValueUnit(){
    return "Lux";
}

String SensorLight::toString(bool unit){
    if(_published_value<1.0f){
        String returnValue=String(_published_value,3);
        if(unit) returnValue=returnValue+String("Lux");
        return returnValue; 
    }
    if(_published_value<10.0f){
        String returnValue=String(_published_value,2);
        if(unit) returnValue=returnValue+String("Lux");
        return returnValue; 
    }
    if(_published_value<100.0f){
        String returnValue=String(_published_value,1);
        if(unit) returnValue=returnValue+String("Lux");
        return returnValue; 
    }
    String returnValue=String(_published_value,0);
    if(unit) returnValue=returnValue+String("Lux");
        return returnValue;
}
int SensorLight::getEvaluationColor(){
    return BLACK;
}