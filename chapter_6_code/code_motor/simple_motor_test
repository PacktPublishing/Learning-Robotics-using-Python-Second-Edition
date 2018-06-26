///Left Motor  Pins
#define INA_1 12
#define INB_1 13
#define PWM_1 35

///Right Motor Pins
#define INA_2 5
#define INB_2 6
#define PWM_2 36
 


void setup()
{

//Setting Left Motor pin as OUTPUT
pinMode(INA_1,OUTPUT); 
pinMode(INB_1,OUTPUT);

//Setting Right Motor pin as OUTPUT
pinMode(INA_2,OUTPUT);
pinMode(INB_2,OUTPUT);
}


void loop()
{

//Move forward for 5 sec
move_forward();
delay(5000);

//Stop for 1 sec
stop();
delay(1000);

//Move backward for 5 sec
move_backward();
delay(5000);

//Stop for 1 sec
stop();
delay(1000);

//Move left for 5 sec
move_left();
delay(5000);

//Stop for 1 sec
stop();
delay(1000);

//Move right for 5 sec
move_right();
delay(5000);  

//Stop for 1 sec
stop();
delay(1000);

}
 
void move_forward()
{
//Setting CW rotation to and Left Motor  and CCW to Right Motor 
 //Left Motor 

digitalWrite(INA_1,HIGH);
digitalWrite(INB_1,LOW);
analogWrite(PWM_1,255);

//Right Motor 
digitalWrite(INA_2,LOW);
digitalWrite(INB_2,HIGH);
analogWrite(PWM_2,255);  

}



void move_left()
{
 
//Left Motor 
digitalWrite(INA_1,HIGH);
digitalWrite(INB_1,HIGH);
analogWrite(PWM_1,0);

//Right Motor 
digitalWrite(INA_2,LOW);
digitalWrite(INB_2,HIGH);
analogWrite(PWM_2,255);  


}

void move_right()
{

//Left Motor 
digitalWrite(INA_1,HIGH);
digitalWrite(INB_1,LOW);
analogWrite(PWM_1,255);

//Right Motor 
digitalWrite(INA_2,HIGH);
digitalWrite(INB_2,HIGH);
analogWrite(PWM_2,0);  
}

void stop()
{

//Left Motor 
digitalWrite(INA_1,HIGH);
digitalWrite(INB_1,HIGH);
analogWrite(PWM_1,0);

//Right Motor 
digitalWrite(INA_2,HIGH);
digitalWrite(INB_2,HIGH);
analogWrite(PWM_2,0);  
}

/////////////////////////////////////////////////

void move_backward()

{

//Left Motor 
digitalWrite(INA_1,LOW);
digitalWrite(INB_1,HIGH);
analogWrite(PWM_1,255);

//Right Motor 
digitalWrite(INA_2,HIGH);
digitalWrite(INB_2,LOW);
analogWrite(PWM_2,255);  
}
