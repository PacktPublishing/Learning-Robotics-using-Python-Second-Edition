/* Service robot code, sensor only version

*/
//MPU 6050 Interfacing libraries

#include "Wire.h"

#include "I2Cdev.h"
#include "MPU6050.h"


//Time limit
#include <limits.h>

//Creating MPU6050 Object

MPU6050 accelgyro(0x68);

//Handling incoming serial data 
#include <Messenger.h>

//Messenger object

Messenger Messenger_Handler = Messenger();
///////////////////////////////////////////////////////////////
//Encoder pins definition

// Left encoder

#define Left_Encoder_PinA 31
#define Left_Encoder_PinB 32

volatile long Left_Encoder_Ticks = 0;
volatile bool LeftEncoderBSet;

//Right Encoder

#define Right_Encoder_PinA 33
#define Right_Encoder_PinB 34
volatile long Right_Encoder_Ticks = 0;
volatile bool RightEncoderBSet;

/////////////////////////////////////////////////////////////////
//Motor Pin definition
//Left Motor

#define A_1 12
#define B_1 13

//Fine tune this pin number
#define PWM_1 PC_6


//Right Motor
#define A_2 5 
#define B_2 6

//Fine tune this pin number
#define PWM_2 PC_5

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Ultrasonic pins definition
const int echo = 9, Trig = 10;
long duration, cm;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Battery level monitor
#define BATTERY_SENSE_PIN PC_4

float battery_level = 12;

/////////////////////////////////////////////////////////////////////////////////////////
//Time  update variables

unsigned long LastUpdateMicrosecs = 0;		
unsigned long LastUpdateMillisecs = 0;
unsigned long CurrentMicrosecs = 0;
unsigned long MicrosecsSinceLastUpdate = 0;
float SecondsSinceLastUpdate = 0;

///////////////////////////////////////////////////////////////////////////////////////

//Motor speed from PC
//Motor left and right speed
float motor_left_speed = 0;
float motor_right_speed = 0;
/////////////////////////////////////////////////////////////////

#define RESET_PIN PB_2

