#ifndef GPRSCOMMUNICATOR_H
#define GPRSCOMMUNICATOR_H

#include "Directives.h"
#include "Adafruit_FONA.h"
#include "Arduino.h"
#include <rtc_clock.h>

class GPRSCommunicator {
    protected:
        Adafruit_FONA* _fona;
        String _destinationURL;
        char _imei[16];
        bool _error;
        RTC_clock* _rtcclock;

    public:
        GPRSCommunicator(String destinationUrl);
        bool sendData(float temperature, float humidity, float pressure, float airco, float audiovolume, float light);
        String getDateTime();
        bool getError();
        void setRtcClock(RTC_clock* rtcclock);
};
#endif /* GPRSCOMMUNICATOR_H */