#include <AsyncTCP.h>
#include <AsyncEventSource.h>
#include <AsyncJson.h>
#include <AsyncWebSocket.h>
#include <AsyncWebSynchronization.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFSEditor.h>
#include <StringArray.h>
#include <WebAuthentication.h>
#include <WebHandlerImpl.h>
#include <WebResponseImpl.h>

#include "ArduinoJson.h"

#include <Wire.h>
#include "time.h"

AsyncWebServer server(80);

const int orzesioneLightOnHour = 9;
const int orzesioneLightOffHour = 21;

const int orzesioneLightPin = 12;
const int redLightPin = 13;
const int waterPin = 14;

const int waterOnTime = 14 * 1000; // misting time
const int waterOn1 = 10;
const int waterOn2 = 3; //13;
const int waterOn3 = 16;
const int waterOn4 = 22;

const char* ssid       = ""; 
const char* password   = "";

const char* ntpServer = "europe.pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600; 

void setup () {
    Serial.begin(9600);
    Wire.begin();

  pinMode(orzesioneLightPin, OUTPUT);
  pinMode(redLightPin, OUTPUT);
  pinMode(waterPin, OUTPUT);

  digitalWrite(orzesioneLightPin, HIGH);
  digitalWrite(redLightPin, HIGH);
  digitalWrite(waterPin, HIGH);

  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);

  //connect to WiFi
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  Serial.println(" CONNECTED");

  while (WiFi.waitForConnectResult() != WL_CONNECTED)
  {
    Serial.printf("WiFi Failed!\n");
  }
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/switch-red", HTTP_GET, [](AsyncWebServerRequest *request) {
     if (request->hasParam("message"))
    {
      if(request->getParam("message")->value() == "red-on")   
      {
        digitalWrite(redLightPin, LOW);
      }
      else
      {
        digitalWrite(redLightPin, HIGH);
      }
    }
   
    request->send(200, "application/json", "{\"message\":\"Welcome Led\"}");
  });

  
  server.on("/switch-water", HTTP_GET, [](AsyncWebServerRequest *request) {
     if (request->hasParam("message"))
    {
      if(request->getParam("message")->value() == "water-on") 
      {
        digitalWrite(waterPin, LOW);
      }
      else
      {
        digitalWrite(waterPin, HIGH);
      }
    }
   
    request->send(200, "application/json", "{\"message\":\"Welcome Led\"}");
  });

   server.begin();
                                  
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  ShowDateOnSerial();

}

void loop() {    
  delay(1000);

  ShowDateOnSerial();

  SwitchMainLight(orzesioneLightOnHour, orzesioneLightOffHour, orzesioneLightPin);
  Water();  
}

void SwitchMainLight(int onHour, int offHour, int lightPin) 
{  
     time_t now = time(NULL);
    struct tm *ptm = localtime(&now);
  if (ptm->tm_hour >= onHour && ptm->tm_hour < offHour)
  {
    digitalWrite(lightPin, LOW);
  }
  else
  {
    digitalWrite(lightPin, HIGH);
  }
}

void Water(){

 time_t now = time(NULL);
    struct tm *ptm = localtime(&now);
    
  if (ptm->tm_hour == waterOn1 && ptm->tm_min == 1)
  {
    digitalWrite(waterPin, LOW);
    delay(waterOnTime);
    digitalWrite(waterPin, HIGH);
    delay(70000);
  }
  
  if (ptm->tm_hour == waterOn2 && ptm->tm_min == 1)
  {
    digitalWrite(waterPin, LOW);
    delay(waterOnTime);
    digitalWrite(waterPin, HIGH);
    delay(70000);
  }
  
  if (ptm->tm_hour == waterOn3 && ptm->tm_min == 1)
  {
    digitalWrite(waterPin, LOW);
    delay(waterOnTime);
    digitalWrite(waterPin, HIGH);
    delay(70000);
  }

  if (ptm->tm_hour == waterOn4 && ptm->tm_min == 1)
  {
    digitalWrite(waterPin, LOW);
    delay(waterOnTime);
    digitalWrite(waterPin, HIGH);
    delay(70000);
  }  
}


void ShowDateOnSerial()
{
 struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}
