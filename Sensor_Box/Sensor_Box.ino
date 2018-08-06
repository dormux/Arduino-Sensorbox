#include <Directives.h>
#include <DHT.h>
#include <Adafruit_TSL2591.h>
#include <Adafruit_MPL3115A2.h>
#include <Adafruit_CCS811.h>
#include <Sensor.h>
#include <SensorHumidity.h>
#include <SensorTemperature.h>
#include <SensorLight.h>
#include <SensorAudioVolume.h>
#include <SensorAirCO.h>
#include <SensorPressure.h>
#include <SensorAirVOC.h>
#include <Screen.h>
#include <GPRSCommunicator.h>
#include <rtc_clock.h>


SensorDevice* humidity;
SensorDevice* temperature;
SensorDevice* light;
SensorDevice* volume;
SensorDevice* pressure;
SensorDevice* air_co;
SensorDevice* air_voc;
Screen* screen;
GPRSCommunicator* gprscommunicator;
RTC_clock* rtcclock;

unsigned long timerTick;
unsigned long timerTickSendData;


void setup() {
  Serial.begin(9600);
  DHT* dht11 = new DHT(DHTPIN, DHTTYPE);
  Adafruit_TSL2591* tsl = new Adafruit_TSL2591(2591);
  Adafruit_MPL3115A2* mpl = new Adafruit_MPL3115A2();
  Adafruit_CCS811* ccs = new Adafruit_CCS811();
  SensorHumidity* tmpdhtH = new SensorHumidity(dht11);
  SensorTemperature* tmpdhtT = new SensorTemperature(dht11);
  SensorLight * tmpLight = new SensorLight(tsl);
  SensorAudioVolume* tmpAudio = new SensorAudioVolume();
  SensorPressure* tmpPressure = new SensorPressure(mpl);
  SensorAirVOC * tmpAriVOC = new SensorAirVOC();
  humidity = tmpdhtH;
  temperature = tmpdhtT;
  light = tmpLight;
  volume = tmpAudio;
  air_voc=tmpAriVOC;
  pressure = tmpPressure;
  humidity->begin();
  temperature->begin();
  light->begin();
  volume->begin();
  pressure->begin();
  air_voc->begin();
  //Create CO Sensor here, because it needs somer values from temp/humidity sensor
  humidity->measure();
  temperature->measure();
  humidity->publish();
  temperature->publish();
  float hum=humidity->getMeasurement();
  float temp=temperature->getMeasurement();
  if(hum=UNSET) hum=50.0f;
  if(temp=UNSET) temp=20.0f;
  SensorAirCO* tmpAirCO = new SensorAirCO(ccs,hum,temp);
  air_co=tmpAirCO;
  screen = new Screen(temperature,pressure,humidity,air_co,volume,light);

  //Set Up GPRS module
  gprscommunicator = new  GPRSCommunicator("dweet.io/dweet/for/94fed198888c97cd37afa375cfef4641");
  initDate();
  gprscommunicator->setRtcClock(rtcclock);
  timerTick = rtcclock->unixtime(0);
  timerTickSendData=timerTick;
}

void loop() {
  while (rtcclock->unixtime(0) - timerTick < TIMER) {
    volume->measure();
    screen->handleInput();
  }
  volume->publish();
  timerTick += TIMER;

  humidity->measure();
  temperature->measure();
  light->measure();
  pressure->measure();
  air_co->measure();
  air_voc->measure();
  
  humidity->publish();
  temperature->publish();
  light->publish();
  pressure->publish();
  air_co->publish();
  air_voc->publish();
  if(!gprscommunicator->getError() && rtcclock->unixtime(0) - timerTickSendData > 600){
    gprscommunicator->sendData(temperature->getAvarage(60),humidity->getAvarage(60),pressure->getAvarage(60),air_co->getAvarage(60),volume->getAvarage(60),light->getAvarage(60));
    timerTickSendData+=600;
  }
  screen->draw();
}

void initDate(){
    int year = 2018;
    int month = 1;
    int day = 1;
    int hour = 0;
    int minute = 0;
    int second = 0;
  if(!gprscommunicator->getError()){
    String rawtime=gprscommunicator->getDateTime();
    year = 2000+rawtime.substring(1,3).toInt();
    month = rawtime.substring(4,6).toInt();
    day = rawtime.substring(7,9).toInt();
    hour = rawtime.substring(10,12).toInt()+2;
    minute = rawtime.substring(13,15).toInt();
    second = rawtime.substring(16,18).toInt();
  }
  rtcclock = new RTC_clock(XTAL);
  rtcclock->init();
  rtcclock->set_time(hour, minute, second);
  rtcclock->set_date(day, month, year);

}

