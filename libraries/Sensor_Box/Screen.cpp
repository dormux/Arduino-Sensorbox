#include "Screen.h"

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max);

Screen::Screen( SensorDevice *temperature, 
                SensorDevice *pressure, 
                SensorDevice *humidity, 
                SensorDevice *air, 
                SensorDevice *audivolume, 
                SensorDevice *light
                ){
    _devices = new SensorDevice*[6];
    _devices[0]=temperature;
    _devices[1]=pressure;
    _devices[2]=humidity;
    _devices[3]=air;
    _devices[4]=audivolume;
    _devices[5]=light;

    _overview_images=new const unsigned char*[6];
    _overview_images[0]=_temperature_overview;
    _overview_images[1]=_pressure_overview;
    _overview_images[2]=_humidity_overview;
    _overview_images[3]=_air_overview;
    _overview_images[4]=_volume_overview;
    _overview_images[5]=_light_overview;

    _ts = new TouchScreen(XP, YP, XM, YM, 300);//Must be instanciated before tft!!!
    _tft = new Adafruit_TFTLCD(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
    //init TFT
    _tft->reset();
    uint16_t identifier = _tft->readID();
    if (identifier == 0x9325 || identifier == 0x9328 || identifier == 0x7575 || identifier == 0x9341 || identifier == 0x8357) {
        Serial.println(F("Found ILI9325 LCD driver"));
        _tft->begin(identifier);
        _tft->setRotation(3);
        _tft->fillScreen(BLACK);
    }
    else {
        Serial.println("TFT Not found");
    }
    _sleep_mode_counter=0;
    _lastinteraction=0;
    _current_screen_mode=SLEEP_MODE;
    _current_graph_mode=TEMPERATURE_MODE;
}

void Screen::handleInput(){
    TSPoint p = _ts->getPoint();
    //digitalWrite(37, LOW);
    pinMode(XM, OUTPUT);
    pinMode(YP, OUTPUT);
    if (p.z > _ts->pressureThreshhold){
        if(_current_screen_mode==SLEEP_MODE){
            _current_screen_mode=OVERVIEW_MODE;
            _lastinteraction=millis();
            draw();
        }
        else if(_current_screen_mode==GRAPH_MODE){
            _current_screen_mode=OVERVIEW_MODE;
            _lastinteraction=millis();
            draw();
        }
        else{
            _current_screen_mode=GRAPH_MODE;
            _lastinteraction=millis();
            int y_temp = p.y;
            p.y = map(p.x, TS_MINX, TS_MAXX, _tft->height(), 0);
            p.x = map(y_temp, TS_MINY, TS_MAXY, 0, _tft->width());
            if (p.x<=160){
                if(p.y<=80){
                    _current_graph_mode=TEMPERATURE_MODE;
                }
                else if(p.y<=160){
                    _current_graph_mode=PRESSURE_MODE;
                }
                else{
                    _current_graph_mode=HUMIDITY_MODE;
                }
            }
            else{
                if(p.y<=80){
                    _current_graph_mode=AIR_MODE;
                }
                else if(p.y<=160){
                    _current_graph_mode=AUDIOVOLUME_MODE;
                }
                else{
                    _current_graph_mode=LIGHT_MODE;
                }
            }
            draw();
        }
    }
    if(millis()-_lastinteraction>40000){
        _current_screen_mode=SLEEP_MODE;
    }
}
void Screen::draw(){
    /*Serial.print("Humididy: ");
    Serial.println(_devices[HUMIDITY_MODE]->getMeasurement());
    Serial.print("Temperature: ");
    Serial.println(_devices[TEMPERATURE_MODE]->getMeasurement());
    Serial.print("Luminosity: ");
    Serial.println(_devices[LIGHT_MODE]->getMeasurement());
    Serial.print("Volume: ");
    Serial.println(_devices[AUDIOVOLUME_MODE]->getMeasurement());
    Serial.print("Pressure: ");
    Serial.println(_devices[PRESSURE_MODE]->getMeasurement());
    Serial.print("CO2: ");
    Serial.println(_devices[AIR_MODE]->getMeasurement());*/
    Serial.println(_current_screen_mode);

    if(_current_screen_mode==SLEEP_MODE){
        drawSleepMode();
    }
    else if(_current_screen_mode==OVERVIEW_MODE){
        drawOverviewMode();
    }
    else{
        float toDraw[60];
        _devices[_current_graph_mode]->getHistory(toDraw);
        drawGraphMode(toDraw, 60, _devices[_current_graph_mode]->getOkValue(), _devices[_current_graph_mode]->getBadValue(),String(_devices[_current_graph_mode]->getValueName()));
    }
}

void Screen::drawSleepMode(){
    int background=_devices[_sleep_mode_counter]->getEvaluationColor();
    int content;
    if(background!=BLACK){
        content = BLACK;
    }else{
        content = WHITE;
    }
    _tft->fillScreen(background);
    _tft->setTextSize(3);
    _tft->setTextColor(content);
    _tft->setCursor(160-((strlen(_devices[_sleep_mode_counter]->getValueName())*16)/2), 10);
    _tft->print(_devices[_sleep_mode_counter]->getValueName());
    _tft->setTextSize(5);
    String outputValue=_devices[_sleep_mode_counter]->toString(false);
    int ycursor=220-(outputValue.length()*15);
    _tft->setCursor(ycursor, 90);
    _tft->print(outputValue);
    if(_sleep_mode_counter==0){
        _tft->setCursor(ycursor, 130);
        _tft->setTextSize(2);
        _tft->print("o");
        ycursor+=15;
    }
    _tft->setCursor(ycursor, 135);
    _tft->setTextSize(3);
    _tft->print(_devices[_sleep_mode_counter]->getValueUnit());
    _tft->drawBitmap(0, 70, _overview_images[_sleep_mode_counter], 128, 128, content);
    _sleep_mode_counter=(_sleep_mode_counter+1)%6;

}

void Screen::drawOverviewMode(){
    String tempstr=_devices[0]->toString(true);
    String pressstr=_devices[1]->toString(true);
    String humstr=_devices[2]->toString(true);
    String airstr=_devices[3]->toString(true);
    String volumestr=_devices[4]->toString(true);
    String lightstr=_devices[5]->toString(true);

    char tempchar[tempstr.length()+2];
    char presschar[pressstr.length()+2];
    char humchar[humstr.length()+2];
    char airchar[airstr.length()+2];
    char volumechar[volumestr.length()+2];
    char lightchar[lightstr.length()+2];

    tempstr.toCharArray(tempchar,tempstr.length()+1);
    pressstr.toCharArray(presschar,pressstr.length()+1);
    humstr.toCharArray(humchar,humstr.length()+1);
    airstr.toCharArray(airchar,airstr.length()+1);
    volumestr.toCharArray(volumechar,volumestr.length()+1);
    lightstr.toCharArray(lightchar,lightstr.length()+1);

    _tft->fillScreen(BLACK);
    int bcolor =_devices[0]->getEvaluationColor();
    if (bcolor==BLACK) bcolor=GREEN;
    button1.initButtonUL(_tft,10,11,145,66,WHITE,bcolor,WHITE,"",1);
    bcolor =_devices[1]->getEvaluationColor();
    if (bcolor==BLACK) bcolor=GREEN;
    button2.initButtonUL(_tft,10,87,145,66,WHITE,bcolor,WHITE,"",1);
    bcolor =_devices[2]->getEvaluationColor();
    if (bcolor==BLACK) bcolor=GREEN;
    button3.initButtonUL(_tft,10,163,145,66,WHITE,bcolor,WHITE,"",1);
    bcolor =_devices[3]->getEvaluationColor();
    if (bcolor==BLACK) bcolor=GREEN;
    button4.initButtonUL(_tft,165,11,145,66,WHITE,bcolor,WHITE,"",1);
    bcolor =_devices[4]->getEvaluationColor();
    if (bcolor==BLACK) bcolor=GREEN;
    button5.initButtonUL(_tft,165,87,145,66,WHITE,bcolor,WHITE,"",1);
    bcolor =_devices[5]->getEvaluationColor();
    if (bcolor==BLACK) bcolor=GREEN;
    button6.initButtonUL(_tft,165,163,145,66,WHITE,bcolor,WHITE,"",1);
    
    
    button1.drawButton();
    _tft->drawBitmap(14, 20,_temperature_small , 48, 48, BLACK);
    _tft->setTextSize(2);
    _tft->setTextColor(BLACK);
    _tft->setCursor(70,29);
    _tft->print(_devices[0]->toString(false));
    _tft->setCursor(70,49);
    _tft->print(_devices[0]->getValueUnit());


    button2.drawButton();
    _tft->drawBitmap(14, 96, _pressure_small, 48, 48, BLACK);
    _tft->setTextSize(2);
    _tft->setTextColor(BLACK);
    _tft->setCursor(70,105);
    _tft->print(_devices[1]->toString(false));
    _tft->setCursor(70,125);
    _tft->print(_devices[1]->getValueUnit());

    button3.drawButton();
    _tft->drawBitmap(14, 172, _humidity_small, 48, 48, BLACK);
    _tft->setTextSize(2);
    _tft->setTextColor(BLACK);
    _tft->setCursor(70,181);
    _tft->print(_devices[2]->toString(false));
    _tft->setCursor(70,201);
    _tft->print(_devices[2]->getValueUnit());

    button4.drawButton();
    _tft->drawBitmap(170, 20,_air_small , 48, 48, BLACK);
    _tft->setTextSize(2);
    _tft->setTextColor(BLACK);
    _tft->setCursor(225,29);
    _tft->print(_devices[3]->toString(false));
    _tft->setCursor(225,49);
    _tft->print(_devices[3]->getValueUnit());

    button5.drawButton();
    _tft->drawBitmap(170, 96, _volume_small, 48, 48, BLACK);
    _tft->setTextSize(2);
    _tft->setTextColor(BLACK);
    _tft->setCursor(225,105);
    _tft->print(_devices[4]->toString(false));
    _tft->setCursor(225,125);
    _tft->print(_devices[4]->getValueUnit());

    button6.drawButton();
    _tft->drawBitmap(170, 172, _light_small, 48, 48, BLACK);
    _tft->setTextSize(2);
    _tft->setTextColor(BLACK);
    _tft->setCursor(225,181);
    _tft->print(_devices[5]->toString(false));
    _tft->setCursor(225,201);
    _tft->print(_devices[5]->getValueUnit());

}

void Screen::drawGraphMode(float values[], int xSize, float medium, float bad, String name) {
    int yScreenFrom = 40;
    int yScreenTo = 210;
    int xScreenFrom = 30;
    int xScreenTo = 310;
    float minVal = -UNSET;
    float maxVal = UNSET;
    int gridColor=0x8430;
    if (medium!=0) minVal = medium;
    if (bad!=0) maxVal = bad;
    
    for (int i = 0; i < xSize; i++) {
        if (values[i] > 0) {
            if (values[i] > maxVal) {
                maxVal = values[i];
            }
            if (values[i] < minVal) {
                minVal = values[i];
            }
        }
    }
    minVal = minVal - minVal * 0.15;
    maxVal = maxVal + maxVal * 0.15;
    int referenceLineDist = getGraphReferenceScaling(minVal, maxVal, 2);
    int firstLine = minVal + (referenceLineDist - ((int)minVal % referenceLineDist));
    _tft->fillScreen(BLACK);
    
    _tft->setTextSize(3);
    _tft->setTextColor(WHITE);
    _tft->setCursor(5, 5);
    _tft->print(name);


    for (int i = firstLine; i < maxVal; i += referenceLineDist) {
        int y1 = mapfloat(i, maxVal, minVal, yScreenFrom, yScreenTo);
        _tft->drawFastHLine(xScreenFrom, y1, _tft->width(), gridColor);
        _tft->setTextSize(1);
        _tft->setTextColor(gridColor);
        _tft->setCursor(5, y1 - 3);
        _tft->print(i);
    }
    if (medium!=0){
        _tft->drawFastHLine(xScreenFrom, mapfloat(medium, maxVal, minVal, yScreenFrom, yScreenTo), _tft->width(), YELLOW);
        _tft->setTextSize(1);
        _tft->setTextColor(YELLOW);
        _tft->setCursor(5, mapfloat(medium, maxVal, minVal, yScreenFrom, yScreenTo) - 3);
        _tft->print(medium);
    }
    if (bad!=0){
        _tft->drawFastHLine(xScreenFrom, mapfloat(bad, maxVal, minVal, yScreenFrom, yScreenTo), _tft->width(), RED);
        _tft->setTextSize(1);
        _tft->setTextColor(RED);
        _tft->setCursor(5, mapfloat(bad, maxVal, minVal, yScreenFrom, yScreenTo) - 3);
        _tft->print(bad);
    }
    int y_name=10;
    for (int i = 0; i < xSize - 1; i++) {
        int color=WHITE;
        float fromvalue = values[i];
        float tovalue = values[i + 1];
        if (fromvalue < 0 || tovalue < 0){
            fromvalue = minVal;
            tovalue = minVal;
            color=BLUE;
        } 
        int x1 = mapfloat(i, 0, xSize - 1, xScreenFrom, xScreenTo);
        int x2 = mapfloat(i + 1, 0, xSize - 1, xScreenFrom, xScreenTo);
        int y1 = mapfloat(fromvalue, maxVal, minVal, yScreenFrom, yScreenTo);
        int y2 = mapfloat(tovalue, maxVal, minVal, yScreenFrom, yScreenTo);
        _tft->drawLine(x1, y1, x2, y2, color);
        if(i%6==0){
            _tft->drawFastVLine(x1, yScreenTo, 5, gridColor);
            String axisscale=String(-y_name)+"m";
            y_name--;
            _tft->setTextSize(1);
            _tft->setTextColor(gridColor);
            _tft->setCursor(x1-10,yScreenTo+8);
            _tft->print(axisscale);
            if(y_name==0){
                _tft->drawFastVLine(xScreenTo, yScreenTo, 5, gridColor);
                String axisscale=String("0m");
                _tft->setCursor(xScreenTo-5,yScreenTo+8);
                _tft->print(axisscale);
            }
        }
    }
}
float Screen::getGraphReferenceScaling(float minVal, float maxVal, int numOfLines){
    numOfLines++;
    int range = abs(maxVal - minVal);
    if (range / 1000 >= numOfLines) {
        return 1000;
    }
    else if (range / 500 >= numOfLines) {
        return 500;
    }
    else if (range / 100 >= numOfLines) {
        return 100;
    }
    else if (range / 50 >= numOfLines) {
        return 50;
    }
    else if (range / 20 >= numOfLines) {
        return 20;
    }
    else if (range / 10 >= numOfLines) {
        return 10;
    }
    else if (range / 5 >= numOfLines) {
        return 5;
    }
    else {
        return 1;
    }
}

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
 return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}