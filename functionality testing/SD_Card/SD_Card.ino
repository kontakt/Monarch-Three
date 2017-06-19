
#include <SD.h>
#include <SPI.h>

const int chipSelect = BUILTIN_SDCARD;

void setup() {
  SD.begin(chipSelect);
  Serial.begin(115200);
  Serial4.begin(19200);
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
}

void loop() {
  String dataString = "";
  File dataFile = SD.open("datalog.txt", FILE_WRITE);
  delay(100);
  while(Serial4.available()){
    dataString += (String(Serial4.read()));
  }
  Serial.println(dataString);
  dataFile.println(dataString);
  dataFile.close();
}
