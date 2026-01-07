#include "CameraWebServer_AP.h"
#include <WiFi.h>
#include "esp_camera.h"
#include "FS.h"
#include "SD_MMC.h"

WiFiServer server(80);
CameraWebServer_AP CameraWebServerAP;

bool WA_en = false;

File logFile;

void setup()
{
    Serial.begin(57600); 

  CameraWebServerAP.CameraWebServer_AP_Init();
  server.begin();
  // Initialize the MicroSD
  Serial.print("Initializing the MicroSD card module... ");
  initMicroSDCard();
  // Create a header row in the file if it's a new file
  logFile = SD_MMC.open("/datalog.txt", FILE_APPEND);
  if (logFile) {
    if (logFile.size() == 0) { // Check if file is empty
      logFile.println("Timestamp (ms),Sensor Value");
    }
    logFile.close();
  } 

}
void loop()
{

String dataString = Serial.readStringUntil('\n');
 
 if (Serial.available() > 0)
 {
 // Open the log file in append mode
  logFile = SD_MMC.open("/datalog.txt", FILE_APPEND);
  
  if (logFile) {
    logFile.println(dataString); // Write data and a newline character
    logFile.close(); // Close the file immediately to ensure data is saved
  } 
 } 
}

// extra functions:
void initMicroSDCard() {
  // Start the MicroSD card

  Serial.println("Mounting MicroSD Card");
  if (!SD_MMC.begin()) {
    Serial.println("MicroSD Card Mount Failed");
    return;
  }
  uint8_t cardType = SD_MMC.cardType();
  if (cardType == CARD_NONE) {
    Serial.println("No MicroSD Card found");
    return;
  }

}
