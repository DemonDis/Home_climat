/*
Схема подключения:

Модуль SD карты подключен в SPI по стандартной схеме:
  -------------
  MOSI - pin 11
  MISO - pin`12
  CLK  - pin 13
  CS   - pin 10/8
  -------------

Схема времени:
  -------------
  CDA  - pin A4
  SCL  - pin A5
  -------------
 */
 
#include <TroykaMQ.h>
#include <SD.h>
#include <SPI.h>
#include "DHT.h"
#include <DS3231.h>

#define DHTPIN      2
#define DHTTYPE     DHT11
#define PIN_MQ135   A1

DHT     dht  (DHTPIN, DHTTYPE);
DS3231  rtc  (SDA, SCL);
MQ135   mq135(PIN_MQ135);

File myFile;

// Подключение SD
const uint8_t   CS_pin    = 10; // SD card

// Подключение датчиков
//int             sensorPin      = A0;   // датчик влажности
//int             sensorPWR      = 7;    // питание датчика влажности
int             pinlight       = A3;   // датчик света

// подключение RGB-светодиода
const int       pinRED         = 5;    // красный
const int       pinGREEN       = 4;    // зеленый
const int       pinBLUE        = 6;    // синий

// подключение красного светодиода
const int       pinREDblick    = 9;    // красный мигающий

// реле
//int             relay          = 10;   // реле

//------------------------------------------------------
void setup()
{
  //digitalWrite(10, HIGH);
  
  Serial.begin(9600);
  Serial.println("Initializing SD card OK!");

  pinMode(CS_pin,       OUTPUT);
  //pinMode(7,            OUTPUT);
  //pinMode(A0,           INPUT);
  pinMode(A3,           INPUT);
  pinMode(pinRED,       OUTPUT);
  pinMode(pinGREEN,     OUTPUT);
  pinMode(pinBLUE,      OUTPUT);
  pinMode(pinREDblick,  OUTPUT);
  //pinMode(3,            OUTPUT);

    mq135.calibrate(113);
    Serial.print("Ro = ");
    Serial.println(mq135.getRo());
    
    rtc.begin();
    Serial.println("rtc ok!");
    dht.begin();
    Serial.println("dht ok!");
    
  if (!SD.begin(13))
    {
    Serial.println("initialization failed!");
    return;
    }
  delay(1500);
}

