  // mouseover serial

  // Demonstrates how to send data to the Arduino I/O board, in order to
  // turn ON a light if the mouse is over a square and turn it off
  // if the mouse is not.

  // created 2003-4
  // based on examples by Casey Reas and Hernando Barragan
  // modified 30 Aug 2011
  // by Tom Igoe
  // This example code is in the public domain.



  import processing.serial.*;

  float boxX;
  float boxY;
  int boxSize = 20;
  boolean mouseOverBox = false;

  Serial port;

  void setup() {
  size(200, 200);
  boxX = width/2.0;
  boxY = height/2.0;
  rectMode(RADIUS);

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

  String val;
  
  int buttonState;
  float fvalX, fvalY;

  void draw()
  {
    if (port.available() > 0)
      val = port.readStringUntil('\n');
    
    if (val != null)
    {
      println(val);
      
      int splitterIndex = val.indexOf(";");
      
      /*
      float valX = float(val.substring(0, splitterIndex - 1));
      float valY = float(val.substring(splitterIndex, val.length()-1));
      */
      try
      {
        String button = val.substring(1,2);
        String valX = val.substring(2+2, splitterIndex);
        String valY = val.substring(splitterIndex + (1+2), val.length()-1);
        
        fvalX = float(valX);
        fvalY = float(valY);
        buttonState = int(button);
        
        println("Value for X: " + fvalX + "\nValue for Y: " + fvalY);
        println("Should be drawn?: " + (boolean(buttonState) ? "Yes" : "No"));
      }
      catch (Exception exception)
      {
        println(exception);
      }
      
      background(51);
      
      if (buttonState == 1)
      {
        stroke(244);
        line(100, 100, 200*fvalX, 200*fvalY);
      }
    }
  }
