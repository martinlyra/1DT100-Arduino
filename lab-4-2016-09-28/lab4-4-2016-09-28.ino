float valueX, valueY;       // variables where we keep our finished values
int restX, restY;           // variables where we keep our cabliration of the accelerometer

int buttonState;            // the state where the button is pressed or not

void setup() {
  Serial.begin(9600);       //setup the serial connection
  //cabilrate the accelerometer
  restX = analogRead(0);    // x-axis
  restY = analogRead(1);    // y-axis
}

void loop() {
  // read the values from the accelerometer
  int val0 = analogRead(0); // x-axis
  int val1 = analogRead(1); // y-axis

  // get an factor by subtracting the rest value from the read value and a constant factor
  valueX = (val0 - restX)/67.584; // X
  valueY = (val1 - restY)/67.584; // Y
  
  // we were using a pre-assembled proto-shield from university's inventory
  // the accelerometer was connected to the analog pins 0,1,2
  // while there was a button available at analog pin 5, and for some reason,
  // it was connected in a way that when depressed, it sent HIGH signals
  buttonState = analogRead(5) == 0 ? 1 : 0; // read the button state, if pressed (LOW or 0), then it is HIGH

  // Time to output it!
  Serial.print("B");
  Serial.print(buttonState);  // single character only
  Serial.print("X:");         // float number 3-6 characters
  Serial.print(valueX);
  Serial.print(";"); // splitter character for indexOf() in Processing
  Serial.print("Y:");         // float number 3-6 characters
  Serial.print(valueY);
  // The lines above will print a string in this format:
  // B[buttonState]X:[valueX];Y:[valueY]
  // Where the square brackets are where the variables will be placed, and as stated, which.
  
  Serial.println(""); // We are finished, terminate the string by sending a '\n'

  delay(30); // We don't need to send tons of data to the other end of the serial connection. (Wait 30 ms)
}
