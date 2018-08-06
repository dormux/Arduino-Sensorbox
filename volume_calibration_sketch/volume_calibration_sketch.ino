    /****************************************
    Example Sound Level Sketch for the 
    Adafruit Microphone Amplifier
    ****************************************/

    #define AUDIOPIN A6
    void setup() 
    {
       Serial.begin(9600);
    }
     
     
    void loop() 
    {
      Serial.println(measureSound());
    }

float measureSound(){
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