//------------------------------------------------------
void loop()
{
   //digitalWrite(sensorPWR,HIGH);    // включить датчик уровня влажности почвы
   //delay(50);                       // время необходимое датчику для вкл и замера 
   //digitalWrite(sensorPWR,LOW);     // выключить датчик уровня влажности почвы
   
   //int sensorSOIL   = analogRead(sensorPin);
   
   int xD0;

Serial.println("conect modul SD ok");

myFile = SD.open("CO2.txt", FILE_WRITE);

if (myFile)
{
   Serial.print(rtc.getTimeStr());
   Serial.print(", "); 
   Serial.print(rtc.getDateStr());
   Serial.print(" -- ");
   myFile.print(rtc.getTimeStr());
   myFile.print(", "); 
   myFile.print(rtc.getDateStr());
   myFile.print(" -- ");

  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);

  if (isnan(h) || isnan(t) || isnan(f))
  {
    Serial.println("Failed to read from DHT sensor!");
    myFile.println("Failed to read from DHT sensor!");
    return;
  }
  float hi = dht.computeHeatIndex(f, h);
  Serial.print("Temperature: "); 
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print(f);
  Serial.print(" *F\n");
  myFile.print("Temperature: "); 
  myFile.print(t);
  myFile.print(" *C ");
  myFile.print(f);
  myFile.println(" *F\n");
  
/*   
// Показание датчика влажности

   Serial.print(rtc.getTimeStr());
   Serial.print(", "); 
   Serial.print(rtc.getDateStr());
   Serial.print(" -- ");
   myFile.print(rtc.getTimeStr());
   myFile.print(", "); 
   myFile.print(rtc.getDateStr());
   myFile.print(" -- ");
  
if(sensorSOIL < 1000 && sensorSOIL >= 600)
    { 
     Serial.print("DRY (1)");
     myFile.print("DRY (1)");
     Serial.print(" -- ");
     myFile.print(" -- ");
     Serial.println(sensorSOIL);
     myFile.println(sensorSOIL);
    }
  if(sensorSOIL < 600 && sensorSOIL >= 370)
    {
     Serial.print("HUMID (1)");
     myFile.print("HUMID (1)");
     Serial.print(" -- ");
     myFile.print(" -- ");
     Serial.println(sensorSOIL);
     myFile.println(sensorSOIL);
    }
  if(sensorSOIL < 370)
    {
     Serial.print("WATER (1)");
     myFile.print("WATER (1)");
     Serial.print(" -- ");
     myFile.print(" -- ");
     Serial.println(sensorSOIL);
     myFile.println(sensorSOIL);
     }
*/     
// Показание датчика света

   Serial.print(rtc.getTimeStr());
   Serial.print(", "); 
   Serial.print(rtc.getDateStr());
   Serial.print(" -- ");
   myFile.print(rtc.getTimeStr());
   myFile.print(", "); 
   myFile.print(rtc.getDateStr());
   myFile.print(" -- ");
   
  xD0 = digitalRead (pinlight);
  Serial.print      ("IN = ");
  Serial.println    (xD0, DEC);
  myFile.print      ("IN = "); 
  myFile.println    (xD0, DEC);

// Показание датчика MQ-135

   Serial.print(rtc.getTimeStr());
   Serial.print(", "); 
   Serial.print(rtc.getDateStr());
   Serial.print(" -- ");
   myFile.print(rtc.getTimeStr());
   myFile.print(", "); 
   myFile.print(rtc.getDateStr());
   myFile.print(" -- ");
   
  Serial.print("Ratio: ");
  Serial.print(mq135.readRatio());
  myFile.print("Ratio: ");
  myFile.print(mq135.readRatio());
  
  Serial.print("\tCO2: ");
  Serial.print(mq135.readCO2());
  Serial.println(" ppm");
  myFile.print("\tCO2: ");
  myFile.print(mq135.readCO2());
  myFile.println(" ppm");
  
myFile.flush(); 
myFile.close();
Serial.println("save");
  RGB();
  //RelayPlay();
  //Serial.print("Temperature: ");
  //Serial.print(rtc.getTemp());
  //Serial.println(" C");
  }
  else 
  {
    Serial.println("error opening CO2.txt");
     digitalWrite (pinREDblick, HIGH);
     delay(5000);
     digitalWrite (pinREDblick, LOW);
  }
delay (300000); // циклы
}

