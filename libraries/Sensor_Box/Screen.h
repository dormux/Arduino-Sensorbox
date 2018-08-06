#ifndef SCREEN_H
#define SCREEN_H

#include "Directives.h"
#include "Sensor.h"
#include "Images.h"
#include <Adafruit_GFX.h>
#include <Adafruit_TFTLCD.h>
#include <avr/pgmspace.h>
#include <TouchScreen.h>
#include <Arduino.h>


typedef enum
{
  SLEEP_MODE,
  OVERVIEW_MODE,
  GRAPH_MODE,
}
screen_output_mode;

typedef enum
{
    TEMPERATURE_MODE=0,
    PRESSURE_MODE=1,
    HUMIDITY_MODE=2,
    AIR_MODE=3,
    AUDIOVOLUME_MODE=4,
    LIGHT_MODE=5
}
graph_mode_type;


class Screen {
    protected:
        TouchScreen* _ts;
        Adafruit_TFTLCD* _tft;
        SensorDevice **_devices;
        const unsigned char **_overview_images;
        Adafruit_GFX_Button button1;
        Adafruit_GFX_Button button2;
        Adafruit_GFX_Button button3;
        Adafruit_GFX_Button button4;
        Adafruit_GFX_Button button5;
        Adafruit_GFX_Button button6;
        int _sleep_mode_counter;
        screen_output_mode _current_screen_mode;
        graph_mode_type _current_graph_mode;
        void drawSleepMode();
        void drawOverviewMode();
        void drawGraphMode(float values[], int xSize, float medium, float bad, String name);
        float getGraphReferenceScaling(float minVal, float maxVal, int numOfLines);
        unsigned long _lastinteraction;

    public:
        Screen( SensorDevice *temperature, 
                SensorDevice *pressure, 
                SensorDevice *humidity, 
                SensorDevice *air, 
                SensorDevice *audivolume, 
                SensorDevice *light
                );
        void handleInput();
        void draw();
};


#endif //SCREEN_H