void setup()
{
  
  //Init Serial port with 115200 buad rate
  Serial.begin(115200);  
  
  //Setup Encoders
  SetupEncoders();
  //Setup Motors
  SetupMotors();
  //Setup Ultrasonic
  //SetupUltrasonic();  
  //Setup MPU 6050
  //Setup_MPU6050();
  //Setup Reset pins
  SetupReset();
  //Set up Messenger 
  Messenger_Handler.attach(OnMssageCompleted);
    
  
  
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//MPU Variable declaration
//int16_t ax, ay, az;
//int16_t gx, gy, gz;

//int16_t ax, ay, az;
//int16_t gx, gy, gz;

////////////////////////////////////////////////////////////////////////////////////////

//SetupEncoders() Definition

void SetupEncoders()
{
  // Quadrature encoders
  // Left encoder
  pinMode(Left_Encoder_PinA, INPUT_PULLUP);      // sets pin A as input  
  pinMode(Left_Encoder_PinB, INPUT_PULLUP);      // sets pin B as input
  attachInterrupt(Left_Encoder_PinA, do_Left_Encoder, RISING);
  

  // Right encoder
  pinMode(Right_Encoder_PinA, INPUT_PULLUP);      // sets pin A as input
  pinMode(Right_Encoder_PinB, INPUT_PULLUP);      // sets pin B as input

  attachInterrupt(Right_Encoder_PinA, do_Right_Encoder, RISING); 

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Setup Motors() function

void SetupMotors()
{
 
 //Left motor
 pinMode(A_1,OUTPUT);
 pinMode(B_1,OUTPUT); 
 

 //Right Motor
 pinMode(A_2,OUTPUT);
 pinMode(B_2,OUTPUT);  
 //pinMode(PWM_2,OUTPUT);
  
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Setup UltrasonicsSensor() function
void SetupUltrasonic()
{
 pinMode(Trig, OUTPUT);
 pinMode(echo, INPUT); 
  
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

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Setup Reset() function

void SetupReset()

{
  
  
  pinMode(GREEN_LED,OUTPUT);
  pinMode(RESET_PIN,OUTPUT);
  
  
  ///Conenect RESET Pins to the RESET pin of launchpad,its the 16th PIN
  digitalWrite(RESET_PIN,HIGH);
  
  
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//MAIN LOOP












void loop()
{

    //Read from Serial
    Read_From_Serial();
    
    
    //Update Time
    Update_Time();
    
    //Update_Encoder Values
    Update_Encoders();
    
    //Update Ultrasonic
    //Update_Ultra_Sonic();
    

    //Update Motors
    Update_Motors();


    //Update MPU 6050
    //Update_MPU6050();
    
  
    //Update battery
    Update_Battery();
    
    
  
}




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Read from Serial Function

void Read_From_Serial()

{
   while(Serial.available() > 0)
    {
     
       int data = Serial.read();
       Messenger_Handler.process(data);
     
     
    } 
    
}










///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//OnMssg Complete function definition

void OnMssageCompleted()
{
   
  char reset[] = "r";
  char set_speed[] = "s";
  
  if(Messenger_Handler.checkString(reset))
  {
    
     Serial.println("Reset Done"); 
     Reset();
    
  }
  if(Messenger_Handler.checkString(set_speed))
  {
    
     //This will set the speed
     Set_Speed();
     return; 
    
    
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//do_Left_Encoder() Definitions
void do_Left_Encoder()
{
   // Test transition; since the interrupt will only fire on 'rising' we don't need to read pin A
  LeftEncoderBSet = digitalRead(Left_Encoder_PinB);   // read the input pin
  Left_Encoder_Ticks -= LeftEncoderBSet ? -1 : +1;
   
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//do_Right_Encoder() Definitions

void do_Right_Encoder()
{
  
  RightEncoderBSet = digitalRead(Right_Encoder_PinB);   // read the input pin
  Right_Encoder_Ticks += RightEncoderBSet ? -1 : +1;
 
 
  
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Set_Speed()
{
    
  motor_left_speed = Messenger_Handler.readLong();
  motor_right_speed = Messenger_Handler.readLong();
  
  
  
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Reset function
void Reset()
{
 
  digitalWrite(GREEN_LED,HIGH);
  delay(1000);
  digitalWrite(RESET_PIN,LOW);
  digitalWrite(GREEN_LED,LOW); 
 
  
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Will update both motors
void Update_Motors()
{
  
  moveRightMotor(motor_right_speed);
  moveLeftMotor(motor_left_speed);

  Serial.print("s");
  Serial.print("\t");
  Serial.print(motor_left_speed);
  Serial.print("\t");
  Serial.print(motor_right_speed);  
  Serial.print("\n");


}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Will update both encoder value through serial port
void Update_Encoders()
{
 
  Serial.print("e");
  Serial.print("\t");
  Serial.print(Left_Encoder_Ticks);
  Serial.print("\t");
  Serial.print(Right_Encoder_Ticks);
  Serial.print("\n");
  
  
  
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Will update ultrasonic sensors through serial port

void Update_Ultra_Sonic()
{
  digitalWrite(Trig, LOW);
  delayMicroseconds(2);
  digitalWrite(Trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(Trig, LOW);
  // The echo pin is used to read the signal from the PING))): a HIGH
  // pulse whose duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  duration = pulseIn(echo, HIGH);
  // convert the time into a distance
  cm = microsecondsToCentimeters(duration);
  
  //Sending through serial port
  Serial.print("u");
  Serial.print("\t");
  Serial.print(cm);
  Serial.print("\n");
  
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Update MPU6050

void Update_MPU6050()
{
  
  
  
  int16_t ax, ay, az;
  int16_t gx, gy, gz;

      // read raw accel/gyro measurements from device
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);


    //Serial.println("Test");
    // these methods (and a few others) are also available
 //   accelgyro.getAcceleration(&ax, &ay, &az);
   // accelgyro.getRotation(&gx, &gy, &gz);

    

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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Update_Time()
{
  
      
  CurrentMicrosecs = micros();
  LastUpdateMillisecs = millis();
  MicrosecsSinceLastUpdate = CurrentMicrosecs - LastUpdateMicrosecs;
  if (MicrosecsSinceLastUpdate < 0)
    {
	MicrosecsSinceLastUpdate = INT_MIN - LastUpdateMicrosecs + CurrentMicrosecs;

    }
  LastUpdateMicrosecs = CurrentMicrosecs;
  SecondsSinceLastUpdate = MicrosecsSinceLastUpdate / 1000000.0;

  Serial.print("t");
  Serial.print("\t");
  Serial.print(LastUpdateMicrosecs);
  Serial.print("\t");
  Serial.print(SecondsSinceLastUpdate);
  Serial.print("\n");
 
  
}





////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Conversion of microsecond to centimeter
long microsecondsToCentimeters(long microseconds)
{
// The speed of sound is 340 m/s or 29 microseconds per centimeter.
// The ping travels out and back, so to find the distance of the
// object we take half of the distance travelled.
return microseconds / 29 / 2;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Update_Battery()

{
 battery_level = analogRead(PC_4); 
 
 Serial.print("b");
 Serial.print("\t");
 Serial.print(battery_level);
 Serial.print("\n");

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Motr running function


void moveRightMotor(float rightServoValue)
{
  if (rightServoValue>0)
  {
       
 digitalWrite(A_1,HIGH);
 digitalWrite(B_1,LOW);
 analogWrite(PWM_1,rightServoValue);
    
  }
  else if(rightServoValue<0)
  {
 digitalWrite(A_1,LOW);
 digitalWrite(B_1,HIGH);
 analogWrite(PWM_1,abs(rightServoValue));
 
  }
  
  else if(rightServoValue == 0)
  {
 digitalWrite(A_1,HIGH);
 digitalWrite(B_1,HIGH);
// analogWrite(PWM_1,0);
    
    
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void moveLeftMotor(float leftServoValue)
{
 if (leftServoValue > 0)
  {
digitalWrite(A_2,LOW);
digitalWrite(B_2,HIGH);
analogWrite(PWM_2,leftServoValue);
  }
  else if(leftServoValue < 0)
  {
 digitalWrite(A_2,HIGH);
 digitalWrite(B_2,LOW);
 analogWrite(PWM_2,abs(leftServoValue));

  }
  else if(leftServoValue == 0)
  {

   digitalWrite(A_2,HIGH);
   digitalWrite(B_2,HIGH);
 //  analogWrite(PWM_2,0);
  
   }  
  
  
}

