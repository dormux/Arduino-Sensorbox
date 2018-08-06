#ifndef DIRECTIVES_H
#define DIRECTIVES_H



//****General Directives
#define UNSET -32768 //<- Error Value for wrong measurements
#define TIMER 10 //defines the interval for the main loop in seconds
#define UNIQUE_ID 94fed198888c97cd37afa375cfef4641 //<- This has to bes by every box differently
//****Humidity Sensor DHT directives
#define DHTPIN 8
#define DHTTYPE DHT11
//****Audio Sensor Analog Pin 
#define AUDIOPIN A6
//****VOC Sensor Analog Pin 
#define VOCPIN A7
//****LCD Display Directives
//**Output Pins
// The control pins for the LCD can be assigned to any digital or
// analog pins...but we'll use the analog pins as this allows us to
// double up the pins with the touch screen (see the TFT paint example).
#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0
#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin
//**Touch pins LCD
#define YP A3  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 7
#define XP 6
//**Touch XY Values measured in the corners of the touchscreen
//**necessary to get correct xy position
//#define TS_MINX 170
//#define TS_MINY 145
//#define TS_MAXX 780
//#define TS_MAXY 880
#define TS_MINX 180//150
#define TS_MINY 170//120
#define TS_MAXX 770//920
#define TS_MAXY 860//940
//**Color definitions: LCD uses RGB565 values (picker http://www.barth-dev.de/online/rgb565-color-picker/)
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

#define GASGOOD   0x2B00
#define AUDIOGOOD 0x44A0
#define LIGHTGOOD 0x8E4B

#define GASOK   0xA360
#define AUDIOOK 0xFD40
#define LIGHTOK 0xFDE8

#define GASBAD   0xA000
#define AUDIOBAD 0xF800
#define LIGHTBAD 0xFA08

//****FONA Directives
#define FONA_RX 19
#define FONA_TX 18
#define FONA_RST 4


#endif /* DIRECTIVES_ */
