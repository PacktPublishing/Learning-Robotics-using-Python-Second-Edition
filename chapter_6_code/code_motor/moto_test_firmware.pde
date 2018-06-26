void setup()
{
  pinMode(11, OUTPUT);   // sets the pin as output
  pinMode(12, OUTPUT);   // sets the pin as output
  pinMode(13, OUTPUT);   // sets the pin as output
  pinMode(8, OUTPUT);   // sets the pin as output
  pinMode(9, OUTPUT);   // sets the pin as output
  pinMode(10, OUTPUT);   // sets the pin as output
}

void loop()
{
  int i;
  digitalWrite(12,HIGH);
  digitalWrite(8,HIGH);
  digitalWrite(13,LOW);
  digitalWrite(9,LOW);
  for(i = 0;i<256;i++)
  { 
    analogWrite(11, i);  // analogRead values go from 0 to 1023, analogWrite values from 0 to 255
    analogWrite(10, i);
    delay(10);
  }
  digitalWrite(13,HIGH);
  digitalWrite(9,HIGH);
  digitalWrite(12,LOW);
  digitalWrite(8,LOW);
  for(i = 0;i<256;i++)
  { 
    analogWrite(11, i);  // analogRead values go from 0 to 1023, analogWrite values from 0 to 255
    analogWrite(10, i);
    delay(10);
  }
}