//------------------------------------------------------
void RGB()
{
  //digitalWrite(sensorPWR,HIGH);
  //delay(50);
  //digitalWrite(sensorPWR,LOW);

  float t = dht.readTemperature();
  float f = dht.readTemperature(true);
  
  //int sensorSOIL   = analogRead(sensorPin);
  Serial.println("-----------------------");
  
// Работа RGB влажность почвы
/*
  if(sensorSOIL < 1000 && sensorSOIL >= 600)
    { 
     Serial.print(sensorSOIL);
     Serial.println(" - RED - DRY(SOIL)");
     digitalWrite (pinRED, HIGH);
     delay(500);
     digitalWrite (pinRED, LOW);
     delay(500);
    }
  if(sensorSOIL < 600 && sensorSOIL >= 370)
    {
     Serial.print(sensorSOIL);
     Serial.println(" - GREEN - HUMID(SOIL)");
     digitalWrite (pinGREEN, HIGH);
     delay(500);
     digitalWrite (pinGREEN, LOW);
     delay(500);
    }
  if(sensorSOIL < 370)
    {
     Serial.print(sensorSOIL);
     Serial.println(" - GREEN - WATER(SOIL)");
     digitalWrite (pinBLUE, HIGH);
     delay(500);
     digitalWrite (pinBLUE, LOW);
     delay(500);
     }
*/
// Работа RGB влажность
   
  if(f<40)
  {
     Serial.print(f);
     Serial.println(" - RED - DRY");
     digitalWrite (pinRED, HIGH);
     delay(1000);
     digitalWrite (pinRED, LOW);
     delay(1000);
  }
  else if(f>=40 && f<=75)
  {
     Serial.print(f);
     Serial.println(" - GREEN - Humid");
     digitalWrite (pinGREEN, HIGH);
     delay(1000);
     digitalWrite (pinGREEN, LOW);
     delay(1000);
  }
  else if(f>75)
  {
     Serial.print(f);
     Serial.println(" - BLUE - VERY Humid");
     digitalWrite (pinBLUE, HIGH);
     delay(1000);
     digitalWrite (pinBLUE, LOW);
     delay(1000);
  }

// Работа RGB влажность2
//https://blog.kvv213.com/2016/11/otslezhivaem-soderzhanie-co2-v-vozduhe-pri-pomoshhi-wemos-d1-mini-pro-thingspeak-i-mhz-19/
 
 switch (mq135.readCO2())
   {
   case 0 ... 400:
     Serial.print(mq135.readCO2());
     Serial.println(" - GOOD");
     digitalWrite (pinGREEN, HIGH);
     delay(1000);
     digitalWrite (pinGREEN, LOW);
     delay(500);
     digitalWrite (pinGREEN, HIGH);
     delay(1000);
     digitalWrite (pinGREEN, LOW);
     delay(500);
     digitalWrite (pinGREEN, HIGH);
     delay(1000);
     digitalWrite (pinGREEN, LOW);
     delay(1000);
     break;
      
   case 401 ... 600:
     Serial.print(mq135.readCO2());
     Serial.println(" - NORMAL");
     digitalWrite (pinBLUE, HIGH);
     delay(1000);
     digitalWrite (pinBLUE, LOW);
     delay(500);
     digitalWrite (pinBLUE, HIGH);
     delay(1000);
     digitalWrite (pinBLUE, LOW);
     delay(500);
     digitalWrite (pinBLUE, HIGH);
     delay(1000);
     digitalWrite (pinBLUE, LOW);
     delay(1000);
      break;
      
   case 601 ... 800:
     Serial.print(mq135.readCO2());
     Serial.println(" - NOT NORMAL");
     digitalWrite (pinRED, HIGH);
     delay(1000);
     digitalWrite (pinRED, LOW);
     delay(500);
     digitalWrite (pinREDblick, HIGH);
     delay(1000);
     digitalWrite (pinREDblick, LOW);
     delay(1000);
      break;
      
   case 801 ... 1000:
     Serial.print(mq135.readCO2());
     Serial.println(" - BAD");
     digitalWrite (pinRED, HIGH);
     delay(1000);
     digitalWrite (pinRED, LOW);
     delay(500);
     digitalWrite (pinREDblick, HIGH);
     digitalWrite (pinRED, HIGH);
     delay(1000);
     digitalWrite (pinREDblick, LOW);
     digitalWrite (pinRED, LOW);
     delay(1000);
     break;
     
     case 1001 ... 4000:
     Serial.print(mq135.readCO2());
     Serial.println(" - VERY BAD!!!!!!!!!");
     digitalWrite (pinRED, HIGH);
     digitalWrite (pinREDblick, HIGH);
     delay(6000);
     digitalWrite (pinRED, LOW);
     digitalWrite (pinREDblick, LOW);
     delay(1000);
     break;
     
   default:
     Serial.println("BREAK!!!!!!!!!");
     break;
   }
   
// Работа красного миганющего светодиода температура

  if(t<17)
  {
     Serial.print(t);
     Serial.println(" - REDblick - Temp - COLD");
     digitalWrite (pinREDblick, HIGH);
     delay(4000);
     digitalWrite (pinREDblick, LOW);
     delay(1000);
  }
  else if(t>25)
  {
     Serial.print(t);
     Serial.println(" - REDblick - Temp - HOT");
     digitalWrite (pinREDblick, HIGH);
     delay(4000);
     digitalWrite (pinREDblick, LOW);
     delay(1000);
  }
  else
  {
     Serial.print(t);
     Serial.println(" - REDblick - NORMAL");
  }
  Serial.println("-----------------------");
}
/*
//------------------------------------------------------
void RelayPlay()
{
  digitalWrite(sensorPWR,HIGH);
  delay(50);
  digitalWrite(sensorPWR,LOW);

   if (digitalRead(sensorPin) == 1)
   {
     Serial.println("ON RELAY!!!");
     digitalWrite(relay,LOW);
     delay(5000); 
     digitalWrite(relay,HIGH);

Serial.println("-----------------------");
   }
}
*/
