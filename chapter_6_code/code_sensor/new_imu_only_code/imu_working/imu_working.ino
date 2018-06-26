
#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"


//Creating MPU6050 Object
MPU6050 accelgyro(0x68);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//DMP options
//Set true if DMP init was successful
bool dmpReady = false;
//Holds actual interrupt status byte from MPU
uint8_t mpuIntStatus;
//return status after each device operation
uint8_t devStatus;
//Expected DMP packet size
uint16_t packetSize;
//count of all bytes currently in FIFO
uint16_t fifoCount;
//FIFO storate buffer
uint8_t fifoBuffer[64];


#define OUTPUT_READABLE_QUATERNION

////////////////////////////////////////////////////////////////////////////////////////////////

//orientation/motion vars

Quaternion q;
//VectorInt16 aa;
//VectorInt16 aaReal;
//VectorInt16 aaWorld;
//VectorFloat gravity;

//float euler[3];
//float ypr[3];

//INTERRUPT DETECTION ROUTINE

volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
void dmpDataReady() {
    mpuInterrupt = true;
}


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
    
    //Initialize DMP in MPU 6050
    Setup_MPU6050_DMP();
}

//Setup MPU 6050 DMP
void Setup_MPU6050_DMP()
{
  
     //DMP Initialization
   devStatus = accelgyro.dmpInitialize();
   accelgyro.setXGyroOffset(220);
   accelgyro.setXGyroOffset(76);
   accelgyro.setXGyroOffset(-85); 
   accelgyro.setXGyroOffset(1788);  
   if(devStatus == 0){
    
		accelgyro.setDMPEnabled(true);
		pinMode(PUSH2,INPUT_PULLUP);    
		attachInterrupt(PUSH2, dmpDataReady, RISING);
		mpuIntStatus = accelgyro.getIntStatus();
		dmpReady = true;
		packetSize = accelgyro.dmpGetFIFOPacketSize();
     
	}
	else{
     
		;
    }
  
    
}

void loop()
{

	
    //Update MPU 6050
    Update_MPU6050();
}
void Update_MPU6050()
{
  
 
	  Update_MPU6050_DMP();

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Update MPU6050 DMP functions

void Update_MPU6050_DMP()
{
  
	//DMP Processing

    if (!dmpReady) return;
    
    while (!mpuInterrupt && fifoCount < packetSize) {
      
			;    
      
    }



    mpuInterrupt = false;
    mpuIntStatus = accelgyro.getIntStatus();
    
    //get current FIFO count
    fifoCount = accelgyro.getFIFOCount();
    
    
    if ((mpuIntStatus & 0x10) || fifoCount > 512) {
        // reset so we can continue cleanly
        accelgyro.resetFIFO();
    }

else if (mpuIntStatus & 0x02) {
        // wait for correct available data length, should be a VERY short wait
        while (fifoCount < packetSize) fifoCount = accelgyro.getFIFOCount();

        // read a packet from FIFO
        accelgyro.getFIFOBytes(fifoBuffer, packetSize);
        
        // track FIFO count here in case there is > 1 packet available
        // (this lets us immediately read more without waiting for an interrupt)
        fifoCount -= packetSize;

        #ifdef OUTPUT_READABLE_QUATERNION
            // display quaternion values in easy matrix form: w x y z
        accelgyro.dmpGetQuaternion(&q, fifoBuffer);
            
            
        Serial.print("i");Serial.print("\t");
        Serial.print(q.x); Serial.print("\t");
        Serial.print(q.y); Serial.print("\t");
        Serial.print(q.z); Serial.print("\t");
        Serial.print(q.w);
        Serial.print("\n");
            
        #endif
    }
}


