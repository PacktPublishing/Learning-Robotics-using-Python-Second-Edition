
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
  Serial.begin(9600);


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
   
    
    Serial.print("LeftEncoderTicks");
    Serial.print("\t");
    Serial.print(_LeftEncoderTicks);
    Serial.print("\t");
    Serial.print("RightEncoderTicks");
    Serial.print("\t");
    Serial.print(_RightEncoderTicks);
    Serial.print("\n");
  

  delay(20);         // Wait for x milliseconds
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


