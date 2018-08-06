#include "GPRSCommunicator.h"

GPRSCommunicator::GPRSCommunicator(String destinationUrl){
    _destinationURL=destinationUrl;
    _error=false;
    HardwareSerial *fonaSerial = &Serial1;
    _fona = new Adafruit_FONA(FONA_RST);
    fonaSerial->begin(4800);
    if (! _fona->begin(*fonaSerial)) {
        Serial.println(F("Couldn't find FONA"));
        _error=true;
    }
    uint8_t imeiLen = _fona->getIMEI(_imei);
    if (imeiLen = 0) {
        Serial.println(F("FONA not working properly"));
        _error=true;
    }
    else{
        Serial.println(_imei);
    }
    _fona->setGPRSNetworkSettings(F("bob.at"), F("data@bob.at"), F("ppp"));
    if (!_fona->enableGPRS(false)){
          Serial.println(F("Failed to turn off"));
    }
    if (!_fona->enableGPRS(true)){
          Serial.println(F("Failed to turn off"));
          _error=true;
    }
     if (!_fona->enableNTPTimeSync(true, F("pool.ntp.org"))){
          Serial.println(F("Failed to enable"));
          _error=true;
    }
}
bool GPRSCommunicator::getError(){
    return _error;
}
String GPRSCommunicator::getDateTime(){
    char buffer[23];
    _fona->getTime(buffer, 23);
    return String(buffer);
}

void GPRSCommunicator::setRtcClock(RTC_clock* rtcclock){
    _rtcclock=rtcclock;
}

bool GPRSCommunicator::sendData(float temperature, float humidity, float pressure, float airco, float audiovolume, float light){
    //TODO float decimals!
    String timestampstr=   String("\"timestamp\":\"")+_rtcclock->get_years()+String("-")+_rtcclock->get_months()+String("-")+_rtcclock->get_days()+
                        String("T")+_rtcclock->get_hours()+String(":")+_rtcclock->get_minutes()+String(":")+_rtcclock->get_seconds()+
                        ".000Z\",";
    String imeistr= String("\"IMEI\":\"")+_imei+String("\",");
    String tempstr= String("\"temperature\":")+temperature+String(",");
    String humstr= String("\"humidity\":")+humidity+String(",");
    String presstr= String("\"pressure\":")+pressure+String(",");
    String airstr= String("\"airco2\":")+airco+String(",");
    String audiopstr= String("\"audiovolume\":")+audiovolume+String(",");
    String ligstr= String("\"light\":")+light+String("");

    String datastr=String("{")+timestampstr+imeistr+tempstr+humstr+presstr+airstr+audiopstr+ligstr+String("}");
    int lengthstr=datastr.length()+1;
    char data[lengthstr];
    datastr.toCharArray(data,lengthstr);
    lengthstr=_destinationURL.length()+1;
    char url[lengthstr];
    _destinationURL.toCharArray(url,lengthstr);
    uint16_t statuscode;
    int16_t length;
    Serial.println(url);
    Serial.println(data);
    while (Serial.available())Serial.read();
    if (!_fona->HTTP_POST_start(url, F("application/json"), (uint8_t *) data, strlen(data), &statuscode, (uint16_t *)&length)) {
          Serial.println("Failed!");
        }
    else{
        while (length > 0) {
            while (_fona->available()) {
                char c = _fona->read();
                Serial.write(c);
                length--;
                if (! length) break;
            }
        }
        _fona->HTTP_GET_end();
    }
}