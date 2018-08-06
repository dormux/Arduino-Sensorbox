#include "SensorAudioVolume.h"

//Prototype
int sort_desc(const void *cmp1, const void *cmp2);

SensorAudioVolume::SensorAudioVolume():SensorDevice(){
    _amount_measurements=0;
    _measured_tmp_value=0;
    _ok_value=50.0f;
    _bad_value=64.0f;
}
void SensorAudioVolume::begin(){
    _amount_measurements=0;
    _measured_tmp_value=0;
}
void SensorAudioVolume::measure(){
    _amount_measurements++;
    _measured_tmp_value+=measureSound();
}
void SensorAudioVolume::publish(){
    _measured_tmp_value=_measured_tmp_value/(float)_amount_measurements;
    Serial.print("Raw Audio Measurement: ");
    Serial.println(_measured_tmp_value);
    _measured_tmp_value=peakToDB(_measured_tmp_value);
    _published_value=_measured_tmp_value;
    _ring_buffer_pointer=(_ring_buffer_pointer+1)%60;
    _history[_ring_buffer_pointer]=_measured_tmp_value;
    begin();
}
char* SensorAudioVolume::getValueName(){
    return "Schall";
}
char* SensorAudioVolume::getValueUnit(){
    return "dB";
}

String SensorAudioVolume::toString(bool unit){
    String returnValue =  String(getMeasurement(), 0);
    if(unit) returnValue=returnValue+String("dB");
    return returnValue;
}

float SensorAudioVolume::measureSound(){
    int sampleWindow = 100; //Window in MS to measure Sound
    unsigned int peakToPeak = 0;   // peak-to-peak level
    unsigned int signalMax = 0;
    unsigned int signalMin = 1024;
    int measureMax[20];
    int measureMin[20];
    for (int i = 0; i < 20 ; i++) {
        measureMax[i] = 0;
        measureMin[i] = 1024;
    }
    unsigned long startMillis = millis(); // Start of sample window
    while (millis() - startMillis < sampleWindow)
    {
        int measure = analogRead(AUDIOPIN);
        if (measure > measureMax[0]) {
            measureMax[0] = measure;
        }
        if (measure < measureMin[19]) {
            measureMin[19] = measure;
        }
        qsort(measureMax, sizeof(measureMax) / sizeof(measureMax[0]), sizeof(measureMax[0]), sort_desc);
        qsort(measureMin, sizeof(measureMax) / sizeof(measureMax[0]), sizeof(measureMin[0]), sort_desc);
    }
    int sumMax = 0;
    int sumMin = 0;
    for (int j = 5; j < 15; j++)
    {
        sumMax += measureMax[j];
        sumMin += measureMin[19 - j];
    }
    float resultMin = (float) sumMin / 10.0f;
    float resultMax = (float) sumMax / 10.0f;
    peakToPeak = resultMax - resultMin;  // max - min = peak-peak amplitude
    return peakToPeak;
}
int SensorAudioVolume::peakToDB(float peak) {
  return 9.35281f*log(28.2395*(peak-26.0f));
}


int sort_desc(const void *cmp1, const void *cmp2)
{
  // Need to cast the void * to int *
  float a = *((float *)cmp1);
  float b = *((float *)cmp2);
  // The comparison
  return a > b ? 1 : (a < b ? -1 : 0);
  // A simpler, probably faster way:
  //return b - a;
}
int SensorAudioVolume::getEvaluationColor(){
    if ( _published_value < 50) {
        return BLACK;
    }
    else if (_published_value < 64) {
        return YELLOW;
    }
    else {
        return RED;
    }
}