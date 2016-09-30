  // This code is based of the "Mouseover Serial" example code from Arduino.cc

  import processing.serial.*;

  Serial port;

  void setup() {
  // The two comment blocks below here are from the example code.
  
  // List all the available serial ports in the output pane.
  // You will need to choose the port that the Arduino board is
  // connected to from this list. The first port in the list is
  // port #0 and the third port in the list is port #2.
  // if using Processing 2.1 or later, use Serial.printArray()
  println(Serial.list());

  // Open the port that the Arduino board is connected to (in this case #0)
  // Make sure to open the port at the same speed Arduino is using (9600bps)
  port = new Serial(this, Serial.list()[2], 9600);
  }

  String val;           // The string we get from reading the serial connection
  
  float fvalX, fvalY;   // Float factor values determinating the result

  void draw()
  {
    if (port.available() > 0)           // is data being sent?
      val = port.readStringUntil('\n'); // read the serial connection
    
    if (val != null)                    // if there was no data, skip
    {
      println(val);                     // debugging purposes
      
      int splitterIndex = val.indexOf(";"); // where is the ';' located? we need this for splitting the string into more useable data
      
      // since start off with incomplete strings because there are no UDP or TCP data transfer protocols,
      // sometimes the data are cut, try processing the data, else spit and try again
      //
      // the string is formated like this:
      // X:<valX>;Y:<valY>
      // for example it'd be
      // X:-0.45;Y:0.12     where valX = -0.45 and valY = 0.12
      try
      {
        String valX = val.substring(2, splitterIndex);  // get the X value
        String valY = val.substring(splitterIndex + (1+2), val.length()-1); // get the Y value
        
        fvalX = float(valX); // parse the string into a number
        fvalY = float(valY); // same here
        
        println("Value for X: " + fvalX + "\nValue for Y: " + fvalY); // output it! for debugging needs
      }
      catch (Exception exception)
      {
        println(exception); // Hah, we caught a fault, no problem!
      }
      
      background(51); // Paint it dark.
      
      stroke(244);    // Makes the line being drawn white
      line(100, 100, 200*fvalX, 200*fvalY); // the line starts at {100,100} and ends at {200*fvalX,200*fvalY}
    }
  }
