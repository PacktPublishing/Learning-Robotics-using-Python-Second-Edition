#include <Servo.h> 

///Pin number 23 and A7
const int c_AnalogInPin = 23 ; // Analog input pin that the potentiometer is attached to

///24 pin = PD_1
///25 PIN = PD_2
const int c_LeftServoOutPin = 24;    // Output pin that the servo is attached to
const int c_RightServoOutPin = 25;    // Output pin that the servo is attached to

Servo _LeftServo;
Servo _RightServo;

#define c_MaxMotorCV 90      // range is 0 ... 90 (half servo range)

// Quadrature encoders
// Left encoder
/// Pin 11 = PA_2
///Pin 31 = PF_4
#define c_LeftEncoderPinA 11
#define c_LeftEncoderPinB 31
#define LeftEncoderIsReversed
volatile bool _LeftEncoderBSet;
volatile long _LeftEncoderTicks = 0;

// Right encoder
///Pin 12 = PA_3
///Pin 32 = PD_7
#define c_RightEncoderPinA 12
#define c_RightEncoderPinB 32
volatile bool _RightEncoderBSet;
volatile long _RightEncoderTicks = 0;


void setup()
{
  // Configure the ADC pin
  Serial.begin(9600);
 // pinMode(c_AnalogInPin, INPUT);

  _LeftServo.attach(c_LeftServoOutPin);  // attaches the servo on specified pin to the servo object 
  _RightServo.attach(c_RightServoOutPin);  // attaches the servo on specified pin to the servo object 

  // Quadrature encoders
  // Left encoder
  pinMode(c_LeftEncoderPinA, INPUT_PULLUP);      // sets pin A as input
  pinMode(c_LeftEncoderPinB, INPUT_PULLUP);      // sets pin B as input
  attachInterrupt(c_LeftEncoderPinA, HandleLeftMotorInterruptA, RISING);

  // Right encoder
  pinMode(c_RightEncoderPinA, INPUT_PULLUP);      // sets pin A as input
  pinMode(c_RightEncoderPinB, INPUT_PULLUP);      // sets pin B as input
  attachInterrupt(c_RightEncoderPinA, HandleRightMotorInterruptA, RISING);
}

void loop()
{
  int sensorValue = analogRead(c_AnalogInPin);
  float normalizedRightMotorCV = sensorValue / 4095.0;
  float normalizedLeftMotorCV = normalizedRightMotorCV;
    
  float leftServoValue = mapFloat(normalizedLeftMotorCV, 0, 1, 90.0 - c_MaxMotorCV, 90.0 + c_MaxMotorCV);     // scale it to use it with the servo (value between 0 and 180) 
  float rightServoValue = mapFloat(normalizedRightMotorCV, 0, 1, 90.0 - c_MaxMotorCV, 90.0 + c_MaxMotorCV);     // scale it to use it with the servo (value between 0 and 180) 
    
  _LeftServo.write(leftServoValue);
  _RightServo.write(rightServoValue);

    Serial.print("sensor = " );
    Serial.print(sensorValue);
    Serial.print(", normalizedRightMotorCV = " );
    Serial.print(normalizedRightMotorCV);
    Serial.print(", servo = " );
    Serial.println(rightServoValue);
    
    
    Serial.print(_LeftEncoderTicks);
    Serial.print("\t");
    Serial.print(_RightEncoderTicks);
    Serial.print("\n");
  

  delay(20);         // Wait for x milliseconds
}

long mapFloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// Interrupt service routines for the left motor's quadrature encoder
void HandleLeftMotorInterruptA()
{
  // Test transition; since the interrupt will only fire on 'rising' we don't need to read pin A
  _LeftEncoderBSet = digitalRead(c_LeftEncoderPinB);   // read the input pin

  // and adjust counter + if A leads B
  #ifdef LeftEncoderIsReversed
    _LeftEncoderTicks -= _LeftEncoderBSet ? -1 : +1;
  #else
    _LeftEncoderTicks += _LeftEncoderBSet ? -1 : +1;
  #endif
}

// Interrupt service routines for the right motor's quadrature encoder
void HandleRightMotorInterruptA()
{
  // Test transition; since the interrupt will only fire on 'rising' we don't need to read pin A
  _RightEncoderBSet = digitalRead(c_RightEncoderPinB);   // read the input pin

  // and adjust counter + if A leads B
  #ifdef RightEncoderIsReversed
    _RightEncoderTicks -= _RightEncoderBSet ? -1 : +1;
  #else
    _RightEncoderTicks += _RightEncoderBSet ? -1 : +1;
  #endif
}


