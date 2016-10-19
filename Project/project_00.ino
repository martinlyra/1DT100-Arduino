// Bluetooth-controlled Car with Arduino
//
// Author: Martin Lyrå
// Date: 19-10-2016 (19th October 2016)
// License: MIT
//
// For Arduinos with a motor shield (Arduino's L293D shield was used in this example)
// Bluetooth module connected to pins 0 and 1 (TX and RX)
// Controlled by Calle.Co's "Arduino Bluetooth RC Car" available for Android phones only, last I checked... (October, 2016)
//
// This was created as my project for the first year of Computer Engineering at Linnéuniversitet, Växjö, Sweden. 

// FORWARD = LEFT = 1
// REVERSE = RIGHT = 2
//
// see the comment for motorWrite() and servoWrite() for more info
#define FORWARD 1
#define REVERSE 2

#define LEFT 1
#define RIGHT 2

// DC MOTOR CHANNEL
#define CHANNEL_A_DIR 12
#define CHANNEL_A_BRK 9
#define CHANNEL_A_PWM 3

// DC SERVO CHANNEL
#define CHANNEL_B_DIR 13
#define CHANNEL_B_BRK 8
#define CHANNEL_B_PWM 11

char motorSpeed = 0; // max 255 - used for analogWrite() for the speed of the motor

char outputPin[] = {8,9,12,13}; // for easy quick setup, insert or remove output pins here, rather than writing pinMode() for every pin
int outputPinsMax = sizeof(outputPin)/sizeof(outputPin[0]); // need to figure out how many elements there are in the element
// tip: analogWrite() pins don't need setup, you can use them without pinMode()

long previousMillis;  // timestamp of the previous interval
long currentMillis;   // current time

// Defines the interval as 50 ms
#define INTERVAL 50

// easy setup, rather than having to writing a new command for each pin we want to use
void setupPins() {
  for (int i = 0; i < outputPinsMax; i++) 
    pinMode(outputPin[i], OUTPUT);
}

void setupMotorPins() {
  digitalWrite(CHANNEL_A_DIR, HIGH);  // default
  digitalWrite(CHANNEL_B_DIR, HIGH);
  
  digitalWrite(CHANNEL_A_BRK, HIGH);  // Make sure the car stays still at start
  digitalWrite(CHANNEL_B_BRK, HIGH);
}

void setup() {
  analogWrite(CHANNEL_A_PWA, 0);      // another safe to make sure the car won't run away soon as the device is turned on
  analogWrite(CHANNEL_B_PWA, 0);
  
  setupPins();
  setupMotorPins();
  
  Serial.begin(9600);     // bluetooth;
  Serial.setTimeout(50);  // make sure we won't by any chance hangup reading an infinite stream of data (times out after 50ms)
}

// motorWrite and servoWrite do the same thing: change the direction and turn on when told to do so
// the only difference is: motorWrite is affected by speed meanwhile servoWrite won't be affected
// FORWARD = LEFT = 1
// REVERSE = RIGHT = 2
// 0 to stop (enables the brake)

void motorWrite(int state) {
  if (state == FORWARD){
    digitalWrite(CHANNEL_A_DIR, HIGH);        // send power
    digitalWrite(CHANNEL_A_BRK, LOW);         // disable the brake (turn on)
    analogWrite(CHANNEL_A_PWM, motorSpeed);   // send power based on duty cycle
  } else if (state == REVERSE){
    digitalWrite(CHANNEL_A_DIR, LOW);         // receive power (don't send power) -- reverse the direction
    digitalWrite(CHANNEL_A_BRK, LOW);
    analogWrite(CHANNEL_A_PWM, motorSpeed);
  } else
    digitalWrite(CHANNEL_A_BRK, HIGH);        // enable the brake (turn off)
}

void servoWrite(int state) {
  if (state == LEFT) {
    digitalWrite(CHANNEL_B_DIR, HIGH);
    digitalWrite(CHANNEL_B_BRK, LOW);
    analogWrite(CHANNEL_B_PWM, 255);
  } else if (state == RIGHT) {
    digitalWrite(CHANNEL_B_DIR, LOW);
    digitalWrite(CHANNEL_B_BRK, LOW);
    analogWrite(CHANNEL_B_PWM, 255);
  } else
    digitalWrite(CHANNEL_B_BRK, HIGH);
}

// the range for the speed can be set up like this:
//  0%                  50%                 100%  per cent speed (duty cycle)
//  |                   |                   |
//  0 - 1 - 2 - 3 - 4 - 5 - 6 - 7 - 8 - 9 - 10    as numbers
//  |                   |                   |
//  30  31  32  33  34  35  36  37  38  39  71    as raw char data (assumed it is Unicode)
//  |                   |                   |
//  0 - 1 - 2 - 3 - 4 - 5 - 6 - 7 - 8 - 9 - q     above represented as Unicode (assumed) letters

void changeSpeed(char speedc) {
  if (speedc == 'q')
    speedc = 10;
  else
    speedc = map(speedc,'0','9',0,9);
  motorSpeed = map(speedc,0,10,0,255);  
}

// I thought it'd be more agile if I put this here instead in loop()
// basically: it checks and updates as according to commands sent by loop(), from "Arduino Bluetooth RC Car" app
//
// see the comments for the function above regarding speed; the purpose of the 0 -> 9 and 'q' commands
//
// command matrix for directions:
// N is Forward, S is Back/Reverse, W is Left, E is Right.
// Consult the left side to figure out what each command on the right side mean
//
//   NW    N    NE    G    F    I
//     \   ^   /       \   ^   /    
//         |               | 
//    W <--o--> E     L <--o--> R   
//         |               |        
//     /   v   \       /   v   \      
//   SW    S    SE    H    B    J   
//
// see Bluetooth RC's "4. Putting It All Together and The Android Application" for more commands. 
// Beware, some of the letters are incosistent
//
// stops everything if there was no command/data passed in this function, same for unrecognized commands

void updateState(char newState) {
  // CHANGE SPEED
  if      ((newState >= '0' && newState <= '9') || newState == 'q')// MOVE THE GEAR UP (or down)!
    changeSpeed(newState);
    
  // DC MOTOR
  if      (newState == 'F' || newState == 'G' || newState == 'I') // DRIVE FORWARDS
    motorWrite(FORWARD);
  else if (newState == 'B' || newState == 'H' || newState == 'J') // DRIVE BACKWARDS
    motorWrite(REVERSE);
  else                                                            // STOP, RESET (does also do this when the BT link is lost)
    motorWrite(0);

  // DC SERVO  
  if      (newState == 'L' || newState == 'G' || newState == 'H') // TURN LEFT
    servoWrite(LEFT);
  else if (newState == 'R' || newState == 'I' || newState == 'J') // TURN RIGHT
    servoWrite(RIGHT);
  else                                                            // STOP, RESET (does also do this when the BT link is lost)
    servoWrite(0);
}

void loop() {
  currentMillis = millis();                       // replacement for delay()

  if (currentMillis - previousMillis >= INTERVAL || Serial.available()) // >= 50 ms or when there are data available
  {
    previousMillis = currentMillis;               // an interval has just elasped, change the timestamp to the latast for next interval
    char btValue = Serial.read();                 // grab the data, even if there isn't any available
          
    updateState(btValue);                         // if there was no data, the car will (or SHOULD) stop anyway
  }
}
