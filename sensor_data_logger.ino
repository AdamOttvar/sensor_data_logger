/*
  Simple data logger for the Sparkfun LSM9DS1 IMU on an 
  Arduino UNO with an Adafruit CC3000 wifi shield.

  Code for logging the data is taken from example code for the 
  SD library and code for accessing IMU data is taken from 
  example code from LSM9DS1 library, 
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
  SD card over SPI, No wiring is neede if the Adafruit CC3000 
  wifi shield is used.
  ** MOSI - pin 11
  ** MISO - pin 12
  ** CLK - pin 13
  ** CS - pin 4  
 **************************************************************/

#include <SPI.h>
#include <SD.h>

// The SFE_LSM9DS1 library requires both Wire and SPI be
// included BEFORE including the 9DS1 library.
#include <Wire.h>
#include <SparkFunLSM9DS1.h>

/* SD card setup **********************************************/
const int chipSelect = 4;
int counter = 0;
const String filename = "datalog.txt";

/* IMU setup *************************************************/
// LSM9DS1 Library Init //
LSM9DS1 imu;

// Example I2C Setup //
// SDO_XM and SDO_G are both pulled high, so our addresses are:
#define LSM9DS1_M  0x1E // Would be 0x1C if SDO_M is LOW
#define LSM9DS1_AG  0x6B // Would be 0x6A if SDO_AG is LOW

// Sketch Output Settings //
#define PRINT_SPEED 50 // 50 ms between prints
static unsigned long lastPrint = 0; // Keep track of print time


void setup() {
  // Open serial communications:
  Serial.begin(115200);
  
  // Before initializing the IMU, there are a few settings
  // we may need to adjust. Use the settings struct to set
  // the device's communication mode and addresses:
  imu.settings.device.commInterface = IMU_MODE_I2C;
  imu.settings.device.mAddress = LSM9DS1_M;
  imu.settings.device.agAddress = LSM9DS1_AG;
  imu.settings.gyro.sampleRate = 2; // 59.5Hz ODR
  imu.settings.accel.sampleRate = 2; // Set accel to 50Hz.
  imu.settings.mag.sampleRate = 5; // Set OD rate to 20Hz
  
  // The above lines will only take effect AFTER calling
  // imu.begin(), which verifies communication with the IMU
  // and turns it on.
  if (!imu.begin())
  {
    Serial.println("Failed to communicate with LSM9DS1.");
    Serial.println("Double-check wiring.");
    while (1);
  }
  Serial.println("IMU initialized.");

  Serial.print("Initializing SD card...");
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  Serial.println("card initialized.");
  if (SD.exists(filename)) {
    Serial.println("Removed existing file.");
    SD.remove(filename);
  }
  File dataFile = SD.open(filename, FILE_WRITE);
  dataFile.println("Time, a.x, a.y, a.z, g.x, g.y, g.z, m.x, m.y, m.z");
  dataFile.close();
  
}

void loop() {
  // Update the sensor values whenever new data is available
  if ( imu.gyroAvailable() )
  {
    imu.readGyro();
  }
  if ( imu.accelAvailable() )
  {
    imu.readAccel();
  }
  if ( imu.magAvailable() )
  {
    imu.readMag();
  }
  
  // Print data to card at given interval
  if ((lastPrint + PRINT_SPEED) < millis())
  {
    // open the file. note that only one file can be open at a time,
    // so you have to close this one before opening another.
    File dataFile = SD.open(filename, FILE_WRITE);
  
    // if the file is available, write to it:
    if (dataFile) {
      dataFile.print(String(millis()));
      printAccel(dataFile);
      printGyro(dataFile);
      printMag(dataFile);
      dataFile.println();
      dataFile.close();
      // print to the serial port for debugging:
      Serial.println("Written to file");
    }
    // if the file isn't open, pop up an error:
    else {
      Serial.println("error opening datalog.txt");
    }
    lastPrint = millis(); // Update lastPrint time
  }
}

void printAccel(File dataFile)
{  
  // Print accel values as calculated g's
  dataFile.print(", ");
  dataFile.print(imu.calcAccel(imu.ax), 2);
  dataFile.print(", ");
  dataFile.print(imu.calcAccel(imu.ay), 2);
  dataFile.print(", ");
  dataFile.print(imu.calcAccel(imu.az), 2);

}

void printGyro(File dataFile)
{
  // Print gyro values as calculated DPS
  dataFile.print(", ");
  dataFile.print(imu.calcGyro(imu.gx), 2);
  dataFile.print(", ");
  dataFile.print(imu.calcGyro(imu.gy), 2);
  dataFile.print(", ");
  dataFile.print(imu.calcGyro(imu.gz), 2);
}

void printMag(File dataFile)
{  
  // Print magn values as calculated Gauss
  dataFile.print(", ");
  dataFile.print(imu.calcMag(imu.mx), 2);
  dataFile.print(", ");
  dataFile.print(imu.calcMag(imu.my), 2);
  dataFile.print(", ");
  dataFile.print(imu.calcMag(imu.mz), 2);
}
