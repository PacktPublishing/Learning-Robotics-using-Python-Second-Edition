/* Service robot code, sensor only version

*/
//MPU 6050 Interfacing libraries

#include <Wire.h>

//#include "I2Cdev.h"
#include "MPU6050.h"

MPU6050 accelgyro(0x68);

void setup()
{
  
  //Init Serial port with 115200 buad rate
  Serial.begin(115200);  
  Setup_MPU6050();
}

void Setup_MPU6050()
{
   Wire.begin();
  
      // initialize device
    Serial.println("Initializing I2C devices...");
    accelgyro.initialize();

    // verify connection
    Serial.println("Testing device connections...");
    Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
 
}

void loop()
{

   //Update MPU 6050
    Update_MPU6050();
 
}

void Update_MPU6050()
{
  
   int16_t ax, ay, az;
  int16_t gx, gy, gz;

      // read raw accel/gyro measurements from device
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

    // display tab-separated accel/gyro x/y/z values
    Serial.print("i");Serial.print("\t");
    Serial.print(ax); Serial.print("\t");
    Serial.print(ay); Serial.print("\t");
    Serial.print(az); Serial.print("\t");
    Serial.print(gx); Serial.print("\t");
    Serial.print(gy); Serial.print("\t");
    Serial.println(gz);
    Serial.print("\n");

}


