#include <SD.h>
#include <SPI.h>

const int chipSelect = 10;

void setup() {
  Serial.begin(115200);
  Serial1.begin(9600);    // Connection to sensor
  SD.begin(chipSelect);
}

void loop() {
  String dataString = "";
  dataString += String(millis());
  dataString += ",";
  Serial1.write("@253PR1?;FF");     // Request pressure sensor
  delay(50);
  if(Serial1.available()) {      // While there is serial data
    dataString += Serial1.readString();
  }
  Serial.println(dataString);
  File dataFile = SD.open("datalog.txt", FILE_WRITE);
  dataFile.println(dataString);
  dataFile.close();  
  delay(50);             // Sample at 10Hz
}

