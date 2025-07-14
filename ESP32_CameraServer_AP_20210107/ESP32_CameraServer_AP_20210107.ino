#include "CameraWebServer_AP.h"
#include <WiFi.h>
#include "esp_camera.h"
WiFiServer server(100);

#define RXD2 33 
#define TXD2 4
CameraWebServer_AP CameraWebServerAP;

bool WA_en = false;
void setup()
{
  
  CameraWebServerAP.CameraWebServer_AP_Init();
  server.begin();

}
void loop()
{

}
