void setup()
{
  pinMode(13,OUTPUT);
  Serial.begin(9600);
}

char val;

void loop() { 
  if( Serial.available() )       // if data is available to read
  {
    val = Serial.read();         // read it and store it in 'val'
  }
  if( val == 'H' )               // if 'H' was received
  {
    digitalWrite(13, HIGH);  // turn ON the LED
  } else { 
    digitalWrite(13, LOW);   // otherwise turn it OFF
  }
  delay(100);                    // wait 100ms for next reading
} 

