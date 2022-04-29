//
//    FILE: readCalibration.ino
//  AUTHOR: Rob Tillaart
// PURPOSE: read the calibration values / errors for a flat sensor.
//    DATE: 2020-07-14


#include "GY521.h"
#define THRESHHOLD 1150.0
GY521 sensor(0x69);

uint32_t counter = 0;

float ax, ay, az;
float gx, gy, gz;
float t;


void setup()
{
  Serial.begin(115200);
  Serial.println(__FILE__);

  Wire.begin();
  delay(100);
  if (sensor.wakeup() == false)
  {
    Serial.println("Could not conect to GY521");
  }
  // adjust when needed.
  sensor.setAccelSensitivity(0);  // 2g
  sensor.setGyroSensitivity(0);   // 250 degrees/s
  sensor.setThrottle(false);

  // set all calibration errors to zero
  sensor.axe = 0;
  sensor.aye = 0;
  sensor.aze = 0;
  sensor.gxe = 0;
  sensor.gye = 0;
  sensor.gze = 0;

  Serial.println("\n\nReading calibration numbers...");
}


void loop()
{
  ax = ay = az = 0;
  gx = gy = gz = 0;
  t = 0;
  for (int i = 0; i < 20; i++)
  {
    sensor.read();
    ax -= sensor.getAccelX();
    ay -= sensor.getAccelY();
    az -= sensor.getAccelZ();
    gx -= sensor.getGyroX();
    gy -= sensor.getGyroY();
    gz -= sensor.getGyroZ();
    t += sensor.getTemperature();
  }

  if(gz > THRESHHOLD)
    Serial.println("right ");
  else if(gz < -THRESHHOLD)
    Serial.println("left ");
  if(gx > THRESHHOLD)
    Serial.println("down");
  else if(gx < -THRESHHOLD)
    Serial.println("up");
    
  // adjust calibration errors so table should get all zero's.
    sensor.axe += ax * 0.05;
    sensor.aye += ay * 0.05;
    sensor.aze += az * 0.05;
    sensor.gxe += gx * 0.05;
    sensor.gye += gy * 0.05;
    sensor.gze += gz * 0.05;

  counter++;
  delay(100);
}


// -- END OF FILE --
