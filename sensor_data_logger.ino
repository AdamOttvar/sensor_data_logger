/*
  Simple data logger for the Sparkfun LSM9DS1 IMU on an Arduino UNO with an Adafruit CC3000 wifi shield.

  Code for logging the data is taken from example code for the SD library and code for accessing
  IMU data is taken from example code from LSM9DS1 library, 
  see: https://github.com/sparkfun/LSM9DS1_Breakout

  Hardware setup:
  Arduinon UNO with Adafruit CC3000 wifi shield (with SD-card)
  Sparkfun LSM9DS1

  Hardware setup IMU: This library supports communicating with the
  LSM9DS1 over either I2C or SPI. This example demonstrates how
  to use I2C. The pin-out is as follows:
  LSM9DS1 --------- Wifi shield
   SCL --------------- SCL
   SDA --------------- SDA
   VDD --------------- 3.3V
   GND --------------- GND
  (CSG, CSXM, SDOG, and SDOXM should all be pulled high. 
  Jumpers on the breakout board will do this for you.)

  Hardware setup WiFi shield: The Arduino communicates with the
  SD card over SPI, No wiring is neede if the Adafruit CC3000 wifi shield is used.
  ** MOSI - pin 11
  ** MISO - pin 12
  ** CLK - pin 13
  ** CS - pin 4  
 */

#include <SPI.h>
#include <SD.h>

// The SFE_LSM9DS1 library requires both Wire and SPI be
// included BEFORE including the 9DS1 library.
#include <Wire.h>
#include <SparkFunLSM9DS1.h>

const int chipSelect = 4;
int counter = 0;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  Serial.println("card initialized.");
}

void loop() {
  // make a string for assembling the data to log:
  String dataString = String(counter) + String(counter+1) + String(counter+1);
  counter++;

  if (counter < 1000) {
    // open the file. note that only one file can be open at a time,
    // so you have to close this one before opening another.
    File dataFile = SD.open("datalog.txt", FILE_WRITE);
  
    // if the file is available, write to it:
    if (dataFile) {
      dataFile.println(dataString);
      dataFile.close();
      // print to the serial port too:
      Serial.println(dataString);
    }
    // if the file isn't open, pop up an error:
    else {
      Serial.println("error opening datalog.txt");
    }
  }
}
