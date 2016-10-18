char btValue;
char lastBtValue;

#define FORWARD 1
#define REVERSE 2

#define LEFT 1
#define RIGHT 2

#define CHANNEL_A_DIR 12
#define CHANNEL_A_BRK 9
#define CHANNEL_A_PWR 3

#define CHANNEL_B_DIR 13
#define CHANNEL_B_BRK 8
#define CHANNEL_B_PWR 11

char motorSpeed = 255;

char outputPin[] = {8,9,12,13};
int outputPinsMax = sizeof(outputPin)/sizeof(outputPin[0]);

void setupPins() {
  for (int i = 0; i < outputPinsMax; i++)
    pinMode(outputPin[i], OUTPUT);
}

void setupMotorPins() {
  digitalWrite(CHANNEL_A_DIR, HIGH);
  digitalWrite(CHANNEL_B_DIR, HIGH);
  
  digitalWrite(CHANNEL_A_BRK, HIGH); // Make sure the car stays still at start
  digitalWrite(CHANNEL_B_BRK, HIGH);
}

void setup() {
  setupPins();
  setupMotorPins();
  
  Serial.begin(9600); // bluetooth;
  Serial.setTimeout(50);
}

void motorWrite(int state) {
  if (state == FORWARD){
    digitalWrite(CHANNEL_B_DIR, HIGH);
    digitalWrite(CHANNEL_B_BRK, LOW);
    analogWrite(CHANNEL_B_PWR, motorSpeed);
  } else if (state == REVERSE){
    digitalWrite(CHANNEL_B_DIR, LOW);
    digitalWrite(CHANNEL_B_BRK, LOW);
    analogWrite(CHANNEL_B_PWR, motorSpeed);
  } else
    digitalWrite(CHANNEL_B_BRK, HIGH);
}

void servoWrite(int state) {
  if (state == LEFT) {
    digitalWrite(CHANNEL_A_DIR, HIGH);
    digitalWrite(CHANNEL_A_BRK, LOW);
    analogWrite(CHANNEL_A_PWR, 255);
  } else if (state == RIGHT) {
    digitalWrite(CHANNEL_A_DIR, LOW);
    digitalWrite(CHANNEL_A_BRK, LOW);
    analogWrite(CHANNEL_A_PWR, 255);
  } else
    digitalWrite(CHANNEL_A_BRK, HIGH);
}

void updateState(char newState) {
  // DC MOTOR
  if      (newState == 'F' || newState == 'G' || newState == 'I') // DRIVE FORWARDS
    motorWrite(FORWARD);
  else if (newState == 'B' || newState == 'H' || newState == 'J') // DRIVE BACKWARDS
    motorWrite(REVERSE);
  else                                                            // STOP, RESET
    motorWrite(0);
    
  if      (newState == 'L' || newState == 'G' || newState == 'H') // TURN LEFT
    servoWrite(LEFT);
  else if (newState == 'R' || newState == 'I' || newState == 'J') // TURN RIGHT
    servoWrite(RIGHT);
  else                                                            // STOP, RESET
    servoWrite(0);
}

bool shouldUpdate()
{
  if (lastBtValue == btValue)
    return false;
  else
    lastBtValue = btValue;
    return true;
}

void loop() {
  btValue = 'S';
    
  if (Serial.available())
    btValue = Serial.read();

  if (shouldUpdate())
    updateState(btValue);
    
  delay(;
}
